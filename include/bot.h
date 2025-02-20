
#ifndef BOT_H
#define BOT_H

#ifndef __USE_POSIX199309
#define __USE_POSIX199309
#endif

#include <time.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

#include "../include/chess.h"

#define BUTTERFLY_BOARD_SIZE (64 * 64)
#define MAX_KILLER_MOVES 2

struct match_state_t;
struct bot_settings_t;
struct bot_term_cond_t;
struct bot_t;
struct worker_t;

enum tt_entry_type { 
  TT_EMPTY = 0, // slot is empty
  TT_EXACT,     // slot is a pv node
  TT_UPPER,     // slot failed low
  TT_LOWER,     // slot failed high
};

// bitpacked data for transposition table entry
// layout
// 0..2 type
// 2..16 unused
// 16..24 age
// 24..32 depth
// 32..48 score
// 48..64 best move
typedef uint64_t entry_t;

// key value paired stored using xor validation
// key value is key ^ entry so to check if key matches do key == entry.key ^
// entry.entry this guarantees correctness as if two threads write the key and
// entry for a node simultaneously it will be invalidated
// https://www.chessprogramming.org/Shared_Hash_Table#Xor
typedef struct key_entry_pair_t {
  zobrist_t key;
  uint64_t entry;
} key_entry_pair_t;

// lockless shared transposition table
// https://www.chessprogramming.org/Shared_Hash_Table#Xor
typedef struct table_t {
  volatile struct key_entry_pair_t* items;
  uint64_t capacity;
} table_t;

typedef struct bot_settings_t {
  int debug;   // debug flag
  int Hash;    // size of transposition table in MB
  int Ponder;  // whether pondering is enabled
  // other settings ...
} bot_settings_t;

typedef struct bot_term_cond_t {
  long time_limit_ms;
  long node_limit_nds;
  long depth_limit_ply;
  long mate_in_ply;
} bot_term_cond_t;

typedef struct match_state_t {
  // time control info
  size_t wtime;
  size_t btime;
  size_t winc;
  size_t binc;
} match_state_t;

typedef struct bot_t {
  int n_threads;  // number of threads to use
  struct worker_t** workers;

  struct bot_settings_t settings;

  enum {
    SEARCHMODE_INACTIVE,
    SEARCHMODE_REGULAR,
    SEARCHMODE_PONDER
  } search_mode;  // inactive=0, regular=1, ponder=2

  // state
  atomic_bool running, abort;
  struct timespec start_time;
  atomic_intmax_t duration_ms;
  atomic_intmax_t nodes_searched;
  atomic_intmax_t depth_searched;

  // termination conditions
  struct bot_term_cond_t stop_cond;

  // match info
  struct match_state_t match_info;

  // position
  chess_state_t root_position;

  // shared structures
  table_t transpostion_table;
} bot_t;

//
// basic single threaded worker type
typedef struct worker_t {
  // bot api should only look at this bit
  int id;
  pthread_t handle;
  int terminated;
  bot_t* bot;

  chess_state_t position;
  int move_count;
  centipawn_t scores[256];
  move_t moves[256];
  compact_move_t killer_moves[64][MAX_KILLER_MOVES];
  int root_ply;
  int16_t history_heuristic[BUTTERFLY_BOARD_SIZE];
  int16_t butterfly_heuristic[BUTTERFLY_BOARD_SIZE];

} worker_t;

/**
 * Struct to store the search state
 */
typedef struct search_t {
  chess_state_t position;

  // root node info
  int root_move_count;
  centipawn_t root_move_scores[256];
  move_t root_moves[256];

  table_t * transpo_table;
  // table_t * pv_table; // use a seperate table just for pv nodes?!?
  compact_move_t killer_move_table[64][MAX_KILLER_MOVES];

  int root_ply;
} search_t;

/*
typedef struct {
  move_t move, refutation;
  score_cp_t score;
  int depth_searched;
} root_move_t;
*/

// initialises the bot with settings,
// if settings is null it uses default settings
int bot_init(bot_t* bot, bot_settings_t* settings);

// loads the fen string into position
int bot_load_fen(bot_t* bot, const char* postext);

// loads a move into position
int bot_load_move(bot_t* bot, const char* movetext);

// starts the bot
int bot_start(bot_t* bot);

// converts the ponder search into regular searching
int bot_ponder_hit(bot_t* bot);

// cancels the search
int bot_stop(bot_t* bot);

// releases bot resources
int bot_release(bot_t* bot);

// called when the bot completes a search or the search is cancelled
void bot_on_stop(bot_t* bot);

// checks if bot is still running
int bot_is_running(bot_t* bot);

// waits until the bot finishes running
int bot_wait(bot_t* bot);

// begins search
void* worker_start(void*);

// checks if this thread is the main thread, i.e. the first thread in the thread list
int is_main_thread(const worker_t*);

// checks if termination condition is met or if the bots abort flag is set
int stop(worker_t*);

// logs info to gui
void log_info();

// returns correctly formatted bestmove to gui
void bestmove(move_t bestmove, move_t ponder);

// add killer move to killer move list
void add_killer_move(compact_move_t* killer_moves, move_t move);

// gets the static exchange evaluation
// returns positive if it is a winning exchange
// returns negative if it is a losing exchange
// returns zero if it is an even exchange
centipawn_t static_exchange_evaluation(const chess_state_t* chess_state,
  move_t move);

// gets the index in the butterfly board for move
static inline int butterfly_index(move_t move) {
  int from = sq0x88_to_sq8x8(move.from);
  int to = sq0x88_to_sq8x8(move.to);
  return (to<<6)+from;
}

static inline void inc_butteryfly_board(int16_t* board, move_t move) {
  board[butterfly_index(move)]++;
}

static inline int16_t get_butteryfly_board(int16_t* board, move_t move) {
  return board[butterfly_index(move)];
}

void reset_butterfly_board(int16_t* board);

enum move_order_state {
  PRIORITY_PV_MOVE         = 0xA00,
  PRIORITY_WINNING_CAPTURE = 0x800,
  PRIORITY_NEUTRAL_CAPTURE = 0x600,
  PRIORITY_KILLER_MOVES    = 0x400,
  PRIORITY_LOSING_CAPTURE  = 0x200,
  PRIORITY_QUIET_MOVE      = 0x000,
};

// stores move ordering and moves
typedef struct move_list_t {
  move_t moves[256];
  size_t move_count;
  move_t hash_move;
  compact_move_t* killer_moves;
  int16_t* bh, *hh;
  enum move_order_state state;
} move_list_t;

// initializes move list
void init_move_list(const chess_state_t* position, move_list_t* move_list, move_t hash_move, compact_move_t* killer_moves, int16_t*,int16_t*);

// gets next move from move list the move ordering rules
// returns null_move when move list is exhausted
move_t next_move(const chess_state_t* position, move_list_t* move_list);

// gets next capture from move list the move ordering rules
// returns null_move when all captures from move list are exhausted
move_t next_capture(const chess_state_t* position, move_list_t* move_list);

move_t entry_best_move(entry_t entry);

centipawn_t entry_score(entry_t entry);

enum tt_entry_type entry_type(entry_t entry);

int entry_depth(entry_t entry);

int entry_age(entry_t entry);

entry_t make_entry(enum tt_entry_type type, move_t best_move, centipawn_t score,
                   int depth, int age);

void tt_init(table_t* table, uint64_t capacity);

void tt_free(table_t* table);

entry_t tt_get(table_t* table, zobrist_t key);

// stores using always replace policy
void tt_store(table_t* table, zobrist_t key, enum tt_entry_type type,
              move_t best_move, centipawn_t score, int depth, int age);

// stores using depth prefered policy
void tt_store_depth_prefered(table_t* table, zobrist_t key,
                             enum tt_entry_type type, move_t best_move,
                             centipawn_t score, int depth, int age);

// only stores node if it is an exact node
void tt_store_pv(table_t* table, zobrist_t key, enum tt_entry_type type,
                 move_t best_move, centipawn_t score, int depth, int age);

// searches root position to fixed depth
int      rootSearch(worker_t* worker, centipawn_t alpha, centipawn_t beta, int depth);

// implements pv search and nega-max
centipawn_t abSearch(worker_t* worker, centipawn_t alpha, centipawn_t beta, int depth);

// implements quiescence search with nega-max with check extensions
centipawn_t  qSearch(worker_t* worker, centipawn_t alpha, centipawn_t beta, int depth);

centipawn_t piece_value(sq0x88_t, piece_t piece);

centipawn_t material_score(const chess_state_t*);

// statically analyses position
centipawn_t eval(const chess_state_t* position);

// checks if position is either a threefold repetition or is a repetition within the search
int is_repetition(const chess_state_t* position, int ply_of_root);

static const centipawn_t king_square_table[64] = {
     20,  50,   0, -10, -25,  50,  60,  20,
     10,  20, -50, -75, -50,  20,  20,  10,
     10, -20, -75,-100,-100, -50, -50,  10,
    -20, -50,-110,-110,-110,-110,-110, -20,
   -125,-125,-125,-125,-125,-125,-125,-125,
   -150,-150,-150,-150,-150,-150,-150,-150, 
   -175,-175,-175,-175,-175,-175,-175,-175,
   -200,-200,-200,-200,-200,-200,-200,-200,
};
static const centipawn_t queen_square_table[64] = {
      0,   5,  10,  10,   5,   5,   5,   0,
      0,   5,  20,  15,  10,   5,  10,   0,
      0,  20,   0,   0,   0,  10,   0,   0,
     15,   0,   0,   0,   0,   0,   5,   0,
      0,   0,   0,   0,   0,   0,   0,   5,
      0,   0,   0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0,   0,   0,
    -10, -10, -10, -10, -10, -10, -10, -10,
};
static const centipawn_t rook_square_table[64] = {
      0,  10,  20,  30,  30,  20,  10,   0,
     10,   0,   0,  20,  20,   0,   0,  10,
     20,   0,   0,   0,   0,   0,   0,  20,
     30,   0,   0,   0,   0,   0,   0,  30,
     30,   0,   0,   0,   0,   0,   0,  30,
     20,   0,   0,   0,   0,   0,   0,  20, 
     50,  50,  50,  50,  50,  50,  50,  50,
      0,  10,  20,  30,  30,  20,  10,   0,
};
static const centipawn_t bishop_square_table[64] = {
     10,  -5,  -5,  -5,  -5,  -5,  -5,  10,
      5,  20,  10,  20,  20,  10,  20,   0,
     10,   0,   5,  20,  20,   5,   0,  10,
      0,   0,  20,  10,  10,  20,   0,   0,
      0,   0,   0,  10,  10,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0,   0,   0,
    -10,   0,   0,   0,   0,   0,   0, -10,
};
static const centipawn_t knight_square_table[64] = { // 10 for each plmove
   -100, -10, -10, -10, -10, -10, -10,-100,
    -40,   0,  10,  10,  10,  10,   0, -40,
    -20,   0,  20,  20,  20,  20,   0, -20,
    -10,   0,  20,  40,  40,  20,   0, -10,
    -10,   0,  20,  40,  40,  20,   0, -10,
    -20,   0,  20,  20,  20,  20,   0, -20, 
    -60, -40, -10, -10, -10, -10, -40, -60,
   -100, -80, -50, -50, -50, -50, -80,-100,
};

static const centipawn_t pawn_square_table[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
     50,  50, -10, -10, -10,  15,  35,  50,
     10,  10,   5,   5,   5,  10,  10,  10,
     20, -10,  20,  20,  20,   5, -10,  20,
     30,  10,  30,  30,  30,  15,  10,  30,
     75,  75,  75,  75,  75,  75,  75,  75, 
    200, 200, 200, 200, 200, 200, 200, 200,
      0,   0,   0,   0,   0,   0,   0,   0,
    };

#endif