
#include <stdlib.h>

#include "../include/chess-lib.h"
#include "../include/private/chess-lib-internals.h"

#pragma region Helper Functions

// pawn helpers

int is_promoting(const chess_state_t* chess_state, sq0x88_t from) {
  return (sq0x88_to_rank07(from) == 1 &&
          chess_state->friendly_colour == BLACK) ||
         (sq0x88_to_rank07(from) == 6 && chess_state->friendly_colour == WHITE);
}

int pawn_can_double_push(const chess_state_t* chess_state, sq0x88_t from) {
  return (sq0x88_to_rank07(from) == 6 &&
          chess_state->friendly_colour == BLACK) ||
         (sq0x88_to_rank07(from) == 1 && chess_state->friendly_colour == WHITE);
}

int can_capture_enpassent(const chess_state_t* chess_state, sq0x88_t from) {
  return sq0x88_to_rank07(from + chess_state->up_increment) ==
             sq0x88_to_rank07(enpassent_target(chess_state)) &&
         abs(sq0x88_to_file07(from) -
             sq0x88_to_file07(enpassent_target(chess_state))) == 1;
}

// king helpers

int can_castle_king_side(const chess_state_t* chess_state) {
  sq0x88_t king_square = chess_state->friendly_pieces->king_square;
  return ((chess_state->friendly_colour == WHITE &&
           (chess_state->castle_rights & WHITE_KING_SIDE) != 0) ||
          (chess_state->friendly_colour == BLACK &&
           (chess_state->castle_rights & BLACK_KING_SIDE) != 0)) &&
         piece(chess_state, king_square + 1) == EMPTY &&
         piece(chess_state, king_square + 2) == EMPTY;
}

int can_castle_queen_side(const chess_state_t* chess_state) {
  sq0x88_t king_square = chess_state->friendly_pieces->king_square;
  return ((chess_state->friendly_colour == WHITE &&
           (chess_state->castle_rights & WHITE_QUEEN_SIDE) != 0) ||
          (chess_state->friendly_colour == BLACK &&
           (chess_state->castle_rights & BLACK_QUEEN_SIDE) != 0)) &&
         piece(chess_state, king_square - 1) == EMPTY &&
         piece(chess_state, king_square - 2) == EMPTY &&
         piece(chess_state, king_square - 3) == EMPTY;
}

#pragma endregion

#pragma region Piece Specific Move Generators

#pragma region Knight Moves
// knight movement

size_t knight_moves(const chess_state_t* chess_state, move_t* moves,
                    size_t move_count, sq0x88_t from) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_friendly(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    move_flags_t flags = (target_piece == EMPTY) ? (QUIET_MOVE) : (CAPTURE);
    moves[move_count++] = move(from, to, flags);
  }
  return move_count;
}

size_t knight_quiets(const chess_state_t* chess_state, move_t* moves,
                    size_t move_count, sq0x88_t from) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_friendly(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece != EMPTY) continue;
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }
  return move_count;
}

size_t knight_captures(const chess_state_t* chess_state, move_t* moves,
                    size_t move_count, sq0x88_t from) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_friendly(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece == EMPTY) continue;
    moves[move_count++] = move(from, to, CAPTURE);
  }
  return move_count;
}

#pragma endregion

#pragma region King Moves

size_t king_moves(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t king_square) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = king_square + king_increments_list[i];
    if (off_the_board(to) || piece_is_friendly(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    move_flags_t flags = (target_piece == EMPTY) ? (QUIET_MOVE) : (CAPTURE);
    moves[move_count++] = move(king_square, to, flags);
  }

  return move_count;
}

size_t king_quiets(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t from) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + king_increments_list[i];
    if (off_the_board(to) || piece_is_friendly(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece != EMPTY) continue;
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }

  return move_count;
}

size_t king_captures(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t from) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + king_increments_list[i];
    if (off_the_board(to) || piece_is_friendly(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece == EMPTY) continue;
    moves[move_count++] = move(from, to, CAPTURE);
  }

  return move_count;
}

size_t castling_moves(const chess_state_t* chess_state, move_t* moves,
                      size_t move_count, sq0x88_t king_square) {
  // castling
  if (((chess_state->friendly_colour == WHITE &&
        (chess_state->castle_rights & WHITE_KING_SIDE) != 0) ||
       (chess_state->friendly_colour == BLACK &&
        (chess_state->castle_rights & BLACK_KING_SIDE) != 0)) &&
      piece(chess_state, king_square + 1) == EMPTY &&
      piece(chess_state, king_square + 2) == EMPTY) {
    moves[move_count++] = move(king_square, king_square + 2, KING_CASTLE);
  }
  if (((chess_state->friendly_colour == WHITE &&
        (chess_state->castle_rights & WHITE_QUEEN_SIDE) != 0) ||
       (chess_state->friendly_colour == BLACK &&
        (chess_state->castle_rights & BLACK_QUEEN_SIDE) != 0)) &&
      piece(chess_state, king_square - 1) == EMPTY &&
      piece(chess_state, king_square - 2) == EMPTY &&
      piece(chess_state, king_square - 3) == EMPTY) {
    moves[move_count++] = move(king_square, king_square - 2, QUEEN_CASTLE);
  }
  return move_count;
}

#pragma endregion

#pragma region Pawn Moves

size_t pawn_moves(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = chess_state->up_increment;
  sq0x88_t to = from + inc;
  if (is_promoting(chess_state, from)) {
    if (piece(chess_state, to) == EMPTY) {
      moves[move_count++] = move(from, to, QUEEN_PROMOTION);
      moves[move_count++] = move(from, to, ROOK_PROMOTION);
      moves[move_count++] = move(from, to, BISHOP_PROMOTION);
      moves[move_count++] = move(from, to, KNIGHT_PROMOTION);
    }
    to = from + inc + 1;
    if (!off_the_board(to) && piece_is_enemy(chess_state, to)) {
      moves[move_count++] = move(from, to, QUEEN_CAPTURE_PROMOTION);
      moves[move_count++] = move(from, to, ROOK_CAPTURE_PROMOTION);
      moves[move_count++] = move(from, to, BISHOP_CAPTURE_PROMOTION);
      moves[move_count++] = move(from, to, KNIGHT_CAPTURE_PROMOTION);
    }
    to = from + inc - 1;
    if (!off_the_board(to) && piece_is_enemy(chess_state, to)) {
      moves[move_count++] = move(from, to, QUEEN_CAPTURE_PROMOTION);
      moves[move_count++] = move(from, to, ROOK_CAPTURE_PROMOTION);
      moves[move_count++] = move(from, to, BISHOP_CAPTURE_PROMOTION);
      moves[move_count++] = move(from, to, KNIGHT_CAPTURE_PROMOTION);
    }
    return move_count;
  }

  if (!off_the_board(to) && piece(chess_state, to) == EMPTY) {
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_enemy(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_enemy(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + 2 * inc;
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY &&
      piece(chess_state, to - inc) == EMPTY &&
      pawn_can_double_push(chess_state, from)) {
    moves[move_count++] = move(from, to, DOUBLE_PAWN_PUSH);
  }

  if (!off_the_board(enpassent_target(chess_state)) &&
      can_capture_enpassent(chess_state, from)) {
    moves[move_count++] = move(from, enpassent_target(chess_state), ENPASSENT);
  }

  return move_count;
}

size_t pawn_quiets(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = chess_state->up_increment;
  sq0x88_t to = from + inc;
  if (is_promoting(chess_state, from)) {
    return move_count;
  }

  if (!off_the_board(to) && piece(chess_state, to) == EMPTY) {
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }
  to = from + 2 * inc;
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY &&
      piece(chess_state, to - inc) == EMPTY &&
      pawn_can_double_push(chess_state, from)) {
    moves[move_count++] = move(from, to, DOUBLE_PAWN_PUSH);
  }
  return move_count;
}



size_t pawn_captures(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = chess_state->up_increment;
  sq0x88_t to = from + inc;
  if (is_promoting(chess_state, from)) {
    return move_count;
  }
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_enemy(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_enemy(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  if (!off_the_board(enpassent_target(chess_state)) &&
      can_capture_enpassent(chess_state, from)) {
    moves[move_count++] = move(from, enpassent_target(chess_state), ENPASSENT);
  }
  return move_count;
}

size_t pawn_promotions(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = chess_state->up_increment;
  sq0x88_t to = from + inc;
  if (!is_promoting(chess_state, from)) {
    return move_count;
  }
  if (piece(chess_state, to) == EMPTY) {
    moves[move_count++] = move(from, to, QUEEN_PROMOTION);
    moves[move_count++] = move(from, to, ROOK_PROMOTION);
    moves[move_count++] = move(from, to, BISHOP_PROMOTION);
    moves[move_count++] = move(from, to, KNIGHT_PROMOTION);
  }
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_enemy(chess_state, to)) {
    moves[move_count++] = move(from, to, QUEEN_CAPTURE_PROMOTION);
    moves[move_count++] = move(from, to, ROOK_CAPTURE_PROMOTION);
    moves[move_count++] = move(from, to, BISHOP_CAPTURE_PROMOTION);
    moves[move_count++] = move(from, to, KNIGHT_CAPTURE_PROMOTION);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_enemy(chess_state, to)) {
    moves[move_count++] = move(from, to, QUEEN_CAPTURE_PROMOTION);
    moves[move_count++] = move(from, to, ROOK_CAPTURE_PROMOTION);
    moves[move_count++] = move(from, to, BISHOP_CAPTURE_PROMOTION);
    moves[move_count++] = move(from, to, KNIGHT_CAPTURE_PROMOTION);
  }
  return move_count;
}

#pragma endregion

#pragma region Sliding Moves

size_t sliding_moves(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t from,
                     const sq0x88_t* increments, int increments_count) {
  for (sq0x88_t i = 0; i < increments_count; i++) {
    sq0x88_t inc = increments[i];
    sq0x88_t to;
    for (to = from + inc; piece(chess_state, to) == EMPTY; to += inc) {
      moves[move_count++] = move(from, to, QUIET_MOVE);
    }
    if (off_the_board(to) || piece_is_friendly(chess_state, to)) continue;

    moves[move_count++] = move(from, to, CAPTURE);
  }
  return move_count;
}

size_t sliding_quiets(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t from,
                     const sq0x88_t* increments, int increments_count) {
  for (sq0x88_t i = 0; i < increments_count; i++) {
    sq0x88_t inc = increments[i];
    sq0x88_t to;
    for (to = from + inc; piece(chess_state, to) == EMPTY; to += inc) {
      moves[move_count++] = move(from, to, QUIET_MOVE);
    }
  }
  return move_count;
}

size_t sliding_captures(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t from,
                     const sq0x88_t* increments, int increments_count) {
  for (sq0x88_t i = 0; i < increments_count; i++) {
    sq0x88_t inc = increments[i];
    sq0x88_t to = forwards_ray_cast(chess_state, from, inc);
    if (off_the_board(to) || piece_is_friendly(chess_state, to)) continue;
    moves[move_count++] = move(from, to, CAPTURE);
  }
  return move_count;
}

#pragma endregion

#pragma endregion

#pragma region Piece List Move Generators

#define GENERATE_FOR_ALL_PIECES(MOVE_TYPE)                                    \
  move_count = king_##MOVE_TYPE(chess_state, moves, move_count,               \
                                piece_lists->king_square);                    \
  FOR_EACH_PIECE(piece_lists, queen, square) {                                \
    move_count =                                                              \
        sliding_##MOVE_TYPE(chess_state, moves, move_count, square,           \
                            queen_increments_list, QUEEN_INCREMENTS_COUNT);   \
  }                                                                           \
  FOR_EACH_PIECE(piece_lists, rook, square) {                                 \
    move_count =                                                              \
        sliding_##MOVE_TYPE(chess_state, moves, move_count, square,           \
                            rook_increments_list, ROOK_INCREMENTS_COUNT);     \
  }                                                                           \
  FOR_EACH_PIECE(piece_lists, light_bishop, square) {                         \
    move_count =                                                              \
        sliding_##MOVE_TYPE(chess_state, moves, move_count, square,           \
                            bishop_increments_list, BISHOP_INCREMENTS_COUNT); \
  }                                                                           \
  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {                          \
    move_count =                                                              \
        sliding_##MOVE_TYPE(chess_state, moves, move_count, square,           \
                            bishop_increments_list, BISHOP_INCREMENTS_COUNT); \
  }                                                                           \
  FOR_EACH_PIECE(piece_lists, knight, square) {                               \
    move_count = knight_##MOVE_TYPE(chess_state, moves, move_count, square);  \
  }                                                                           \
                                                                              \
  FOR_EACH_PIECE(piece_lists, pawn, square) {                                 \
    move_count = pawn_##MOVE_TYPE(chess_state, moves, move_count, square);    \
  }

size_t generate_captures_internal(const chess_state_t* chess_state,
                                  const piece_list_t* piece_lists,
                                  move_t* moves) {
  size_t move_count = 0;

  GENERATE_FOR_ALL_PIECES(captures);

  return move_count;
}

size_t generate_quiets_internal(const chess_state_t* chess_state,
                                const piece_list_t* piece_lists,
                                move_t* moves) {
  size_t move_count = 0;
  move_count = castling_moves(chess_state, moves, move_count,
                              piece_lists->king_square);
  GENERATE_FOR_ALL_PIECES(quiets);

  return move_count;
}

size_t generate_promotions_internal(const chess_state_t* chess_state,
                                    const piece_list_t* piece_lists,
                                    move_t* moves) {
  size_t move_count = 0;
  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_promotions(chess_state, moves, move_count, square);
  }
  return move_count;
}

size_t generate_moves_internal(const chess_state_t* chess_state,
                               const piece_list_t* piece_lists, move_t* moves) {
  size_t move_count = 0;
  
  move_count = castling_moves(chess_state, moves, move_count,
                              piece_lists->king_square);
  GENERATE_FOR_ALL_PIECES(moves);
  
  return move_count;
}

#pragma endregion

#pragma region Pseudo Legal Move Generators

size_t generate_moves(const chess_state_t* chess_state, move_t* moves) {
  return generate_moves_internal(chess_state, chess_state->friendly_pieces,
                                 moves);
}

size_t generate_captures(const chess_state_t* chess_state, move_t* moves) {
  return generate_captures_internal(chess_state, chess_state->friendly_pieces,
                                    moves);
}

size_t generate_quiets(const chess_state_t* chess_state, move_t* moves) {
  return generate_quiets_internal(chess_state, chess_state->friendly_pieces,
                                  moves);
}

size_t generate_promotions(const chess_state_t* chess_state, move_t* moves) {
  return generate_promotions_internal(chess_state, chess_state->friendly_pieces,
                                      moves);
}

#pragma endregion

#pragma region Legal Move Generators

size_t remove_illegal_moves(const chess_state_t* chess_state, move_t* moves,
                            size_t move_count) {
  for (size_t i = 0; i < move_count; i++) {
    if (!is_legal(chess_state, moves[i])) {
      moves[i--] = moves[--move_count];
    }
  }
  return move_count;
}

size_t generate_legal_moves(const chess_state_t* chess_state, move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_moves(chess_state, moves));
}

size_t generate_legal_captures(const chess_state_t* chess_state,
                               move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_captures(chess_state, moves));
}

size_t generate_legal_quiets(const chess_state_t* chess_state, move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_quiets(chess_state, moves));
}

size_t generate_legal_promotions(const chess_state_t* chess_state,
                                 move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_promotions(chess_state, moves));
}

#pragma endregion