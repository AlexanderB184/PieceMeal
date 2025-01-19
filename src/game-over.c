#include <stdio.h>

#include "../include/chess.h"

int is_draw_by_repetition(const chess_state_t* chess_state) {
  if (chess_state->ply_counter - chess_state->ply_of_last_irreversible_move <
      5) {
    return 0;
  }
  zobrist_t current_zobrist = chess_state->zobrist;
  int repetitions = 1;
  for (int i = chess_state->ply_counter - 2;
       i >= chess_state->ply_of_last_irreversible_move && i >= 2; i -= 2) {
    if (chess_state->ply_stack[i].zobrist == current_zobrist) {
      repetitions++;
    }
  }
  return repetitions >= 3;
}

int is_draw_by_insufficient_material(const chess_state_t* chess_state) {
  if (chess_state->black_pieces.pawn_count > 0 ||
      chess_state->white_pieces.pawn_count > 0 ||
      chess_state->black_pieces.rook_count > 0 ||
      chess_state->white_pieces.rook_count > 0 ||
      chess_state->black_pieces.queen_count > 0 ||
      chess_state->white_pieces.queen_count > 0 ||
      chess_state->white_pieces.knight_count +
              chess_state->white_pieces.dark_bishop_count +
              chess_state->white_pieces.light_bishop_count >
          1 ||
      chess_state->black_pieces.knight_count +
              chess_state->black_pieces.dark_bishop_count +
              chess_state->black_pieces.light_bishop_count >
          1) {
    return 0;
  }
  return 1;
}

int is_draw_by_50_move_rule(const chess_state_t* chess_state) {
  return chess_state->half_move_clock >= 100;
}

int is_draw(const chess_state_t* chess_state) {
  return is_draw_by_repetition(chess_state) ||
         is_draw_by_50_move_rule(chess_state) ||
         is_draw_by_insufficient_material(chess_state);
}

int is_checkmate(const chess_state_t* chess_state) {
  if (!is_check(chess_state)) return 0;
  move_t moves[256];
  size_t move_count = generate_moves(chess_state, moves, chess_state->friendly_colour);
  for (size_t i = 0; i < move_count; i++) {
    if (is_legal(chess_state, moves[i])) {
      return 0;
    }
  }
  return 1;
}

#define KING_INCREMENTS_COUNT 8
#define QUEEN_INCREMENTS_COUNT 8
#define KNIGHT_INCREMENTS_COUNT 8
#define BISHOP_INCREMENTS_COUNT 4
#define ROOK_INCREMENTS_COUNT 4

static const sq0x88_t king_increments_list[KING_INCREMENTS_COUNT] = {
    1, 255, 240, 16, 241, 239, 15, 17};
static const sq0x88_t knight_increments_list[KNIGHT_INCREMENTS_COUNT] = {
    18, 14, 33, 31, 242, 238, 225, 223};
static const sq0x88_t queen_increments_list[QUEEN_INCREMENTS_COUNT] = {
    1, 255, 240, 16, 241, 239, 15, 17};
static const sq0x88_t bishop_increments_list[BISHOP_INCREMENTS_COUNT] = {
    241, 239, 15, 17};
static const sq0x88_t rook_increments_list[ROOK_INCREMENTS_COUNT] = {1, 255,
                                                                     240, 16};

int is_stalemate(const chess_state_t* chess_state) {
  if (is_check(chess_state)) return 0;
  move_t moves[256];
  size_t move_count = castling_moves(chess_state, moves, 0, chess_state->friendly_pieces->king_square, chess_state->friendly_colour);
  for (size_t i = 0; i < move_count; i++) {
    if (is_legal(chess_state, moves[i])) {
      return 0;
    }
  }
  move_count = king_moves(chess_state, moves, 0, chess_state->friendly_pieces->king_square, chess_state->friendly_colour);
  for (size_t i = 0; i < move_count; i++) {
    if (is_legal(chess_state, moves[i])) {
      return 0;
    }
  }
  FOR_EACH_PIECE(chess_state->friendly_pieces, pawn, square) {
    size_t move_count = pawn_moves(chess_state, moves, 0, square, chess_state->friendly_colour);
    for (size_t i = 0; i < move_count; i++) {
      if (is_legal(chess_state, moves[i])) {
        return 0;
      }
    }
  }

  FOR_EACH_PIECE(chess_state->friendly_pieces, knight, square) {
    size_t move_count = knight_moves(chess_state, moves, 0, square, chess_state->friendly_colour);
    for (size_t i = 0; i < move_count; i++) {
      if (is_legal(chess_state, moves[i])) {
        return 0;
      }
    }
  }

  FOR_EACH_PIECE(chess_state->friendly_pieces, light_bishop, square) {
    size_t move_count = sliding_moves(chess_state, moves, 0, square, chess_state->friendly_colour, bishop_increments_list, BISHOP_INCREMENTS_COUNT);
    for (size_t i = 0; i < move_count; i++) {
      if (is_legal(chess_state, moves[i])) {
        return 0;
      }
    }
  }

  FOR_EACH_PIECE(chess_state->friendly_pieces, dark_bishop, square) {
    size_t move_count = sliding_moves(chess_state, moves, 0, square, chess_state->friendly_colour, bishop_increments_list, BISHOP_INCREMENTS_COUNT);
    for (size_t i = 0; i < move_count; i++) {
      if (is_legal(chess_state, moves[i])) {
        return 0;
      }
    }
  }

  FOR_EACH_PIECE(chess_state->friendly_pieces, rook, square) {
    size_t move_count = sliding_moves(chess_state, moves, 0, square, chess_state->friendly_colour, rook_increments_list, ROOK_INCREMENTS_COUNT);
    for (size_t i = 0; i < move_count; i++) {
      if (is_legal(chess_state, moves[i])) {
        return 0;
      }
    }
  }
  FOR_EACH_PIECE(chess_state->friendly_pieces, queen, square) {
    size_t move_count = sliding_moves(chess_state, moves, 0, square, chess_state->friendly_colour, queen_increments_list, QUEEN_INCREMENTS_COUNT);
    for (size_t i = 0; i < move_count; i++) {
      if (is_legal(chess_state, moves[i])) {
        return 0;
      }
    }
  }
  return 1;
}

enum gameover_state is_gameover(const chess_state_t* chess_state) {
  if (is_checkmate(chess_state)) {
    return CHECKMATE;
  }
  if (is_stalemate(chess_state)) {
    return STALEMATE;
  }
  if (is_draw_by_50_move_rule(chess_state)) {
    return DRAW_BY_50_MOVE_RULE;
  }
  if (is_draw_by_repetition(chess_state)) {
    return DRAW_BY_REPETITION;
  }
  if (is_draw_by_insufficient_material(chess_state)) {
    return DRAW_BY_INSUFFICIENT_MATERIAL;
  }
  return ONGOING;
}

colour_t get_winner(const chess_state_t* chess_state) {
  if (is_checkmate(chess_state)) {
    return chess_state->black_to_move ? WHITE : BLACK;
  }
  if (is_stalemate(chess_state) || is_draw(chess_state)) {
    return EMPTY;
  }
  return EMPTY;
}