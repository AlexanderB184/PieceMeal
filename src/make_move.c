// #include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/chess.h"

void push_ply_stack(chess_state_t* chess_state, move_t move) {
  if (chess_state->ply_counter >= chess_state->ply_stack_capacity) {
    chess_state->ply_stack_capacity = chess_state->ply_stack_capacity * 2;
    size_t ply_stack_size =
        sizeof(ply_stack_item_t) * chess_state->ply_stack_capacity;
    chess_state->ply_stack = realloc(chess_state->ply_stack, ply_stack_size);
  }
  chess_state->ply_stack[chess_state->ply_counter++] = (ply_stack_item_t){
      .move = move,
      .enpassent = chess_state->enpassent_target,
      .half_move_clock = chess_state->half_move_clock,
      .rights = chess_state->castle_rights,
      .captured = piece(chess_state, get_to(move)),
      .zobrist = chess_state->zobrist,
      .check_square = chess_state->check_square,
      .n_checks = chess_state->n_checks,
      .discovered_check = chess_state->discovered_check,
      .last_irreversible = chess_state->ply_of_last_irreversible_move,
  };
}

void update_board(chess_state_t* chess_state, move_t move) {
  sq0x88_t from = get_from(move);
  sq0x88_t to = get_to(move);

  if (is_enpassent(move)) {
    remove_piece(chess_state,
                 chess_state->enpassent_target - chess_state->up_increment);
  } else if (is_capture(move)) {
    remove_piece(chess_state, to);
  }

  if (!is_promotion(move)) {
    //  move piece
    move_piece(chess_state, from, to);

  } else {
    remove_piece(chess_state, from);
    place_piece(chess_state, to,
                get_promotes_to(move) | chess_state->friendly_colour);
  }

  if (is_king_castle(move)) {
    move_piece(chess_state, from + 3, to - 1);
  }
  if (is_queen_castle(move)) {
    move_piece(chess_state, from - 4, to + 1);
  }
}

void update_rights(chess_state_t* chess_state, move_t move) {
  sq0x88_t from = get_from(move);
  sq0x88_t to = get_to(move);
  // castle rights
  if (from == A1 || to == A1) {
    chess_state->castle_rights &= ~WHITE_QUEEN_SIDE;
  }
  if (from == H1 || to == H1) {
    chess_state->castle_rights &= ~WHITE_KING_SIDE;
  }
  if (from == A8 || to == A8) {
    chess_state->castle_rights &= ~BLACK_QUEEN_SIDE;
  }
  if (from == H8 || to == H8) {
    chess_state->castle_rights &= ~BLACK_KING_SIDE;
  }
  if (from == E1) {
    chess_state->castle_rights &= ~WHITE_BOTH_SIDES;
  }
  if (from == E8) {
    chess_state->castle_rights &= ~BLACK_BOTH_SIDES;
  }
}

void update_enpassent_target(chess_state_t* chess_state, move_t move) {
  if (get_flags(move) == DOUBLE_PAWN_PUSH) {
    chess_state->enpassent_target = get_from(move) + chess_state->up_increment;
  } else {
    chess_state->enpassent_target = 0x88;
  }
}

void update_half_move_clock(chess_state_t* chess_state, move_t move) {
  if (piece(chess_state, get_from(move)) & PAWN || is_capture(move)) {
    chess_state->ply_of_last_irreversible_move = chess_state->ply_counter;
    chess_state->half_move_clock = 0;
  } else {
    chess_state->half_move_clock++;
  }
}

#define SWAP(type, lhs, rhs) \
  {                          \
    type temp = (lhs);       \
    (lhs) = (rhs);           \
    (rhs) = temp;            \
  }

void update_turn(chess_state_t* chess_state) {
  chess_state->black_to_move = !chess_state->black_to_move;
  chess_state->zobrist = zobrist_flip_turn(chess_state->zobrist);
  SWAP(colour_t, chess_state->friendly_colour, chess_state->enemy_colour);
  SWAP(piece_list_t*, chess_state->friendly_pieces, chess_state->enemy_pieces);
  SWAP(sq0x88_t, chess_state->up_increment, chess_state->down_increment);
}

void make_move(chess_state_t* chess_state, move_t move) {
  if (is_null_move(move)) {
    push_ply_stack(chess_state, move);
    update_half_move_clock(chess_state, move);
    update_enpassent_target(chess_state, move);
    update_turn(chess_state);
    return;
  }
#ifdef DEBUG
  if (piece(chess_state, move.to) & KING) {
    trace_ply_stack(chess_state);
    printf("illegal move detected (captures king)");
    abort();
  }
  if (piece_is_friendly(chess_state, move.to)) {
    trace_ply_stack(chess_state);
    printf("illegal move detected (captures friendly)");
    abort();
  }
  if (off_the_board(move.from) || off_the_board(move.to)) {
    trace_ply_stack(chess_state);
    printf("illegal move detected (off the board)");
    abort();
  }
#endif
  push_ply_stack(chess_state, move);
  update_half_move_clock(chess_state, move);
  update_board(chess_state, move);
  update_enpassent_target(chess_state, move);
  update_rights(chess_state, move);
  update_turn(chess_state);
  update_check(chess_state, move);
}

// restores the board to the state before the previous move was made
void unmake_move(chess_state_t* chess_state) {
  ply_stack_item_t unwound_state =
      chess_state->ply_stack[--chess_state->ply_counter];

  chess_state->check_square = unwound_state.check_square;
  chess_state->discovered_check = unwound_state.discovered_check;
  chess_state->n_checks = unwound_state.n_checks;
  chess_state->enpassent_target = unwound_state.enpassent;
  chess_state->castle_rights = unwound_state.rights;
  chess_state->half_move_clock = unwound_state.half_move_clock;
  chess_state->ply_of_last_irreversible_move = unwound_state.last_irreversible;

  move_t prev_move = unwound_state.move;
  piece_t captured_piece = unwound_state.captured;

  update_turn(chess_state);

  sq0x88_t from = get_from(prev_move);
  sq0x88_t to = get_to(prev_move);

  if (is_promotion(prev_move)) {
    remove_piece(chess_state, to);
    place_piece(chess_state, from, chess_state->friendly_colour | PAWN);
  } else {
    move_piece(chess_state, to, from);
  }

  if (is_enpassent(prev_move)) {
    place_piece(chess_state,
                chess_state->enpassent_target - chess_state->up_increment,
                chess_state->enemy_colour | PAWN);
  } else if (is_capture(prev_move)) {
    place_piece(chess_state, to, captured_piece);
  }

  if (is_king_castle(prev_move)) {
    move_piece(chess_state, to - 1, from + 3);
  }
  if (is_queen_castle(prev_move)) {
    move_piece(chess_state, to + 1, from - 4);
  }
}
