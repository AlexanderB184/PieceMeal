#ifndef CHESS_LIB_INLINES_H
#define CHESS_LIB_INLINES_H

#include "chess-lib-types.h"

// move_t interface functions

static inline sq8x8_t get_from(move_t move) { return move.from; }

static inline sq8x8_t get_to(move_t move) { return move.to; }

static inline move_flags_t get_flags(move_t move) {
  return (move_flags_t)(move.bitpacked_data & 0xF);
}

static inline int is_capture(move_t move) { return get_flags(move) & CAPTURE; }

static inline int is_enpassent(move_t move) { return get_flags(move) == ENPASSENT; }

static inline int is_double_pawn_push(move_t move) {
  return get_flags(move) == DOUBLE_PAWN_PUSH;
}

static inline int is_queen_castle(move_t move) {
  return get_flags(move) == QUEEN_CASTLE;
}

static inline int is_king_castle(move_t move) {
  return get_flags(move) == KING_CASTLE;
}

static inline int is_promotion(move_t move) { return get_flags(move) & PROMOTION; }

static inline piece_t get_promotes_to(move_t move) {
  switch (get_flags(move) & 0b11) {
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
  return (move.bitpacked_data & 0xFFF0) >> 4;
}

static inline move_t set_priority(move_t move, uint16_t prio) {
  move.bitpacked_data = ((prio << 4) & 0xFFF0) | (move.bitpacked_data & 0x000F);
  return move;
}

static inline int is_null_move(move_t move) {
  return move.from == move.to;
}



// move constructor
static inline move_t move(sq8x8_t from, sq8x8_t to, uint16_t flags) {
  return (move_t){from, to, flags & 0xF};
}

// board interface

static inline piece_t piece(const chess_state_t* chess_state, sq0x88_t square) {
  return chess_state->board[square];
}

static inline int piece_is_white(const chess_state_t* chess_state, sq0x88_t square) {
  return chess_state->board[square] & WHITE;
}

static inline int piece_is_black(const chess_state_t* chess_state, sq0x88_t square) {
  return chess_state->board[square] & BLACK;
}

static inline int piece_is_friendly(const chess_state_t* chess_state,
                             sq0x88_t square) {
  return chess_state->board[square] & chess_state->friendly_colour;
}

static inline int piece_is_enemy(const chess_state_t* chess_state, sq0x88_t square) {
  return chess_state->board[square] & chess_state->enemy_colour;
}

static inline int piece_is_empty(const chess_state_t* chess_state, sq0x88_t square) {
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

static inline uint8_t off_the_board(sq0x88_t sq0x88) { return sq0x88 & (uint8_t)0x88; }

// coordinate conversion funcitons

static inline sq8x8_t sq0x88_to_sq8x8(sq0x88_t sq0x88) {
  return (sq0x88 + (sq0x88 & 7)) >> 1;
}
static inline sq0x88_t sq8x8_to_sq0x88(sq8x8_t sq8x8) { return sq8x8 + (sq8x8 & ~7); }

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
  return lhs.from == rhs.from && lhs.to == rhs.to && get_flags(lhs) == get_flags(rhs);
}

static inline compact_move_t compress_move(move_t move) {
  return sq0x88_to_sq8x8(move.from) | (sq0x88_to_sq8x8(move.to) << 6) | ((move.bitpacked_data & 0xF) << 12);
}

static inline move_t uncompress_move(compact_move_t compact_move) {
  return move(sq8x8_to_sq0x88(compact_move), sq8x8_to_sq0x88(compact_move >> 6), compact_move >> 12);
}

#endif