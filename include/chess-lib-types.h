#ifndef CHESS_LIB_TYPES_H
#define CHESS_LIB_TYPES_H

#include <stddef.h>
#include <stdint.h>

// coordinates stored in 0x88 format
// upper nibble represents the rank
// lower nibble represents the file
// bit packing 0b0rrr0fff
// bits marked r are the rank, bits marked f are the file
// allows easy off the board checks
// https://www.chessprogramming.org/0x88
typedef uint_fast8_t sq0x88_t;

// cordinates stored in 0..63 format
typedef uint_fast8_t sq8x8_t;

// 0 indexed rank
typedef uint_fast8_t rank07_t;

// 0 indexed file
typedef uint_fast8_t file07_t;

// difference between two 0x88 coordinates
// is unique for all directions and distances unlike for 8x8 coordinates
typedef uint_fast8_t diff0x88_t;

// https://www.chessprogramming.org/Butterfly_Boards
typedef uint16_t butterfly_index_t;

// chess piece stored with disjoint piece flags
// https://www.chessprogramming.org/Pieces#Disjoint_Piece_Flags
// specific flag values derived from
// https://pure.uvt.nl/ws/files/1098572/Proefschrift_Fritz_Reul_170609.pdf
typedef uint_fast8_t piece_t;

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
  WHITE_KING = WHITE |KING,

  BLACK_PAWN = BLACK | PAWN,
  BLACK_KNIGHT = BLACK | KNIGHT,
  BLACK_BISHOP = BLACK | BISHOP,
  BLACK_ROOK = BLACK | ROOK,
  BLACK_QUEEN = BLACK | QUEEN,
  BLACK_KING = BLACK | KING,

} disjoint_piece_flag_encoding;

typedef enum {
  WHITE_QUEEN_SIDE = 0b1000,
  WHITE_KING_SIDE = 0b0100,
  BLACK_QUEEN_SIDE = 0b0010,
  BLACK_KING_SIDE = 0b0001,
  WHITE_BOTH_SIDES = WHITE_QUEEN_SIDE | WHITE_KING_SIDE,
  BLACK_BOTH_SIDES = BLACK_QUEEN_SIDE | BLACK_KING_SIDE,
  ALL_RIGHTS = WHITE_BOTH_SIDES | BLACK_BOTH_SIDES,
  NO_RIGHTS = 0,
} castle_rights_t;

// move flags
typedef enum {
  PROMOTION = 0b1000,
  CAPTURE = 0b0100,
  SPECIAL1 = 0b0010,
  SPECIAL0 = 0b0001,
  QUIET_MOVE = 0b0000,

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

// board stored in 0x88 form
// https://www.chessprogramming.org/0x88
typedef struct {
  piece_t board_0x88[16 * 16];
  sq0x88_t enpassent;
  int move_number, half_move_clock;
  int black_to_move;
  castle_rights_t rights;
} board_t;

enum {
  MAX_PAWNS = 8,
  MAX_KNIGHTS = 2 + 8,
  MAX_BISHOPS = 1 + 4,
  MAX_ROOKS = 2 + 8,
  MAX_QUEENS = 1 + 8,
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
  uint16_t bitpacked_data;
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
  piece_t board[16 * 16]; // board stored as 0x88 board with borders
  piece_list_t white_pieces, black_pieces; // detailed piece lists

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
  colour_t friendly_colour, enemy_colour; // for relative colour comparisons
  sq0x88_t up_increment, down_increment; // for pawn pushes and piece square tables
  piece_list_t *friendly_pieces, *enemy_pieces; // for move and attack gen

} chess_state_t;


#endif