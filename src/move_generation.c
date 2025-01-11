
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

int can_capture_enpassent(const chess_state_t* chess_state, sq0x88_t from,
                          colour_t colour) {
  sq0x88_t inc = pawn_push_increment(colour);
  sq0x88_t target = enpassent_target(chess_state);
  return colour == chess_state->friendly_colour &&
         sq0x88_to_rank07(from + inc) == sq0x88_to_rank07(target) &&
         abs(sq0x88_to_file07(from) - sq0x88_to_file07(target)) == 1;
}

// king helper functions

int can_castle_king_side(const chess_state_t* chess_state, colour_t colour) {
  sq0x88_t king_square = get_piece_list(chess_state, colour)->king_square;
  return ((colour == WHITE &&
           (chess_state->castle_rights & WHITE_KING_SIDE) != 0) ||
          (colour == BLACK &&
           (chess_state->castle_rights & BLACK_KING_SIDE) != 0)) &&
         piece(chess_state, king_square + 1) == EMPTY &&
         piece(chess_state, king_square + 2) == EMPTY;
}

int can_castle_queen_side(const chess_state_t* chess_state, colour_t colour) {
  sq0x88_t king_square = get_piece_list(chess_state, colour)->king_square;
  return ((colour == WHITE &&
           (chess_state->castle_rights & WHITE_QUEEN_SIDE) != 0) ||
          (colour == BLACK &&
           (chess_state->castle_rights & BLACK_QUEEN_SIDE) != 0)) &&
         piece(chess_state, king_square - 1) == EMPTY &&
         piece(chess_state, king_square - 2) == EMPTY &&
         piece(chess_state, king_square - 3) == EMPTY;
}

// knight movement

size_t knight_moves(const chess_state_t* chess_state, move_t* moves,
                    size_t move_count, sq0x88_t from, colour_t colour) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) {
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

size_t knight_captures(const chess_state_t* chess_state, move_t* moves,
                       size_t move_count, sq0x88_t from, colour_t colour) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) {
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

size_t king_moves(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t king_square, colour_t colour) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = king_square + king_increments_list[i];
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) {
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
                     size_t move_count, sq0x88_t king_square, colour_t colour) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = king_square + king_increments_list[i];
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) {
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

size_t castling_moves(const chess_state_t* chess_state, move_t* moves,
                      size_t move_count, sq0x88_t king_square,
                      colour_t colour) {
  // castling
  if (can_castle_king_side(chess_state, colour)) {
    moves[move_count++] = move(king_square, king_square + 2, KING_CASTLE);
  }
  if (can_castle_queen_side(chess_state, colour)) {
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

size_t pawn_moves(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t from, colour_t colour) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(colour);
  colour_t enemy_colour = opposite_colour(colour);
  sq0x88_t to = from + inc;
  if (is_promoting(from, colour)) {
    if (piece(chess_state, to) == EMPTY) {
      move_count = add_promotion_moves(moves, move_count, from, to, QUIET_MOVE);
    }
    to = from + inc + 1;
    if (!off_the_board(to) && piece_is_colour(chess_state, to, enemy_colour)) {
      move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
    }
    to = from + inc - 1;
    if (!off_the_board(to) && piece_is_colour(chess_state, to, enemy_colour)) {
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
      pawn_can_double_push(from, colour)) {
    moves[move_count++] = move(from, to, DOUBLE_PAWN_PUSH);
  }

  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_colour(chess_state, to, enemy_colour)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_colour(chess_state, to, enemy_colour)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }

  if (!off_the_board(enpassent_target(chess_state)) &&
      can_capture_enpassent(chess_state, from, colour)) {
    moves[move_count++] = move(from, enpassent_target(chess_state), ENPASSENT);
  }

  return move_count;
}

size_t pawn_captures(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t from, colour_t colour) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(colour);
  colour_t enemy_colour = opposite_colour(colour);
  sq0x88_t to = from + inc;
  if (is_promoting(from, colour)) {
    return move_count;
  }
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_colour(chess_state, to, enemy_colour)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_colour(chess_state, to, enemy_colour)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }

  if (!off_the_board(enpassent_target(chess_state)) &&
      can_capture_enpassent(chess_state, from, colour)) {
    moves[move_count++] = move(from, enpassent_target(chess_state), ENPASSENT);
  }

  return move_count;
}

size_t pawn_quiets(const chess_state_t* chess_state, move_t* moves,
                   size_t move_count, sq0x88_t from, colour_t colour) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(colour);
  sq0x88_t to = from + inc;
  if (is_promoting(from, colour)) {
    return move_count;
  }
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY) {
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }
  to = from + 2 * inc;
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY &&
      piece(chess_state, to - inc) == EMPTY &&
      pawn_can_double_push(from, colour)) {
    moves[move_count++] = move(from, to, DOUBLE_PAWN_PUSH);
  }

  return move_count;
}

size_t pawn_promotions(const chess_state_t* chess_state, move_t* moves,
                       size_t move_count, sq0x88_t from, colour_t colour) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(colour);
  colour_t enemy_colour = opposite_colour(colour);
  sq0x88_t to = from + inc;
  if (!is_promoting(from, colour)) {
    return move_count;
  }
  if (piece(chess_state, to) == EMPTY) {
    move_count = add_promotion_moves(moves, move_count, from, to, QUIET_MOVE);
  }
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_colour(chess_state, to, enemy_colour)) {
    move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_colour(chess_state, to, enemy_colour)) {
    move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
  }
  return move_count;
}

size_t sliding_moves(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t from, colour_t colour,
                     const sq0x88_t* increments, int increments_count) {
  for (sq0x88_t i = 0; i < increments_count; i++) {
    sq0x88_t inc = increments[i];
    sq0x88_t to;
    for (to = from + inc; piece(chess_state, to) == EMPTY; to += inc) {
      moves[move_count++] = move(from, to, QUIET_MOVE);
    }
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) continue;

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
                        size_t move_count, sq0x88_t from, colour_t colour,
                        const sq0x88_t* increments, int increments_count) {
  for (sq0x88_t i = 0; i < increments_count; i++) {
    sq0x88_t inc = increments[i];
    sq0x88_t to = forwards_ray_cast(chess_state, from, inc);
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) continue;

    moves[move_count++] = move(from, to, CAPTURE);
  }
  return move_count;
}

#define FOR_ALL_PIECES(MOVE_TYPE)                                              \
  do {                                                                         \
    move_count = king_##MOVE_TYPE(chess_state, moves, move_count,              \
                                  piece_lists->king_square, colour);           \
                                                                               \
    FOR_EACH_PIECE(piece_lists, queen, square) {                               \
      move_count =                                                             \
          sliding_##MOVE_TYPE(chess_state, moves, move_count, square, colour,  \
                              queen_increments_list, QUEEN_INCREMENTS_COUNT);  \
    }                                                                          \
                                                                               \
    FOR_EACH_PIECE(piece_lists, rook, square) {                                \
      move_count =                                                             \
          sliding_##MOVE_TYPE(chess_state, moves, move_count, square, colour,  \
                              rook_increments_list, ROOK_INCREMENTS_COUNT);    \
    }                                                                          \
                                                                               \
    FOR_EACH_PIECE(piece_lists, light_bishop, square) {                        \
      move_count = sliding_##MOVE_TYPE(chess_state, moves, move_count, square, \
                                       colour, bishop_increments_list,         \
                                       BISHOP_INCREMENTS_COUNT);               \
    }                                                                          \
                                                                               \
    FOR_EACH_PIECE(piece_lists, dark_bishop, square) {                         \
      move_count = sliding_##MOVE_TYPE(chess_state, moves, move_count, square, \
                                       colour, bishop_increments_list,         \
                                       BISHOP_INCREMENTS_COUNT);               \
    }                                                                          \
                                                                               \
    FOR_EACH_PIECE(piece_lists, knight, square) {                              \
      move_count =                                                             \
          knight_##MOVE_TYPE(chess_state, moves, move_count, square, colour);  \
    }                                                                          \
                                                                               \
    FOR_EACH_PIECE(piece_lists, pawn, square) {                                \
      move_count =                                                             \
          pawn_##MOVE_TYPE(chess_state, moves, move_count, square, colour);    \
    }                                                                          \
  } while (0)

size_t generate_moves_nocheck_internal(const chess_state_t* chess_state,
                                       move_t* moves, colour_t colour) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);
  move_count = castling_moves(chess_state, moves, move_count,
                              piece_lists->king_square, colour);

  move_count = king_moves(chess_state, moves, move_count,
                          piece_lists->king_square, colour);

  FOR_EACH_PIECE(piece_lists, queen, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square, colour,
                               queen_increments_list, QUEEN_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, rook, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square, colour,
                               rook_increments_list, ROOK_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square, colour,
                               bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square, colour,
                               bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, knight, square) {
    move_count = knight_moves(chess_state, moves, move_count, square, colour);
  }

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_moves(chess_state, moves, move_count, square, colour);
  }

  return move_count;
}

size_t generate_captures_nocheck_internal(const chess_state_t* chess_state,
                                          move_t* moves, colour_t colour) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);

  move_count = king_captures(chess_state, moves, move_count,
                             piece_lists->king_square, colour);

  FOR_EACH_PIECE(piece_lists, queen, square) {
    move_count =
        sliding_captures(chess_state, moves, move_count, square, colour,
                         queen_increments_list, QUEEN_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, rook, square) {
    move_count =
        sliding_captures(chess_state, moves, move_count, square, colour,
                         rook_increments_list, ROOK_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    move_count =
        sliding_captures(chess_state, moves, move_count, square, colour,
                         bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    move_count =
        sliding_captures(chess_state, moves, move_count, square, colour,
                         bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  FOR_EACH_PIECE(piece_lists, knight, square) {
    move_count =
        knight_captures(chess_state, moves, move_count, square, colour);
  }

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_captures(chess_state, moves, move_count, square, colour);
  }

  return move_count;
}

size_t generate_quiets_nocheck_internal(const chess_state_t* chess_state,
                                        move_t* moves, colour_t colour) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);
  move_count = castling_moves(chess_state, moves, move_count,
                              piece_lists->king_square, colour);

  move_count = king_quiets(chess_state, moves, move_count,
                           piece_lists->king_square);

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
    move_count = pawn_quiets(chess_state, moves, move_count, square, colour);
  }

  return move_count;
}

size_t generate_promotions_nocheck_internal(const chess_state_t* chess_state,
                                            move_t* moves, colour_t colour) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);

  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count =
        pawn_promotions(chess_state, moves, move_count, square, colour);
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

size_t generate_captures_of(const chess_state_t* chess_state, move_t* moves,
                            size_t move_count, colour_t colour,
                            sq0x88_t target) {
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);
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
  sq0x88_t inc = pawn_push_increment(colour);
  sq0x88_t from;
  piece_t friendly_pawn = colour | PAWN;
  if (is_promoting(target - inc, colour)) {
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
        can_capture_enpassent(chess_state, from, colour)) {
      moves[move_count++] =
          move(from, enpassent_target(chess_state), ENPASSENT);
    }
    from = target - 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        can_capture_enpassent(chess_state, from, colour)) {
      moves[move_count++] =
          move(from, enpassent_target(chess_state), ENPASSENT);
    }
  }
  return move_count;
}

size_t generate_promotion_captures_of(const chess_state_t* chess_state,
                                      move_t* moves, size_t move_count,
                                      colour_t colour, sq0x88_t target) {
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);
  if (sq8x8_to_rank07(target) != backrank(colour)) {
    return move_count;
  }
  sq0x88_t inc = pawn_push_increment(colour);
  sq0x88_t from;
  piece_t friendly_pawn = colour | PAWN;
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

size_t generate_promotion_interposing_moves(const chess_state_t* chess_state,
                                            move_t* moves, size_t move_count,
                                            colour_t colour, sq0x88_t start,
                                            sq0x88_t stop, sq0x88_t inc) {
  if (sq0x88_to_rank07(start) != backrank(colour) ||
      sq0x88_to_rank07(stop) != backrank(colour)) {
    return move_count;
  }
  // pawn moves
  sq0x88_t pawn_inc = pawn_push_increment(colour);
  piece_t friendly_pawn = PAWN | colour;
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

size_t generate_interposing_moves(const chess_state_t* chess_state,
                                  move_t* moves, size_t move_count,
                                  colour_t colour, sq0x88_t start,
                                  sq0x88_t stop, sq0x88_t inc) {
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);
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
  if (sq0x88_to_rank07(start) == backrank(colour) &&
      sq0x88_to_rank07(stop) == backrank(colour)) {
    return move_count;
  }
  // pawn moves
  sq0x88_t pawn_inc = pawn_push_increment(colour);
  piece_t friendly_pawn = PAWN | colour;
  for (sq0x88_t target = start; target != stop; target += inc) {
    sq0x88_t from;
    from = target - pawn_inc;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
      moves[move_count++] = move(from, target, QUIET_MOVE);
    }
    from = target - 2 * pawn_inc;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        piece(chess_state, target - pawn_inc) == EMPTY &&
        pawn_can_double_push(from, colour)) {
      moves[move_count++] = move(from, target, DOUBLE_PAWN_PUSH);
    }
  }

  return move_count;
}

size_t generate_moves_check_internal(const chess_state_t* chess_state,
                                     move_t* moves, colour_t colour) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);

  sq0x88_t king_square = piece_lists->king_square;

  move_count = king_moves(chess_state, moves, move_count, king_square, colour);

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));

  move_count = generate_captures_of(chess_state, moves, move_count, colour,
                                    checking_square(chess_state));
  move_count = generate_promotion_captures_of(
      chess_state, moves, move_count, colour, checking_square(chess_state));

  // if checking piece isn't a sliding piece cant be interposed
  if (!(checking_piece & (BISHOP | ROOK | QUEEN))) {
    return move_count;
  }

  sq0x88_t inc = queen_increment(king_square, checking_square(chess_state));
  move_count = generate_interposing_moves(chess_state, moves, move_count,
                                          colour, king_square + inc,
                                          checking_square(chess_state), inc);
  move_count = generate_promotion_interposing_moves(
      chess_state, moves, move_count, colour, king_square + inc,
      checking_square(chess_state), inc);

  return move_count;
}

size_t generate_captures_check_internal(const chess_state_t* chess_state,
                                        move_t* moves, colour_t colour) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);

  sq0x88_t king_square = piece_lists->king_square;

  move_count =
      king_captures(chess_state, moves, move_count, king_square, colour);

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));

  move_count = generate_captures_of(chess_state, moves, move_count, colour,
                                    checking_square(chess_state));

  return move_count;
}

size_t generate_quiets_check_internal(const chess_state_t* chess_state,
                                      move_t* moves, colour_t colour) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);

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
  move_count = generate_interposing_moves(chess_state, moves, move_count,
                                          colour, king_square + inc,
                                          checking_square(chess_state), inc);

  return move_count;
}

size_t generate_promotions_check_internal(const chess_state_t* chess_state,
                                          move_t* moves, colour_t colour) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);

  sq0x88_t king_square = piece_lists->king_square;

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));

  move_count = generate_promotion_captures_of(
      chess_state, moves, move_count, colour, checking_square(chess_state));

  // if checking piece isn't a sliding piece cant be interposed
  if (!(checking_piece & (BISHOP | ROOK | QUEEN))) {
    return move_count;
  }

  sq0x88_t inc = queen_increment(king_square, checking_square(chess_state));

  move_count = generate_promotion_interposing_moves(
      chess_state, moves, move_count, colour, king_square + inc,
      checking_square(chess_state), inc);

  return move_count;
}

size_t generate_moves(const chess_state_t* chess_state, move_t* moves,
                      colour_t colour) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_moves_check_internal(chess_state, moves, colour);
  } else {
    move_count = generate_moves_nocheck_internal(chess_state, moves, colour);
  }

  return move_count;
}

size_t generate_captures(const chess_state_t* chess_state, move_t* moves,
                         colour_t colour) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_captures_check_internal(chess_state, moves, colour);
  } else {
    move_count = generate_captures_nocheck_internal(chess_state, moves, colour);
  }

  return move_count;
}
size_t generate_quiets(const chess_state_t* chess_state, move_t* moves,
                       colour_t colour) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_quiets_check_internal(chess_state, moves, colour);
  } else {
    move_count = generate_quiets_nocheck_internal(chess_state, moves, colour);
  }

  return move_count;
}
size_t generate_promotions(const chess_state_t* chess_state, move_t* moves,
                           colour_t colour) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_promotions_check_internal(chess_state, moves, colour);
  } else {
    move_count =
        generate_promotions_nocheck_internal(chess_state, moves, colour);
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

size_t generate_legal_moves(const chess_state_t* chess_state, move_t* moves,
                            colour_t colour) {
  return remove_illegal_moves(chess_state, moves,
                              generate_moves(chess_state, moves, colour));
}

size_t generate_legal_captures(const chess_state_t* chess_state, move_t* moves,
                               colour_t colour) {
  return remove_illegal_moves(chess_state, moves,
                              generate_captures(chess_state, moves, colour));
}

size_t generate_legal_quiets(const chess_state_t* chess_state, move_t* moves,
                             colour_t colour) {
  return remove_illegal_moves(chess_state, moves,
                              generate_quiets(chess_state, moves, colour));
}

size_t generate_legal_promotions(const chess_state_t* chess_state,
                                 move_t* moves, colour_t colour) {
  return remove_illegal_moves(chess_state, moves,
                              generate_promotions(chess_state, moves, colour));
}
