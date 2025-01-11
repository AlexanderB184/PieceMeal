#ifndef CHESS_H
#define CHESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

#define RANK_SIZE 16
#define FILE_SIZE 16
#define LEFT_BORDER 0
#define TOP_BORDER 0
#define MOVE_PRIORITY_MAX 0xFFF

#define MAXSCORE ((centipawn_t)(INT16_MAX))
#define MINSCORE ((centipawn_t)(-INT16_MAX))

#define PAWN_SCORE ((centipawn_t)(100))
#define KNIGHT_SCORE ((centipawn_t)(320))
#define BISHOP_SCORE ((centipawn_t)(330))
#define ROOK_SCORE ((centipawn_t)(500))
#define QUEEN_SCORE ((centipawn_t)(900))
#define KING_SCORE                                                      \
  ((centipawn_t)(KNIGHT_SCORE * 2 + BISHOP_SCORE * 2 + ROOK_SCORE * 2 + \
                 QUEEN_SCORE * 9))
#define CHECKMATE_SCORE ((centipawn_t)(-16000))
#define DRAW_SCORE ((centipawn_t)(0))

#define SQUARE_FROM_RANKFILE(file, rank) \
  ((rank) + TOP_BORDER) * FILE_SIZE + LEFT_BORDER + (file)

#define FOR_EACH_PIECE(PIECE_LIST, PIECE_TYPE, IDENTIFIER)                     \
  for (uint8_t index = 0, IDENTIFIER = (PIECE_LIST)->PIECE_TYPE##_list[index]; \
       index < (PIECE_LIST)->PIECE_TYPE##_count;                               \
       index++, IDENTIFIER = (PIECE_LIST)->PIECE_TYPE##_list[index])

#ifdef DEBUG
#define PRINT_READ_ERRORS
#define PRINT_WRITE_ERRORS
#endif

#ifdef PRINT_READ_ERRORS
#define READ_ERROR(msg, ...)                                \
  {                                                         \
    fprintf(stderr, "READ ERROR: reading \"%s\" ", buffer); \
    fprintf(stderr, msg, ##__VA_ARGS__);                    \
    return -1;                                              \
  }
#else
#define READ_ERROR(msg, ...) \
  { return -1; }
#endif

#ifdef PRINT_WRITE_ERRORS
#define WRITE_ERROR(msg, ...)         \
  {                                   \
    fprintf(stderr, "WRITE ERROR: "); \
    printf(msg, ##__VA_ARGS__);       \
    buffer[0] = 0;                    \
    return -1;                        \
  }
#else
#define WRITE_ERROR(msg, ...) \
  {                           \
    buffer[0] = 0;            \
    return -1;                \
  }
#endif

// coordinates stored in 0x88 format
// upper nibble represents the rank
// lower nibble represents the file
// bit packing 0b0rrr0fff
// bits marked r are the rank, bits marked f are the file
// allows easy off the board checks
// https://www.chessprogramming.org/0x88
typedef uint8_t sq0x88_t;

// cordinates stored in 0..63 format
typedef uint8_t sq8x8_t;

// 0 indexed rank
typedef uint8_t rank07_t;

// 0 indexed file
typedef uint8_t file07_t;

// difference between two 0x88 coordinates
// is unique for all directions and distances unlike for 8x8 coordinates
typedef uint8_t diff0x88_t;

// https://www.chessprogramming.org/Butterfly_Boards
typedef uint16_t butterfly_index_t;

typedef int16_t centipawn_t;

// chess piece stored with disjoint piece flags
// https://www.chessprogramming.org/Pieces#Disjoint_Piece_Flags
// specific flag values derived from
// https://pure.uvt.nl/ws/files/1098572/Proefschrift_Fritz_Reul_170609.pdf
typedef uint8_t piece_t;

typedef uint8_t colour_t;

typedef enum {
  EMPTY = 0x00,
  PAWN = 0x01,
  KNIGHT = 0x02,
  BISHOP = 0x04,
  ROOK = 0x08,
  KING = 0x10,
  WHITE = 0x20,
  BLACK = 0x40,
  BORDER = 0x80,

  QUEEN = BISHOP | ROOK,

  PIECE_MASK = PAWN | KNIGHT | BISHOP | ROOK | QUEEN | KING,
  COLOUR_MASK = WHITE | BLACK,

  WHITE_PAWN = WHITE | PAWN,
  WHITE_KNIGHT = WHITE | KNIGHT,
  WHITE_BISHOP = WHITE | BISHOP,
  WHITE_ROOK = WHITE | ROOK,
  WHITE_QUEEN = WHITE | QUEEN,
  WHITE_KING = WHITE | KING,

  BLACK_PAWN = BLACK | PAWN,
  BLACK_KNIGHT = BLACK | KNIGHT,
  BLACK_BISHOP = BLACK | BISHOP,
  BLACK_ROOK = BLACK | ROOK,
  BLACK_QUEEN = BLACK | QUEEN,
  BLACK_KING = BLACK | KING,

} piece_encoding_t;

typedef enum {
  WHITE_QUEEN_SIDE = 0x8,
  WHITE_KING_SIDE = 0x4,
  BLACK_QUEEN_SIDE = 0x2,
  BLACK_KING_SIDE = 0x1,
  WHITE_BOTH_SIDES = WHITE_QUEEN_SIDE | WHITE_KING_SIDE,
  BLACK_BOTH_SIDES = BLACK_QUEEN_SIDE | BLACK_KING_SIDE,
  ALL_RIGHTS = WHITE_BOTH_SIDES | BLACK_BOTH_SIDES,
  NO_RIGHTS = 0,
} castle_rights_t;

// move flags
typedef enum {
  PROMOTION = 0x8,
  CAPTURE = 0x4,
  SPECIAL1 = 0x2,
  SPECIAL0 = 0x1,
  QUIET_MOVE = 0x0,

  DOUBLE_PAWN_PUSH = SPECIAL0,
  KING_CASTLE = SPECIAL1,
  QUEEN_CASTLE = SPECIAL0 | SPECIAL1,
  ENPASSENT = CAPTURE | SPECIAL0,
  KNIGHT_PROMOTE_TO = 0,
  BISHOP_PROMOTE_TO = SPECIAL0,
  ROOK_PROMOTE_TO = SPECIAL1,
  QUEEN_PROMOTE_TO = SPECIAL0 | SPECIAL1,
  KNIGHT_PROMOTION = PROMOTION | KNIGHT_PROMOTE_TO,
  BISHOP_PROMOTION = PROMOTION | BISHOP_PROMOTE_TO,
  ROOK_PROMOTION = PROMOTION | ROOK_PROMOTE_TO,
  QUEEN_PROMOTION = PROMOTION | QUEEN_PROMOTE_TO,
  KNIGHT_CAPTURE_PROMOTION = PROMOTION | CAPTURE | KNIGHT_PROMOTE_TO,
  BISHOP_CAPTURE_PROMOTION = PROMOTION | CAPTURE | BISHOP_PROMOTE_TO,
  ROOK_CAPTURE_PROMOTION = PROMOTION | CAPTURE | ROOK_PROMOTE_TO,
  QUEEN_CAPTURE_PROMOTION = PROMOTION | CAPTURE | QUEEN_PROMOTE_TO,

} move_flags_t;

enum {
  MAX_PAWNS = 8,
  MAX_KNIGHTS = 2 + 8,
  MAX_BISHOPS = 1 + 4,
  MAX_ROOKS = 2 + 8,
  MAX_QUEENS = 1 + 8,
};

enum {
  A1 = SQUARE_FROM_RANKFILE(0, 0),
  A2 = SQUARE_FROM_RANKFILE(0, 1),
  A3 = SQUARE_FROM_RANKFILE(0, 2),
  A4 = SQUARE_FROM_RANKFILE(0, 3),
  A5 = SQUARE_FROM_RANKFILE(0, 4),
  A6 = SQUARE_FROM_RANKFILE(0, 5),
  A7 = SQUARE_FROM_RANKFILE(0, 6),
  A8 = SQUARE_FROM_RANKFILE(0, 7),
  B1 = SQUARE_FROM_RANKFILE(1, 0),
  B2 = SQUARE_FROM_RANKFILE(1, 1),
  B3 = SQUARE_FROM_RANKFILE(1, 2),
  B4 = SQUARE_FROM_RANKFILE(1, 3),
  B5 = SQUARE_FROM_RANKFILE(1, 4),
  B6 = SQUARE_FROM_RANKFILE(1, 5),
  B7 = SQUARE_FROM_RANKFILE(1, 6),
  B8 = SQUARE_FROM_RANKFILE(1, 7),
  C1 = SQUARE_FROM_RANKFILE(2, 0),
  C2 = SQUARE_FROM_RANKFILE(2, 1),
  C3 = SQUARE_FROM_RANKFILE(2, 2),
  C4 = SQUARE_FROM_RANKFILE(2, 3),
  C5 = SQUARE_FROM_RANKFILE(2, 4),
  C6 = SQUARE_FROM_RANKFILE(2, 5),
  C7 = SQUARE_FROM_RANKFILE(2, 6),
  C8 = SQUARE_FROM_RANKFILE(2, 7),
  D1 = SQUARE_FROM_RANKFILE(3, 0),
  D2 = SQUARE_FROM_RANKFILE(3, 1),
  D3 = SQUARE_FROM_RANKFILE(3, 2),
  D4 = SQUARE_FROM_RANKFILE(3, 3),
  D5 = SQUARE_FROM_RANKFILE(3, 4),
  D6 = SQUARE_FROM_RANKFILE(3, 5),
  D7 = SQUARE_FROM_RANKFILE(3, 6),
  D8 = SQUARE_FROM_RANKFILE(3, 7),
  E1 = SQUARE_FROM_RANKFILE(4, 0),
  E2 = SQUARE_FROM_RANKFILE(4, 1),
  E3 = SQUARE_FROM_RANKFILE(4, 2),
  E4 = SQUARE_FROM_RANKFILE(4, 3),
  E5 = SQUARE_FROM_RANKFILE(4, 4),
  E6 = SQUARE_FROM_RANKFILE(4, 5),
  E7 = SQUARE_FROM_RANKFILE(4, 6),
  E8 = SQUARE_FROM_RANKFILE(4, 7),
  F1 = SQUARE_FROM_RANKFILE(5, 0),
  F2 = SQUARE_FROM_RANKFILE(5, 1),
  F3 = SQUARE_FROM_RANKFILE(5, 2),
  F4 = SQUARE_FROM_RANKFILE(5, 3),
  F5 = SQUARE_FROM_RANKFILE(5, 4),
  F6 = SQUARE_FROM_RANKFILE(5, 5),
  F7 = SQUARE_FROM_RANKFILE(5, 6),
  F8 = SQUARE_FROM_RANKFILE(5, 7),
  G1 = SQUARE_FROM_RANKFILE(6, 0),
  G2 = SQUARE_FROM_RANKFILE(6, 1),
  G3 = SQUARE_FROM_RANKFILE(6, 2),
  G4 = SQUARE_FROM_RANKFILE(6, 3),
  G5 = SQUARE_FROM_RANKFILE(6, 4),
  G6 = SQUARE_FROM_RANKFILE(6, 5),
  G7 = SQUARE_FROM_RANKFILE(6, 6),
  G8 = SQUARE_FROM_RANKFILE(6, 7),
  H1 = SQUARE_FROM_RANKFILE(7, 0),
  H2 = SQUARE_FROM_RANKFILE(7, 1),
  H3 = SQUARE_FROM_RANKFILE(7, 2),
  H4 = SQUARE_FROM_RANKFILE(7, 3),
  H5 = SQUARE_FROM_RANKFILE(7, 4),
  H6 = SQUARE_FROM_RANKFILE(7, 5),
  H7 = SQUARE_FROM_RANKFILE(7, 6),
  H8 = SQUARE_FROM_RANKFILE(7, 7),
};
// piece lists
// https://www.chessprogramming.org/Piece-Lists
// uses seperate lists for light and dark squared bishops
typedef struct {
  sq0x88_t king_square;
  sq0x88_t pawn_list[MAX_PAWNS];
  sq0x88_t knight_list[MAX_KNIGHTS];
  sq0x88_t light_bishop_list[MAX_BISHOPS];
  sq0x88_t dark_bishop_list[MAX_BISHOPS];
  sq0x88_t rook_list[MAX_ROOKS];
  sq0x88_t queen_list[MAX_QUEENS];
  uint8_t pawn_count, knight_count, light_bishop_count, dark_bishop_count,
      rook_count, queen_count;
  uint8_t indices_list[64];
} piece_list_t;

typedef struct move_t {
  sq0x88_t from, to;
  uint16_t priority_and_flags;
} move_t;

typedef uint16_t compact_move_t;

typedef uint64_t zobrist_t;

typedef struct ply_stack_item_t {
  move_t move;
  sq0x88_t enpassent;
  int half_move_clock;
  castle_rights_t rights;
  piece_t captured;
  sq0x88_t check_square;
  int n_checks;
  int discovered_check;
  zobrist_t zobrist;
  size_t last_irreversible;
} ply_stack_item_t;

typedef struct {
  // piece data
  piece_t
      board[RANK_SIZE * FILE_SIZE];  // board stored as 0x88 board with borders
  piece_list_t white_pieces, black_pieces;  // detailed piece lists

  // other board state info
  sq0x88_t enpassent_target;
  int ply_counter, half_move_clock;
  int black_to_move;
  castle_rights_t castle_rights;

  // running zobrist hash
  zobrist_t zobrist;

  // ply stack for unmake move and 3-fold repetition
  ply_stack_item_t* ply_stack;
  int ply_stack_capacity;
  int ply_of_last_irreversible_move;

  // check data from incremental check updates
  sq0x88_t check_square;
  int n_checks;
  int discovered_check;

  // data swapped each ply
  colour_t friendly_colour, enemy_colour;  // for relative colour comparisons
  sq0x88_t up_increment,
      down_increment;  // for pawn pushes and piece square tables
  piece_list_t *friendly_pieces, *enemy_pieces;  // for move and attack gen

} chess_state_t;

static const move_t null_move = {0, 0, 0};

// move_t interface functions

static inline sq8x8_t get_from(move_t move) { return move.from; }

static inline sq8x8_t get_to(move_t move) { return move.to; }

static inline move_flags_t get_flags(move_t move) {
  return (move_flags_t)(move.priority_and_flags & 0xF);
}

static inline int is_capture(move_t move) { return get_flags(move) & CAPTURE; }

static inline int is_enpassent(move_t move) {
  return get_flags(move) == ENPASSENT;
}

static inline int is_double_pawn_push(move_t move) {
  return get_flags(move) == DOUBLE_PAWN_PUSH;
}

static inline int is_queen_castle(move_t move) {
  return get_flags(move) == QUEEN_CASTLE;
}

static inline int is_king_castle(move_t move) {
  return get_flags(move) == KING_CASTLE;
}

static inline int is_promotion(move_t move) {
  return get_flags(move) & PROMOTION;
}

static inline piece_t get_promotes_to(move_t move) {
  switch (get_flags(move) & 3) {
    case KNIGHT_PROMOTE_TO:
      return KNIGHT;
    case BISHOP_PROMOTE_TO:
      return BISHOP;
    case ROOK_PROMOTE_TO:
      return ROOK;
    case QUEEN_PROMOTE_TO:
      return QUEEN;
    default:
      return EMPTY;
  }
}

static inline uint16_t get_priority(move_t move) {
  return (move.priority_and_flags & 0xFFF0) >> 4;
}

static inline move_t set_priority(move_t move, uint16_t prio) {
  move.priority_and_flags =
      ((prio << 4) & 0xFFF0) | (move.priority_and_flags & 0x000F);
  return move;
}

static inline int is_null_move(move_t move) { return move.from == move.to; }

// move constructor
static inline move_t move(sq8x8_t from, sq8x8_t to, int flags) {
#ifdef __cplusplus
  return move_t{from, to, (uint16_t)(flags & 0xF)};
#else
  return (move_t){from, to, (uint16_t)(flags & 0xF)};
#endif
}

// board interface

static inline piece_t piece(const chess_state_t* chess_state, sq0x88_t square) {
  return chess_state->board[square];
}

static inline int piece_is_white(const chess_state_t* chess_state,
                                 sq0x88_t square) {
  return chess_state->board[square] & WHITE;
}

static inline int piece_is_black(const chess_state_t* chess_state,
                                 sq0x88_t square) {
  return chess_state->board[square] & BLACK;
}

static inline int piece_is_friendly(const chess_state_t* chess_state,
                                    sq0x88_t square) {
  return chess_state->board[square] & chess_state->friendly_colour;
}

static inline int piece_is_enemy(const chess_state_t* chess_state,
                                 sq0x88_t square) {
  return chess_state->board[square] & chess_state->enemy_colour;
}

static inline int piece_is_empty(const chess_state_t* chess_state,
                                 sq0x88_t square) {
  return chess_state->board[square] == EMPTY;
}

static inline sq0x88_t enpassent_target(const chess_state_t* chess_state) {
  return chess_state->enpassent_target;
}

static inline int is_white_to_move(const chess_state_t* chess_state) {
  return !chess_state->black_to_move;
}

static inline int is_black_to_move(const chess_state_t* chess_state) {
  return chess_state->black_to_move;
}

// off the board checks

static inline uint8_t off_the_board(sq0x88_t sq0x88) {
  return sq0x88 & (uint8_t)0x88;
}

static inline rank07_t backrank(colour_t colour) {
  switch (colour & COLOUR_MASK) {
    case WHITE:
      return 7;
    case BLACK:
      return 0;
    default:
      return 0;
  }
}

// coordinate conversion funcitons

static inline sq8x8_t sq0x88_to_sq8x8(sq0x88_t sq0x88) {
  return (sq0x88 + (sq0x88 & 7)) >> 1;
}
static inline sq0x88_t sq8x8_to_sq0x88(sq8x8_t sq8x8) {
  return sq8x8 + (sq8x8 & ~7);
}

static inline rank07_t sq8x8_to_rank07(sq8x8_t sq8x8) { return sq8x8 >> 3; }

static inline file07_t sq8x8_to_file07(sq8x8_t sq8x8) { return sq8x8 & 7; }

static inline rank07_t sq0x88_to_rank07(sq0x88_t sq0x88) { return sq0x88 >> 4; }

static inline file07_t sq0x88_to_file07(sq0x88_t sq0x88) { return sq0x88 & 7; }

static inline sq0x88_t rankfile_to_sq0x88(rank07_t rank07, file07_t file07) {
  return (rank07 << 4) + file07;
}

static inline sq8x8_t rankfile_to_sq8x8(rank07_t rank07, file07_t file07) {
  return (rank07 << 3) + file07;
}

static inline diff0x88_t diff_0x88(sq0x88_t from, sq0x88_t to) {
  return (diff0x88_t)0x77 + from - to;
}

// square colour checks

static inline uint8_t is_light_square(sq0x88_t square) {
  return ((square & (sq0x88_t)1) ^ ((square >> 4) & (sq0x88_t)1));
}

static inline uint8_t is_dark_square(sq0x88_t square) {
  return !is_light_square(square);
}

static inline int compare_moves(move_t lhs, move_t rhs) {
  return lhs.from == rhs.from && lhs.to == rhs.to &&
         get_flags(lhs) == get_flags(rhs);
}

static inline compact_move_t compress_move(move_t move) {
  return sq0x88_to_sq8x8(move.from) | (sq0x88_to_sq8x8(move.to) << 6) |
         ((move.priority_and_flags & 0xF) << 12);
}

static inline move_t uncompress_move(compact_move_t compact_move) {
  return move(sq8x8_to_sq0x88(compact_move), sq8x8_to_sq0x88(compact_move >> 6),
              compact_move >> 12);
}

static inline colour_t piece_colour(const chess_state_t* chess_state,
                                    sq0x88_t square) {
  return (piece(chess_state, square) & COLOUR_MASK);
}

static inline int piece_is_colour(const chess_state_t* chess_state,
                                  sq0x88_t square, colour_t colour) {
  return piece_colour(chess_state, square) == colour;
}

static inline colour_t opposite_colour(colour_t colour) {
  switch (colour & COLOUR_MASK) {
    case BLACK:
      return WHITE;
    case WHITE:
      return BLACK;
    default:
      return EMPTY;
  }
}

static inline sq0x88_t pawn_push_increment(colour_t colour) {
  return (colour & WHITE) ? (sq0x88_t)(A2 - A1) : (sq0x88_t)(A1 - A2);
}

static inline const piece_list_t* get_piece_list(
    const chess_state_t* chess_state, colour_t colour) {
  return (colour & WHITE) ? &chess_state->white_pieces
                          : &chess_state->black_pieces;
}

static inline int is_promoting(sq0x88_t from, colour_t colour) {
  return (sq0x88_to_rank07(from) == 1 && colour == BLACK) ||
         (sq0x88_to_rank07(from) == 6 && colour == WHITE);
}

static inline int pawn_can_double_push(sq0x88_t from, colour_t colour) {
  return (sq0x88_to_rank07(from) == 6 && colour == BLACK) ||
         (sq0x88_to_rank07(from) == 1 && colour == WHITE);
}

static inline centipawn_t value_of(piece_t piece) {
  switch (piece & PIECE_MASK) {
    case PAWN:
      return PAWN_SCORE;
    case KNIGHT:
      return KNIGHT_SCORE;
    case BISHOP:
      return BISHOP_SCORE;
    case ROOK:
      return ROOK_SCORE;
    case QUEEN:
      return QUEEN_SCORE;
    case KING:
      return KING_SCORE;
    default:
      return 0;
  }
}

sq0x88_t queen_increment(sq0x88_t from, sq0x88_t to);

sq0x88_t rook_increment(sq0x88_t from, sq0x88_t to);

sq0x88_t bishop_increment(sq0x88_t from, sq0x88_t to);

sq0x88_t knight_increment(sq0x88_t from, sq0x88_t to);

sq0x88_t king_increment(sq0x88_t from, sq0x88_t to);

// returns true if the players piece at square can be pseudo captured
int is_under_attack(const chess_state_t* chess_state, sq0x88_t square,
                    piece_t player);

// returns true if there is atleast one piece threatening the king
int is_check(const chess_state_t* chess_state);

// returns true if two (or more) pieces are threatening the king
int is_double_check(const chess_state_t* chess_state);

// returns true if the king is checked through a discovered attack
int is_discover_check(const chess_state_t* chess_state);

//
int is_check_after_move(const chess_state_t* chess_state, move_t move);

// returns the square of the checking piece, if there is no checking piece it
// will return 0x88. if the position is double check it has undefined value and
// should not be used.
sq0x88_t checking_square(const chess_state_t* chess_state);

int can_capture_enpassent(const chess_state_t*, sq0x88_t from, colour_t colour);

int can_castle_king_side(const chess_state_t* chess_state, colour_t colour);

int can_castle_queen_side(const chess_state_t* chess_state, colour_t colour);

// returns index of first non white space character
long skip_whitespace(const char* buffer);

// writes a square in format file rank, e.g. a1, h4, c8
long write_square(char* buffer, long buffer_size, sq0x88_t square);

// reads a square in format file rank, e.g. a1, h4, c8
long read_square(const char* buffer, long buffer_size, sq0x88_t* square);

// writes the letter code for a piece, e.g. P, K, N
long write_piece(char* buffer, long buffer_size, piece_t piece);

// reads the letter code for a piece, e.g. P, K, N
long read_piece(const char* buffer, long buffer_size, piece_t* piece);

// write to buffer in algebraic notation
// e.g. e8=Q or Nb3
long write_algebraic_notation(char* buffer, long buffer_size,
                              chess_state_t* chess_state, move_t move);

// read from buffer a move in algebraic notation
// e.g. e8=Q or Nb3
long read_algebraic_notation(const char* buffer, long buffer_size,
                             const chess_state_t* chess_state, move_t* move);

// write to buffer in long algebraic notation
// e.g. e7e8Q or e1b3
long write_long_algebraic_notation(char* buffer, long buffer_size, move_t move);
// read from buffer a move in long algebraic notation
// e.g. e7e8Q or e1b3
long read_long_algebraic_notation(const char* buffer, long buffer_size,
                                  const chess_state_t* chess_state,
                                  move_t* move);

// writes a sequence of space seperated moves in algebraic notation.
long write_movetext(char* buffer, long buffer_size,
                    const chess_state_t* chess_state,
                    const char* start_position);

// when debugging the chess_state struct may be in an invalid state after an
// illegal move, using write_movetext to traverse the plystack may be dangerous
// so instead use this version. Since write_long_algebraic_notation does not
// depend on the chess_state, all moves can be written out in a traversal
// without cloning the game and making moves.
long write_movetext_debug(char* buffer, long buffer_size,
                          const chess_state_t* chess_state);

// writes the pgn out to a buffer
// event, site, date, round, names, and fen are all optional parameters, if set
// to NULL they will be left empty in pgn.
long write_pgn(char* buffer, long buffer_size, const chess_state_t* chess_state,
               const char* event, const char* site, const char* date,
               const char* round, const char* white_name,
               const char* black_name, const char* fen);

// loads default chess starting position
void load_start_position(chess_state_t* chess_state);

// release resources
void release_position(chess_state_t* chess_state);

// clones the position
void copy_position(chess_state_t* chess_state,
                   const chess_state_t* state_to_copy);

// clears the position to an empty board
void clear_position(chess_state_t* chess_state);

// load game from fen string
long load_position(chess_state_t* chess_state, const char* fen_string);

// write fen string of current position
long save_position(const chess_state_t* chess_state, char* buffer,
                   long buffer_size);

// updates the games state
void make_move(chess_state_t* chess_state, move_t move);

// restores the position to the previous state
void unmake_move(chess_state_t* chess_state);

// assumes the resulting position is not in check.
// returns true if the move doesn't move an absolutely pinned piece
int is_legal(const chess_state_t* chess_state, move_t move);

// assumes the resulting is not in check.
// returns true if the move doesn't move an absolutely pinned piece
// performs test for player 'colour'
// this allows legality tests on moves for the opposing player
int is_legal_internal(const chess_state_t* chess_state, move_t move,
                      colour_t colour);

// checks move rules, does not check if piece is pinned, must be combined with
// is_legal for full validation
int is_pseudo_legal(const chess_state_t* chess_state, move_t move);

int is_legal_internal(const chess_state_t* chess_state, move_t move,
                      colour_t colour);

// incremental move generation
// all move gen is divided into 3 categories
// generate promotions
// generate captures (excluding capture promotions)
// generate quiet moves
// generate moves performs all 3 of these together
// each produces psuedo legal moves that MUST be checked for legality before
// being played each has a legal variant that performs this for the user
// although this is not required and incremental legality checking is supported
// and even encouraged

// generates psuedo legal moves, is_legal test should be performed before move
// is played.
size_t generate_moves(const chess_state_t* chess_state, move_t* moves,
                      colour_t colour);

// generates psuedo legal capturing moves, is_legal test should be performed
// before move is played.
size_t generate_captures(const chess_state_t* chess_state, move_t* moves,
                         colour_t colour);

// generates psuedo legal quiet moves, is_legal test should be performed before
// move is played.
size_t generate_quiets(const chess_state_t* chess_state, move_t* moves,
                       colour_t colour);

// generates psuedo legal promoting moves, is_legal test should be performed
// before move is played.
size_t generate_promotions(const chess_state_t* chess_state, move_t* moves,
                           colour_t colour);

size_t generate_legal_moves(const chess_state_t* chess_state, move_t* moves,
                            colour_t colour);
size_t generate_legal_captures(const chess_state_t* chess_state, move_t* moves,
                               colour_t colour);
size_t generate_legal_quiets(const chess_state_t* chess_state, move_t* moves,
                             colour_t colour);
size_t generate_legal_promotions(const chess_state_t* chess_state,
                                 move_t* moves, colour_t colour);


size_t generate_moves_nocheck_internal(const chess_state_t* chess_state,
                                       move_t* moves, colour_t colour);

size_t generate_moves_check_internal(const chess_state_t* chess_state,
                                     move_t* moves, colour_t colour);

// zobrist own inverse function to incrementally update the running zobrist hash
// of the current position
zobrist_t zobrist_flip_turn(zobrist_t zobrist);

// zobrist own inverse function to incrementally update the running zobrist hash
// of the current position
zobrist_t zobrist_flip_piece(zobrist_t zobrist, piece_t piece, sq0x88_t square);

// checks if the position is drawn by any of draw by repetition, draw by
// insufficient material, or draw by 50 move rule
int is_draw(const chess_state_t* chess_state);

// checks for 3 fold repetition
int is_draw_by_repetition(const chess_state_t* chess_state);

// checks for draw from insufficient material
int is_draw_by_insufficient_material(const chess_state_t* chess_state);

// checks for draw by 50 move rule
int is_draw_by_50_move_rule(const chess_state_t* chess_state);

// is_checkmate and is_stalemate should be avoided for non leaf nodes to avoid
// duplication of move generation
int is_checkmate(const chess_state_t* chess_state);

// is_checkmate and is_stalemate should be avoided for non leaf nodes to avoid
// duplication of move generation
int is_stalemate(const chess_state_t* chess_state);

enum gameover_state {
  ONGOING = 0,
  CHECKMATE = 1,
  STALEMATE = -1,
  DRAW_BY_REPETITION = -2,
  DRAW_BY_50_MOVE_RULE = -3,
  DRAW_BY_INSUFFICIENT_MATERIAL = -4
};

// 0 for ongoing, 1 for checkmate, < 0 for draw/stalemate
enum gameover_state is_gameover(const chess_state_t* chess_state);

// WHITE if white wins, BLACK if black wins, EMPTY if draw or game ongoing, use
// is_gameover to check if game is ongoing
colour_t get_winner(const chess_state_t* chess_state);

// casts a ray starting at square and iterating in the opposite direction to inc
sq0x88_t backwards_ray_cast(const chess_state_t* chess_state, sq0x88_t square,
                            sq0x88_t inc);

// casts a ray starting at square and iterating in the same direction as inc
sq0x88_t forwards_ray_cast(const chess_state_t* chess_state, sq0x88_t from,
                           sq0x88_t inc);

void trace_ply_stack(const chess_state_t* chess_state);

void init_check(chess_state_t* chess_state);

void push_ply_stack(chess_state_t* chess_state, move_t move);

void update_board(chess_state_t* chess_state, move_t move);

void update_rights(chess_state_t* chess_state, move_t move);

void update_enpassent_target(chess_state_t* chess_state, move_t move);

void update_half_move_clock(chess_state_t* chess_state, move_t move);

void update_turn(chess_state_t* chess_state);

void update_check(chess_state_t* chess_state, move_t move);

void remove_piece(chess_state_t* chess_state, sq0x88_t target);

void place_piece(chess_state_t* chess_state, sq0x88_t target, piece_t piece);

void move_piece(chess_state_t* chess_state, sq0x88_t from, sq0x88_t to);

size_t knight_moves(const chess_state_t* chess_state, move_t* moves,
                    size_t move_count, sq0x88_t from, colour_t colour);

size_t king_moves(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t king_square, colour_t colour);

size_t castling_moves(const chess_state_t* chess_state, move_t* moves,
                      size_t move_count, sq0x88_t king_square, colour_t colour);

// flags should either be QUIET_MOVE or CAPTURE, add_promotion_moves will handle
// the promotion flags
size_t add_promotion_moves(move_t* moves, size_t move_count, sq0x88_t from,
                           sq0x88_t to, int flags);

size_t pawn_moves(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t from, colour_t colour);

size_t sliding_moves(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t from, colour_t colour,
                     const sq0x88_t* increments, int increments_count);

size_t sliding_quiets(const chess_state_t* chess_state, move_t* moves,
                      size_t move_count, sq0x88_t from,
                      const sq0x88_t* increments, int increments_count);

size_t sliding_captures(const chess_state_t* chess_state, move_t* moves,
                        size_t move_count, sq0x88_t from, colour_t colour,
                        const sq0x88_t* increments, int increments_count);

#ifdef __cplusplus
}
#endif

#endif