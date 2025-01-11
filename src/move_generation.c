
#include <stdlib.h>

#include "../include/chess.h"

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

// todo
// make piece list base move generation functions i.e. size_t
// generate_moves_internal(const chess_state_t* chess_state, const piece_list_t*
// piece_lists, move_t* moves); make incremental move generation functions i.
// size_t generate_captures(const chess_state_t* chess_state, move_t * moves);

int can_capture_enpassent_white(const chess_state_t* chess_state,
                                sq0x88_t from) {
  sq0x88_t inc = pawn_push_increment(WHITE);
  sq0x88_t target = enpassent_target(chess_state);
  return WHITE == chess_state->friendly_colour &&
         sq0x88_to_rank07(from + inc) == sq0x88_to_rank07(target) &&
         abs(sq0x88_to_file07(from) - sq0x88_to_file07(target)) == 1;
}

int can_capture_enpassent_black(const chess_state_t* chess_state,
                                sq0x88_t from) {
  sq0x88_t inc = pawn_push_increment(BLACK);
  sq0x88_t target = enpassent_target(chess_state);
  return BLACK == chess_state->friendly_colour &&
         sq0x88_to_rank07(from + inc) == sq0x88_to_rank07(target) &&
         abs(sq0x88_to_file07(from) - sq0x88_to_file07(target)) == 1;
}

// king helper functions

int can_castle_king_side_white(const chess_state_t* chess_state) {
  sq0x88_t king_square = chess_state->white_pieces.king_square;
  return (chess_state->castle_rights & WHITE_KING_SIDE) != 0 &&
         piece(chess_state, king_square + 1) == EMPTY &&
         piece(chess_state, king_square + 2) == EMPTY;
}

int can_castle_king_side_black(const chess_state_t* chess_state) {
  sq0x88_t king_square = chess_state->black_pieces.king_square;
  return (chess_state->castle_rights & BLACK_KING_SIDE) != 0 &&
         piece(chess_state, king_square + 1) == EMPTY &&
         piece(chess_state, king_square + 2) == EMPTY;
}

int can_castle_queen_side_white(const chess_state_t* chess_state) {
  sq0x88_t king_square = chess_state->white_pieces.king_square;
  return (chess_state->castle_rights & WHITE_QUEEN_SIDE) != 0 &&
         piece(chess_state, king_square - 1) == EMPTY &&
         piece(chess_state, king_square - 2) == EMPTY &&
         piece(chess_state, king_square - 3) == EMPTY;
}

int can_castle_queen_side_black(const chess_state_t* chess_state) {
  sq0x88_t king_square = chess_state->black_pieces.king_square;
  return (chess_state->castle_rights & BLACK_QUEEN_SIDE) != 0 &&
         piece(chess_state, king_square - 1) == EMPTY &&
         piece(chess_state, king_square - 2) == EMPTY &&
         piece(chess_state, king_square - 3) == EMPTY;
}

// knight movement

size_t knight_moves_white(const chess_state_t* chess_state, move_t* moves,
                          size_t move_count, sq0x88_t from) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_white(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece == EMPTY) {
      moves[move_count++] = move(from, to, QUIET_MOVE);
    } else if (target_piece != EMPTY) {
      moves[move_count++] = move(from, to, CAPTURE);
    }
  }
  return move_count;
}

size_t knight_moves_black(const chess_state_t* chess_state, move_t* moves,
                          size_t move_count, sq0x88_t from) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_black(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece == EMPTY) {
      moves[move_count++] = move(from, to, QUIET_MOVE);
    } else if (target_piece != EMPTY) {
      moves[move_count++] = move(from, to, CAPTURE);
    }
  }
  return move_count;
}

size_t knight_captures_white(const chess_state_t* chess_state, move_t* moves,
                             size_t move_count, sq0x88_t from) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_white(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece != EMPTY) {
      moves[move_count++] = move(from, to, CAPTURE);
    }
  }
  return move_count;
}

size_t knight_captures_black(const chess_state_t* chess_state, move_t* moves,
                             size_t move_count, sq0x88_t from) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_black(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece != EMPTY) {
      moves[move_count++] = move(from, to, CAPTURE);
    }
  }
  return move_count;
}

size_t knight_quiets(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t from) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece(chess_state, to) != EMPTY) {
      continue;
    }
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }
  return move_count;
}

size_t king_moves_white(const chess_state_t* chess_state, move_t* moves,
                        size_t move_count, sq0x88_t king_square) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = king_square + king_increments_list[i];
    if (off_the_board(to) || piece_is_white(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece == EMPTY) {
      moves[move_count++] = move(king_square, to, QUIET_MOVE);
    } else if (target_piece != EMPTY) {
      moves[move_count++] = move(king_square, to, CAPTURE);
    }
  }

  return move_count;
}

size_t king_moves_black(const chess_state_t* chess_state, move_t* moves,
                        size_t move_count, sq0x88_t king_square) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = king_square + king_increments_list[i];
    if (off_the_board(to) || piece_is_black(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece == EMPTY) {
      moves[move_count++] = move(king_square, to, QUIET_MOVE);
    } else if (target_piece != EMPTY) {
      moves[move_count++] = move(king_square, to, CAPTURE);
    }
  }

  return move_count;
}

size_t king_captures(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t king_square) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = king_square + king_increments_list[i];
    if (off_the_board(to) || piece_is_white(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece != EMPTY) {
      moves[move_count++] = move(king_square, to, CAPTURE);
    }
  }

  return move_count;
}

size_t king_captures(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t king_square) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = king_square + king_increments_list[i];
    if (off_the_board(to) || piece_is_black(chess_state, to)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece != EMPTY) {
      moves[move_count++] = move(king_square, to, CAPTURE);
    }
  }

  return move_count;
}

size_t king_quiets(const chess_state_t* chess_state, move_t* moves,
                   size_t move_count, sq0x88_t king_square) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = king_square + king_increments_list[i];

    if (off_the_board(to) || piece(chess_state, to) != EMPTY) {
      continue;
    }
    moves[move_count++] = move(king_square, to, QUIET_MOVE);
  }

  return move_count;
}

size_t castling_moves_white(const chess_state_t* chess_state, move_t* moves,
                            size_t move_count, sq0x88_t king_square) {
  // castling
  if (can_castle_king_side_white(chess_state)) {
    moves[move_count++] = move(king_square, king_square + 2, KING_CASTLE);
  }
  if (can_castle_queen_side_white(chess_state)) {
    moves[move_count++] = move(king_square, king_square - 2, QUEEN_CASTLE);
  }
  return move_count;
}

size_t castling_moves_black(const chess_state_t* chess_state, move_t* moves,
                            size_t move_count, sq0x88_t king_square) {
  // castling
  if (can_castle_king_side_black(chess_state)) {
    moves[move_count++] = move(king_square, king_square + 2, KING_CASTLE);
  }
  if (can_castle_queen_side_black(chess_state)) {
    moves[move_count++] = move(king_square, king_square - 2, QUEEN_CASTLE);
  }
  return move_count;
}

// flags should either be QUIET_MOVE or CAPTURE, add_promotion_moves will handle
// the promotion flags
size_t add_promotion_moves(move_t* moves, size_t move_count, sq0x88_t from,
                           sq0x88_t to, int flags) {
  moves[move_count++] = move(from, to, flags | QUEEN_PROMOTION);
  moves[move_count++] = move(from, to, flags | ROOK_PROMOTION);
  moves[move_count++] = move(from, to, flags | BISHOP_PROMOTION);
  moves[move_count++] = move(from, to, flags | KNIGHT_PROMOTION);
  return move_count;
}

size_t pawn_moves_white(const chess_state_t* chess_state, move_t* moves,
                        size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(WHITE);
  sq0x88_t to = from + inc;
  if (is_promoting_white(from)) {
    if (piece(chess_state, to) == EMPTY) {
      move_count = add_promotion_moves(moves, move_count, from, to, QUIET_MOVE);
    }
    to = from + inc + 1;
    if (!off_the_board(to) && piece_is_black(chess_state, to)) {
      move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
    }
    to = from + inc - 1;
    if (!off_the_board(to) && piece_is_black(chess_state, to)) {
      move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
    }
    return move_count;
  }
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY) {
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }
  to = from + 2 * inc;
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY &&
      piece(chess_state, to - inc) == EMPTY &&
      pawn_can_double_push(from, WHITE)) {
    moves[move_count++] = move(from, to, DOUBLE_PAWN_PUSH);
  }

  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_colour_black(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_colour_black(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }

  if (!off_the_board(enpassent_target(chess_state)) &&
      can_capture_enpassent_white(chess_state, from)) {
    moves[move_count++] = move(from, enpassent_target(chess_state), ENPASSENT);
  }

  return move_count;
}

size_t pawn_moves_black(const chess_state_t* chess_state, move_t* moves,
                        size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(BLACK);
  sq0x88_t to = from + inc;
  if (is_promoting_black(from)) {
    if (piece(chess_state, to) == EMPTY) {
      move_count = add_promotion_moves(moves, move_count, from, to, QUIET_MOVE);
    }
    to = from + inc + 1;
    if (!off_the_board(to) && piece_is_white(chess_state, to)) {
      move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
    }
    to = from + inc - 1;
    if (!off_the_board(to) && piece_is_white(chess_state, to)) {
      move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
    }
    return move_count;
  }
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY) {
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }
  to = from + 2 * inc;
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY &&
      piece(chess_state, to - inc) == EMPTY &&
      pawn_can_double_push(from, BLACK)) {
    moves[move_count++] = move(from, to, DOUBLE_PAWN_PUSH);
  }

  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_colour_white(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_colour_white(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }

  if (!off_the_board(enpassent_target(chess_state)) &&
      can_capture_enpassent_black(chess_state, from)) {
    moves[move_count++] = move(from, enpassent_target(chess_state), ENPASSENT);
  }

  return move_count;
}

size_t pawn_captures_white(const chess_state_t* chess_state, move_t* moves,
                           size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(WHITE);
  sq0x88_t to = from + inc;
  if (is_promoting_white(from)) {
    return move_count;
  }
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_black(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_black(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }

  if (!off_the_board(enpassent_target(chess_state)) &&
      can_capture_enpassent_white(chess_state, from)) {
    moves[move_count++] = move(from, enpassent_target(chess_state), ENPASSENT);
  }

  return move_count;
}

size_t pawn_captures_black(const chess_state_t* chess_state, move_t* moves,
                           size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(BLACK);
  sq0x88_t to = from + inc;
  if (is_promoting_black(from)) {
    return move_count;
  }
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_white(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_white(chess_state, to)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }

  if (!off_the_board(enpassent_target(chess_state)) &&
      can_capture_enpassent_black(chess_state, from)) {
    moves[move_count++] = move(from, enpassent_target(chess_state), ENPASSENT);
  }

  return move_count;
}

size_t pawn_quiets_white(const chess_state_t* chess_state, move_t* moves,
                         size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(WHITE);
  sq0x88_t to = from + inc;
  if (is_promoting_white(from)) {
    return move_count;
  }
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY) {
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }
  to = from + 2 * inc;
  if (pawn_can_double_push_white(from) && !off_the_board(to) &&
      piece(chess_state, to) == EMPTY &&
      piece(chess_state, to - inc) == EMPTY) {
    moves[move_count++] = move(from, to, DOUBLE_PAWN_PUSH);
  }

  return move_count;
}

size_t pawn_quiets_black(const chess_state_t* chess_state, move_t* moves,
                         size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(BLACK);
  sq0x88_t to = from + inc;
  if (is_promoting_black(from)) {
    return move_count;
  }
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY) {
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }
  to = from + 2 * inc;
  if (pawn_can_double_push_black(from) && !off_the_board(to) &&
      piece(chess_state, to) == EMPTY &&
      piece(chess_state, to - inc) == EMPTY) {
    moves[move_count++] = move(from, to, DOUBLE_PAWN_PUSH);
  }

  return move_count;
}

size_t pawn_promotions_white(const chess_state_t* chess_state, move_t* moves,
                             size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(WHITE);
  sq0x88_t to = from + inc;
  if (!is_promoting_white(from)) {
    return move_count;
  }
  if (piece(chess_state, to) == EMPTY) {
    move_count = add_promotion_moves(moves, move_count, from, to, QUIET_MOVE);
  }
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_black(chess_state, to)) {
    move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_black(chess_state, to)) {
    move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
  }
  return move_count;
}

size_t pawn_promotions_black(const chess_state_t* chess_state, move_t* moves,
                             size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(BLACK);
  sq0x88_t to = from + inc;
  if (!is_promoting_black(from)) {
    return move_count;
  }
  if (piece(chess_state, to) == EMPTY) {
    move_count = add_promotion_moves(moves, move_count, from, to, QUIET_MOVE);
  }
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_white(chess_state, to)) {
    move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_white(chess_state, to)) {
    move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
  }
  return move_count;
}

size_t sliding_moves_white(const chess_state_t* chess_state, move_t* moves,
                           size_t move_count, sq0x88_t from,
                           const sq0x88_t* increments, int increments_count) {
  for (sq0x88_t i = 0; i < increments_count; i++) {
    sq0x88_t inc = increments[i];
    sq0x88_t to;
    for (to = from + inc; piece(chess_state, to) == EMPTY; to += inc) {
      moves[move_count++] = move(from, to, QUIET_MOVE);
    }
    if (off_the_board(to) || piece_is_white(chess_state, to)) continue;

    moves[move_count++] = move(from, to, CAPTURE);
  }
  return move_count;
}

size_t sliding_moves_black(const chess_state_t* chess_state, move_t* moves,
                           size_t move_count, sq0x88_t from,
                           const sq0x88_t* increments, int increments_count) {
  for (sq0x88_t i = 0; i < increments_count; i++) {
    sq0x88_t inc = increments[i];
    sq0x88_t to;
    for (to = from + inc; piece(chess_state, to) == EMPTY; to += inc) {
      moves[move_count++] = move(from, to, QUIET_MOVE);
    }
    if (off_the_board(to) || piece_is_black(chess_state, to)) continue;

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

size_t sliding_captures_white(const chess_state_t* chess_state, move_t* moves,
                              size_t move_count, sq0x88_t from,
                              const sq0x88_t* increments,
                              int increments_count) {
  for (sq0x88_t i = 0; i < increments_count; i++) {
    sq0x88_t inc = increments[i];
    sq0x88_t to = forwards_ray_cast(chess_state, from, inc);
    if (off_the_board(to) || piece_is_white(chess_state, to)) continue;

    moves[move_count++] = move(from, to, CAPTURE);
  }
  return move_count;
}

size_t generate_moves_nocheck_internal_white(const chess_state_t* chess_state,
                                             move_t* moves) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->white_pieces;
  move_count = castling_moves_white(chess_state, moves, move_count,
                                    piece_lists->king_square);

  move_count = king_moves_white(chess_state, moves, move_count,
                                piece_lists->king_square);

  FOR_EACH_PIECE(piece_lists, queen, square) {
    move_count =
        sliding_moves_white(chess_state, moves, move_count, square,
                            queen_increments_list, QUEEN_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, rook, square) {
    move_count =
        sliding_moves_white(chess_state, moves, move_count, square,
                            rook_increments_list, ROOK_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    move_count =
        sliding_moves_white(chess_state, moves, move_count, square,
                            bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    move_count =
        sliding_moves_white(chess_state, moves, move_count, square,
                            bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, knight, square) {
    move_count = knight_moves_white(chess_state, moves, move_count, square);
  }

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_moves_white(chess_state, moves, move_count, square);
  }

  return move_count;
}

size_t generate_moves_nocheck_internal_black(const chess_state_t* chess_state,
                                             move_t* moves) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->black_pieces;
  move_count = castling_moves_black(chess_state, moves, move_count,
                                    piece_lists->king_square);

  move_count = king_moves_black(chess_state, moves, move_count,
                                piece_lists->king_square);

  FOR_EACH_PIECE(piece_lists, queen, square) {
    move_count =
        sliding_moves_black(chess_state, moves, move_count, square,
                            queen_increments_list, QUEEN_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, rook, square) {
    move_count =
        sliding_moves_black(chess_state, moves, move_count, square,
                            rook_increments_list, ROOK_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    move_count =
        sliding_moves_black(chess_state, moves, move_count, square,
                            bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    move_count =
        sliding_moves_black(chess_state, moves, move_count, square,
                            bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, knight, square) {
    move_count = knight_moves_black(chess_state, moves, move_count, square);
  }

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_moves_black(chess_state, moves, move_count, square);
  }

  return move_count;
}

size_t generate_captures_nocheck_internal_white(
    const chess_state_t* chess_state, move_t* moves) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->white_pieces;

  move_count = king_captures_white(chess_state, moves, move_count,
                                   piece_lists->king_square);

  FOR_EACH_PIECE(piece_lists, queen, square) {
    move_count =
        sliding_captures_white(chess_state, moves, move_count, square,
                               queen_increments_list, QUEEN_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, rook, square) {
    move_count =
        sliding_captures_white(chess_state, moves, move_count, square,
                               rook_increments_list, ROOK_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    move_count =
        sliding_captures_white(chess_state, moves, move_count, square,
                               bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    move_count =
        sliding_captures_white(chess_state, moves, move_count, square,
                               bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, knight, square) {
    move_count = knight_captures_white(chess_state, moves, move_count, square);
  }

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_captures_white(chess_state, moves, move_count, square);
  }

  return move_count;
}

size_t generate_captures_nocheck_internal_black(
    const chess_state_t* chess_state, move_t* moves) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->black_pieces;

  move_count = king_captures_black(chess_state, moves, move_count,
                                   piece_lists->king_square);

  FOR_EACH_PIECE(piece_lists, queen, square) {
    move_count =
        sliding_captures_black(chess_state, moves, move_count, square,
                               queen_increments_list, QUEEN_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, rook, square) {
    move_count =
        sliding_captures_black(chess_state, moves, move_count, square,
                               rook_increments_list, ROOK_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    move_count =
        sliding_captures_black(chess_state, moves, move_count, square,
                               bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    move_count =
        sliding_captures_black(chess_state, moves, move_count, square,
                               bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, knight, square) {
    move_count = knight_captures_black(chess_state, moves, move_count, square);
  }

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_captures_black(chess_state, moves, move_count, square);
  }

  return move_count;
}

size_t generate_quiets_nocheck_internal_white(const chess_state_t* chess_state,
                                              move_t* moves) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->white_pieces;
  move_count = castling_moves_white(chess_state, moves, move_count,
                                    piece_lists->king_square);

  move_count =
      king_quiets(chess_state, moves, move_count, piece_lists->king_square);

  FOR_EACH_PIECE(piece_lists, queen, square) {
    move_count = sliding_quiets(chess_state, moves, move_count, square,
                                queen_increments_list, QUEEN_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, rook, square) {
    move_count = sliding_quiets(chess_state, moves, move_count, square,
                                rook_increments_list, ROOK_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    move_count =
        sliding_quiets(chess_state, moves, move_count, square,
                       bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    move_count =
        sliding_quiets(chess_state, moves, move_count, square,
                       bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, knight, square) {
    move_count = knight_quiets(chess_state, moves, move_count, square);
  }

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_quiets_white(chess_state, moves, move_count, square);
  }

  return move_count;
}

size_t generate_quiets_nocheck_internal_black(const chess_state_t* chess_state,
                                              move_t* moves) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->black_pieces;
  move_count = castling_moves_black(chess_state, moves, move_count,
                                    piece_lists->king_square);

  move_count =
      king_quiets(chess_state, moves, move_count, piece_lists->king_square);

  FOR_EACH_PIECE(piece_lists, queen, square) {
    move_count = sliding_quiets(chess_state, moves, move_count, square,
                                queen_increments_list, QUEEN_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, rook, square) {
    move_count = sliding_quiets(chess_state, moves, move_count, square,
                                rook_increments_list, ROOK_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    move_count =
        sliding_quiets(chess_state, moves, move_count, square,
                       bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    move_count =
        sliding_quiets(chess_state, moves, move_count, square,
                       bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, knight, square) {
    move_count = knight_quiets(chess_state, moves, move_count, square);
  }

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_quiets_black(chess_state, moves, move_count, square);
  }

  return move_count;
}

size_t generate_promotions_nocheck_internal_white(
    const chess_state_t* chess_state, move_t* moves) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->white_pieces;

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_promotions_white(chess_state, moves, move_count, square);
  }

  return move_count;
}

size_t generate_promotions_nocheck_internal_black(
    const chess_state_t* chess_state, move_t* moves) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->black_pieces;

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_promotions_black(chess_state, moves, move_count, square);
  }

  return move_count;
}

int sliding_can_reach(const chess_state_t* chess_state, sq0x88_t from,
                      sq0x88_t target, sq0x88_t inc) {
  return inc && backwards_ray_cast(chess_state, target, inc) == from;
}

int sliding_can_capture(const chess_state_t* chess_state, sq0x88_t from,
                        sq0x88_t target, sq0x88_t inc) {
  return inc && forwards_ray_cast(chess_state, from, inc) == target;
}

size_t generate_captures_of_white(const chess_state_t* chess_state,
                                  move_t* moves, size_t move_count,
                                  sq0x88_t target) {
  const piece_list_t* piece_lists = &chess_state->white_pieces;
  // queen moves
  FOR_EACH_PIECE(piece_lists, queen, square) {
    if (sliding_can_capture(chess_state, square, target,
                            queen_increment(square, target))) {
      moves[move_count++] = move(square, target, CAPTURE);
    }
  }

  // rook moves
  FOR_EACH_PIECE(piece_lists, rook, square) {
    if (sliding_can_capture(chess_state, square, target,
                            rook_increment(square, target))) {
      moves[move_count++] = move(square, target, CAPTURE);
    }
  }

  if (is_light_square(target)) {
    // light bishop moves
    FOR_EACH_PIECE(piece_lists, light_bishop, square) {
      if (sliding_can_capture(chess_state, square, target,
                              bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, CAPTURE);
      }
    }
  } else {
    // dark bishop moves
    FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
      if (sliding_can_capture(chess_state, square, target,
                              bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, CAPTURE);
      }
    }
  }

  // knight moves
  FOR_EACH_PIECE(piece_lists, knight, square) {
    if (knight_increment(square, target)) {
      moves[move_count++] = move(square, target, CAPTURE);
    }
  }
  // pawn captures
  sq0x88_t inc = pawn_push_increment(WHITE);
  sq0x88_t from;
  piece_t friendly_pawn = WHITE_PAWN;
  if (is_promoting_white(target - inc)) {
    return move_count;
  }
  from = target - inc + 1;
  if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
    moves[move_count++] = move(from, target, CAPTURE);
  }

  from = target - inc - 1;
  if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
    moves[move_count++] = move(from, target, CAPTURE);
  }

  if (enpassent_target(chess_state) == target + inc) {
    from = target + 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        can_capture_enpassent_white(chess_state, from)) {
      moves[move_count++] =
          move(from, enpassent_target(chess_state), ENPASSENT);
    }
    from = target - 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        can_capture_enpassent_white(chess_state, from)) {
      moves[move_count++] =
          move(from, enpassent_target(chess_state), ENPASSENT);
    }
  }
  return move_count;
}

size_t generate_captures_of_black(const chess_state_t* chess_state,
                                  move_t* moves, size_t move_count,
                                  sq0x88_t target) {
  const piece_list_t* piece_lists = &chess_state->black_pieces;
  // queen moves
  FOR_EACH_PIECE(piece_lists, queen, square) {
    if (sliding_can_capture(chess_state, square, target,
                            queen_increment(square, target))) {
      moves[move_count++] = move(square, target, CAPTURE);
    }
  }

  // rook moves
  FOR_EACH_PIECE(piece_lists, rook, square) {
    if (sliding_can_capture(chess_state, square, target,
                            rook_increment(square, target))) {
      moves[move_count++] = move(square, target, CAPTURE);
    }
  }

  if (is_light_square(target)) {
    // light bishop moves
    FOR_EACH_PIECE(piece_lists, light_bishop, square) {
      if (sliding_can_capture(chess_state, square, target,
                              bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, CAPTURE);
      }
    }
  } else {
    // dark bishop moves
    FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
      if (sliding_can_capture(chess_state, square, target,
                              bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, CAPTURE);
      }
    }
  }

  // knight moves
  FOR_EACH_PIECE(piece_lists, knight, square) {
    if (knight_increment(square, target)) {
      moves[move_count++] = move(square, target, CAPTURE);
    }
  }
  // pawn captures
  sq0x88_t inc = pawn_push_increment(BLACK);
  sq0x88_t from;
  piece_t friendly_pawn = BLACK_PAWN;
  if (is_promoting_black(target - inc)) {
    return move_count;
  }
  from = target - inc + 1;
  if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
    moves[move_count++] = move(from, target, CAPTURE);
  }

  from = target - inc - 1;
  if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
    moves[move_count++] = move(from, target, CAPTURE);
  }

  if (enpassent_target(chess_state) == target + inc) {
    from = target + 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        can_capture_enpassent_black(chess_state, from)) {
      moves[move_count++] =
          move(from, enpassent_target(chess_state), ENPASSENT);
    }
    from = target - 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        can_capture_enpassent_black(chess_state, from)) {
      moves[move_count++] =
          move(from, enpassent_target(chess_state), ENPASSENT);
    }
  }
  return move_count;
}

size_t generate_promotion_captures_of_white(const chess_state_t* chess_state,
                                            move_t* moves, size_t move_count,
                                            sq0x88_t target) {
  const piece_list_t* piece_lists = &chess_state->white_pieces;
  if (sq8x8_to_rank07(target) != backrank(WHITE)) {
    return move_count;
  }
  sq0x88_t inc = pawn_push_increment(WHITE);
  sq0x88_t from;
  piece_t friendly_pawn = WHITE_PAWN;
  from = target - inc + 1;
  if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
    move_count = add_promotion_moves(moves, move_count, from, target, CAPTURE);
  }
  from = target - inc - 1;
  if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
    move_count = add_promotion_moves(moves, move_count, from, target, CAPTURE);
  }

  return move_count;
}

size_t generate_promotion_captures_of_black(const chess_state_t* chess_state,
                                            move_t* moves, size_t move_count,
                                            sq0x88_t target) {
  const piece_list_t* piece_lists = &chess_state->black_pieces;
  if (sq8x8_to_rank07(target) != backrank(BLACK)) {
    return move_count;
  }
  sq0x88_t inc = pawn_push_increment(BLACK);
  sq0x88_t from;
  piece_t friendly_pawn = BLACK_PAWN;
  from = target - inc + 1;
  if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
    move_count = add_promotion_moves(moves, move_count, from, target, CAPTURE);
  }
  from = target - inc - 1;
  if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
    move_count = add_promotion_moves(moves, move_count, from, target, CAPTURE);
  }

  return move_count;
}

size_t generate_promotion_interposing_moves_white(
    const chess_state_t* chess_state, move_t* moves, size_t move_count,
    sq0x88_t start, sq0x88_t stop, sq0x88_t inc) {
  if (sq0x88_to_rank07(start) != backrank(WHITE) ||
      sq0x88_to_rank07(stop) != backrank(WHITE)) {
    return move_count;
  }
  // pawn moves
  sq0x88_t pawn_inc = pawn_push_increment(WHITE);
  piece_t friendly_pawn = WHITE_PAWN;
  for (sq0x88_t target = start; target != stop; target += inc) {
    sq0x88_t from;
    from = target - pawn_inc;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
      move_count =
          add_promotion_moves(moves, move_count, from, target, QUIET_MOVE);
    }
  }

  return move_count;
}

size_t generate_promotion_interposing_moves_black(
    const chess_state_t* chess_state, move_t* moves, size_t move_count,
    sq0x88_t start, sq0x88_t stop, sq0x88_t inc) {
  if (sq0x88_to_rank07(start) != backrank(BLACK) ||
      sq0x88_to_rank07(stop) != backrank(BLACK)) {
    return move_count;
  }
  // pawn moves
  sq0x88_t pawn_inc = pawn_push_increment(BLACK);
  piece_t friendly_pawn = BLACK_PAWN;
  for (sq0x88_t target = start; target != stop; target += inc) {
    sq0x88_t from;
    from = target - pawn_inc;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
      move_count =
          add_promotion_moves(moves, move_count, from, target, QUIET_MOVE);
    }
  }

  return move_count;
}

size_t generate_interposing_moves_white(const chess_state_t* chess_state,
                                        move_t* moves, size_t move_count,
                                        sq0x88_t start, sq0x88_t stop,
                                        sq0x88_t inc) {
  const piece_list_t* piece_lists = &chess_state->white_pieces;
  // queen moves
  FOR_EACH_PIECE(piece_lists, queen, square) {
    for (sq0x88_t target = start; target != stop; target += inc) {
      if (sliding_can_reach(chess_state, square, target,
                            queen_increment(square, target))) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }

  // rook moves
  FOR_EACH_PIECE(piece_lists, rook, square) {
    for (sq0x88_t target = start; target != stop; target += inc) {
      if (sliding_can_reach(chess_state, square, target,
                            rook_increment(square, target))) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }
  // light bishop moves
  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    for (sq0x88_t target = start; target != stop; target += inc) {
      if (is_light_square(target) &&
          sliding_can_reach(chess_state, square, target,
                            bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }

  // dark bishop moves
  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    for (sq0x88_t target = start; target != stop; target += inc) {
      if (is_dark_square(target) &&
          sliding_can_reach(chess_state, square, target,
                            bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }

  // knight moves
  FOR_EACH_PIECE(piece_lists, knight, square) {
    for (sq0x88_t target = start; target != stop; target += inc) {
      if (knight_increment(square, target)) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }
  if (sq0x88_to_rank07(start) == backrank(WHITE) &&
      sq0x88_to_rank07(stop) == backrank(WHITE)) {
    return move_count;
  }
  // pawn moves
  sq0x88_t pawn_inc = pawn_push_increment(WHITE);
  piece_t friendly_pawn = WHITE_PAWN;
  for (sq0x88_t target = start; target != stop; target += inc) {
    sq0x88_t from;
    from = target - pawn_inc;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
      moves[move_count++] = move(from, target, QUIET_MOVE);
    }
    from = target - 2 * pawn_inc;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        piece(chess_state, target - pawn_inc) == EMPTY &&
        pawn_can_double_push_white(from)) {
      moves[move_count++] = move(from, target, DOUBLE_PAWN_PUSH);
    }
  }

  return move_count;
}

size_t generate_interposing_moves_black(const chess_state_t* chess_state,
                                        move_t* moves, size_t move_count,
                                        sq0x88_t start, sq0x88_t stop,
                                        sq0x88_t inc) {
  const piece_list_t* piece_lists = &chess_state->black_pieces;
  // queen moves
  FOR_EACH_PIECE(piece_lists, queen, square) {
    for (sq0x88_t target = start; target != stop; target += inc) {
      if (sliding_can_reach(chess_state, square, target,
                            queen_increment(square, target))) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }

  // rook moves
  FOR_EACH_PIECE(piece_lists, rook, square) {
    for (sq0x88_t target = start; target != stop; target += inc) {
      if (sliding_can_reach(chess_state, square, target,
                            rook_increment(square, target))) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }
  // light bishop moves
  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    for (sq0x88_t target = start; target != stop; target += inc) {
      if (is_light_square(target) &&
          sliding_can_reach(chess_state, square, target,
                            bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }

  // dark bishop moves
  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    for (sq0x88_t target = start; target != stop; target += inc) {
      if (is_dark_square(target) &&
          sliding_can_reach(chess_state, square, target,
                            bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }

  // knight moves
  FOR_EACH_PIECE(piece_lists, knight, square) {
    for (sq0x88_t target = start; target != stop; target += inc) {
      if (knight_increment(square, target)) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }
  if (sq0x88_to_rank07(start) == backrank(BLACK) &&
      sq0x88_to_rank07(stop) == backrank(BLACK)) {
    return move_count;
  }
  // pawn moves
  sq0x88_t pawn_inc = pawn_push_increment(BLACK);
  piece_t friendly_pawn = BLACK_PAWN;
  for (sq0x88_t target = start; target != stop; target += inc) {
    sq0x88_t from;
    from = target - pawn_inc;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
      moves[move_count++] = move(from, target, QUIET_MOVE);
    }
    from = target - 2 * pawn_inc;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        piece(chess_state, target - pawn_inc) == EMPTY &&
        pawn_can_double_push_black(from)) {
      moves[move_count++] = move(from, target, DOUBLE_PAWN_PUSH);
    }
  }

  return move_count;
}

size_t generate_moves_check_internal_white(const chess_state_t* chess_state,
                                           move_t* moves) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->white_pieces;

  sq0x88_t king_square = piece_lists->king_square;

  move_count = king_moves_white(chess_state, moves, move_count, king_square);

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));

  move_count = generate_captures_of_white(chess_state, moves, move_count,
                                          checking_square(chess_state));
  move_count = generate_promotion_captures_of_white(
      chess_state, moves, move_count, checking_square(chess_state));

  // if checking piece isn't a sliding piece cant be interposed
  if (!(checking_piece & (BISHOP | ROOK | QUEEN))) {
    return move_count;
  }

  sq0x88_t inc = queen_increment(king_square, checking_square(chess_state));
  move_count = generate_interposing_moves_white(
      chess_state, moves, move_count, king_square + inc,
      checking_square(chess_state), inc);
  move_count = generate_promotion_interposing_moves_white(
      chess_state, moves, move_count, king_square + inc,
      checking_square(chess_state), inc);

  return move_count;
}

size_t generate_moves_check_internal_black(const chess_state_t* chess_state,
                                           move_t* moves) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->black_pieces;

  sq0x88_t king_square = piece_lists->king_square;

  move_count = king_moves_black(chess_state, moves, move_count, king_square);

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));

  move_count = generate_captures_of_black(chess_state, moves, move_count,
                                          checking_square(chess_state));
  move_count = generate_promotion_captures_of_black(
      chess_state, moves, move_count, checking_square(chess_state));

  // if checking piece isn't a sliding piece cant be interposed
  if (!(checking_piece & (BISHOP | ROOK | QUEEN))) {
    return move_count;
  }

  sq0x88_t inc = queen_increment(king_square, checking_square(chess_state));
  move_count = generate_interposing_moves_black(
      chess_state, moves, move_count, king_square + inc,
      checking_square(chess_state), inc);
  move_count = generate_promotion_interposing_moves_black(
      chess_state, moves, move_count, king_square + inc,
      checking_square(chess_state), inc);

  return move_count;
}

size_t generate_captures_check_internal_white(const chess_state_t* chess_state,
                                              move_t* moves) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->white_pieces;

  sq0x88_t king_square = piece_lists->king_square;

  move_count = king_captures_white(chess_state, moves, move_count, king_square);

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));

  move_count = generate_captures_of_white(chess_state, moves, move_count,
                                          checking_square(chess_state));

  return move_count;
}

size_t generate_captures_check_internal_black(const chess_state_t* chess_state,
                                              move_t* moves) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->black_pieces;

  sq0x88_t king_square = piece_lists->king_square;

  move_count = king_captures_black(chess_state, moves, move_count, king_square);

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));

  move_count = generate_captures_of_black(chess_state, moves, move_count,
                                          checking_square(chess_state));

  return move_count;
}

size_t generate_quiets_check_internal_white(const chess_state_t* chess_state,
                                            move_t* moves) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->white_pieces;

  sq0x88_t king_square = piece_lists->king_square;

  move_count = king_quiets(chess_state, moves, move_count, king_square);

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));
  // if checking piece isn't a sliding piece cant be interposed
  if (!(checking_piece & (BISHOP | ROOK | QUEEN))) {
    return move_count;
  }

  sq0x88_t inc = queen_increment(king_square, checking_square(chess_state));
  move_count = generate_interposing_moves_white(
      chess_state, moves, move_count, king_square + inc,
      checking_square(chess_state), inc);

  return move_count;
}

size_t generate_quiets_check_internal_black(const chess_state_t* chess_state,
                                            move_t* moves) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->black_pieces;

  sq0x88_t king_square = piece_lists->king_square;

  move_count = king_quiets(chess_state, moves, move_count, king_square);

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));
  // if checking piece isn't a sliding piece cant be interposed
  if (!(checking_piece & (BISHOP | ROOK | QUEEN))) {
    return move_count;
  }

  sq0x88_t inc = queen_increment(king_square, checking_square(chess_state));
  move_count = generate_interposing_moves_black(
      chess_state, moves, move_count, king_square + inc,
      checking_square(chess_state), inc);

  return move_count;
}

size_t generate_promotions_check_internal_white(
    const chess_state_t* chess_state, move_t* moves) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = &chess_state->white_pieces;

  sq0x88_t king_square = piece_lists->king_square;

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));

  move_count = generate_promotion_captures_of_white(
      chess_state, moves, move_count, checking_square(chess_state));

  // if checking piece isn't a sliding piece cant be interposed
  if (!(checking_piece & (BISHOP | ROOK | QUEEN))) {
    return move_count;
  }

  sq0x88_t inc = queen_increment(king_square, checking_square(chess_state));

  move_count = generate_promotion_interposing_moves_white(
      chess_state, moves, move_count, king_square + inc,
      checking_square(chess_state), inc);

  return move_count;
}

size_t generate_moves_white(const chess_state_t* chess_state, move_t* moves) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_moves_check_internal_white(chess_state, moves);
  } else {
    move_count = generate_moves_nocheck_internal_white(chess_state, moves);
  }

  return move_count;
}

size_t generate_moves_black(const chess_state_t* chess_state, move_t* moves) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_moves_check_internal_black(chess_state, moves);
  } else {
    move_count = generate_moves_nocheck_internal_black(chess_state, moves);
  }

  return move_count;
}
size_t generate_captures_white(const chess_state_t* chess_state,
                               move_t* moves) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_captures_check_internal_white(chess_state, moves);
  } else {
    move_count = generate_captures_nocheck_internal_white(chess_state, moves);
  }

  return move_count;
}

size_t generate_captures_black(const chess_state_t* chess_state,
                               move_t* moves) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_captures_check_internal_black(chess_state, moves);
  } else {
    move_count = generate_captures_nocheck_internal_black(chess_state, moves);
  }

  return move_count;
}
size_t generate_quiets_white(const chess_state_t* chess_state, move_t* moves) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_quiets_check_internal_white(chess_state, moves);
  } else {
    move_count = generate_quiets_nocheck_internal_white(chess_state, moves);
  }

  return move_count;
}
size_t generate_quiets_black(const chess_state_t* chess_state, move_t* moves) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_quiets_check_internal_black(chess_state, moves);
  } else {
    move_count = generate_quiets_nocheck_internal_black(chess_state, moves);
  }

  return move_count;
}
size_t generate_promotions_black(const chess_state_t* chess_state,
                                 move_t* moves) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_promotions_check_internal_black(chess_state, moves);
  } else {
    move_count = generate_promotions_nocheck_internal_black(chess_state, moves);
  }

  return move_count;
}

size_t generate_promotions_white(const chess_state_t* chess_state,
                                 move_t* moves) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_promotions_check_internal_white(chess_state, moves);
  } else {
    move_count = generate_promotions_nocheck_internal_white(chess_state, moves);
  }

  return move_count;
}

size_t remove_illegal_moves(const chess_state_t* chess_state, move_t* moves,
                            size_t move_count) {
  for (size_t i = 0; i < move_count; i++) {
    if (!is_legal(chess_state, moves[i])) {
      moves[i--] = moves[--move_count];
    }
  }
  return move_count;
}

size_t generate_legal_moves_white(const chess_state_t* chess_state,
                                  move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_moves_white(chess_state, moves));
}

size_t generate_legal_captures_white(const chess_state_t* chess_state,
                                     move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_captures_white(chess_state, moves));
}

size_t generate_legal_quiets_white(const chess_state_t* chess_state,
                                   move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_quiets_white(chess_state, moves));
}

size_t generate_legal_promotions_white(const chess_state_t* chess_state,
                                       move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_promotions_white(chess_state, moves));
}

size_t generate_legal_moves_black(const chess_state_t* chess_state,
                                  move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_moves_black(chess_state, moves));
}

size_t generate_legal_captures_black(const chess_state_t* chess_state,
                                     move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_captures_black(chess_state, moves));
}

size_t generate_legal_quiets_black(const chess_state_t* chess_state,
                                   move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_quiets_black(chess_state, moves));
}

size_t generate_legal_promotions_black(const chess_state_t* chess_state,
                                       move_t* moves) {
  return remove_illegal_moves(chess_state, moves,
                              generate_promotions_black(chess_state, moves));
}
