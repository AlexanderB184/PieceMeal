
#include <stdlib.h>

#include "../include/chess-lib.h"
#include "../include/private/chess-lib-internals.h"

// todo
// make piece list base move generation functions i.e. size_t
// generate_moves_internal(const chess_state_t* chess_state, const piece_list_t*
// piece_lists, move_t* moves); make incremental move generation functions i.
// size_t generate_captures(const chess_state_t* chess_state, move_t * moves);

#pragma region Helper Functions

colour_t piece_colour(const chess_state_t* chess_state, sq0x88_t square) {
  return (piece(chess_state, square) & COLOUR_MASK);
}

int piece_is_colour(const chess_state_t* chess_state, sq0x88_t square,
                    colour_t colour) {
  return piece_colour(chess_state, square) == colour;
}

colour_t opposite_colour(colour_t colour) {
  switch (colour & COLOUR_MASK) {
    case BLACK:
      return WHITE;
    case WHITE:
      return BLACK;
    default:
      return EMPTY;
  }
}

sq0x88_t pawn_push_increment(colour_t colour) {
  return (colour & WHITE) ? (sq0x88_t)(a2 - a1) : (sq0x88_t)(a1 - a2);
}

const piece_list_t* get_piece_list(const chess_state_t* chess_state,
                                   colour_t colour) {
  return (colour & WHITE) ? &chess_state->white_pieces
                          : &chess_state->black_pieces;
}

int is_promoting(const chess_state_t* chess_state, sq0x88_t from,
                 colour_t colour) {
  return (sq0x88_to_rank07(from) == 1 && colour == BLACK) ||
         (sq0x88_to_rank07(from) == 6 && colour == WHITE);
}

int pawn_can_double_push(const chess_state_t* chess_state, sq0x88_t from,
                         colour_t colour) {
  return (sq0x88_to_rank07(from) == 6 && colour == BLACK) ||
         (sq0x88_to_rank07(from) == 1 && colour == WHITE);
}

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

#pragma endregion

#pragma region Piece Specific Move Generators

#pragma region Knight Moves

// knight movement

size_t knight_moves(const chess_state_t* chess_state, move_t* moves,
                    size_t move_count, sq0x88_t from, colour_t colour) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    move_flags_t flags = (target_piece == EMPTY) ? (QUIET_MOVE) : (CAPTURE);
    moves[move_count++] = move(from, to, flags);
  }
  return move_count;
}

size_t knight_quiets(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t from, colour_t colour) {
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece != EMPTY) continue;
    moves[move_count++] = move(from, to, QUIET_MOVE);
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
    if (target_piece == EMPTY) continue;
    moves[move_count++] = move(from, to, CAPTURE);
  }
  return move_count;
}

#pragma endregion

#pragma region King Moves

size_t king_moves(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t king_square, colour_t colour) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = king_square + king_increments_list[i];
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    move_flags_t flags = (target_piece == EMPTY) ? (QUIET_MOVE) : (CAPTURE);
    moves[move_count++] = move(king_square, to, flags);
  }

  return move_count;
}

size_t king_quiets(const chess_state_t* chess_state, move_t* moves,
                   size_t move_count, sq0x88_t from, colour_t colour) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + king_increments_list[i];
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece != EMPTY) continue;
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }

  return move_count;
}

size_t king_captures(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t from, colour_t colour) {
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + king_increments_list[i];
    if (off_the_board(to) || piece_is_colour(chess_state, to, colour)) {
      continue;
    }
    piece_t target_piece = piece(chess_state, to);
    if (target_piece == EMPTY) continue;
    moves[move_count++] = move(from, to, CAPTURE);
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

#pragma endregion

#pragma region Pawn Moves

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
  if (is_promoting(chess_state, from, colour)) {
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
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_colour(chess_state, to, enemy_colour)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_colour(chess_state, to, enemy_colour)) {
    moves[move_count++] = move(from, to, CAPTURE);
  }
  to = from + 2 * inc;
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY &&
      piece(chess_state, to - inc) == EMPTY &&
      pawn_can_double_push(chess_state, from, colour)) {
    moves[move_count++] = move(from, to, DOUBLE_PAWN_PUSH);
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
  if (is_promoting(chess_state, from, colour)) {
    return move_count;
  }

  if (!off_the_board(to) && piece(chess_state, to) == EMPTY) {
    moves[move_count++] = move(from, to, QUIET_MOVE);
  }
  to = from + 2 * inc;
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY &&
      piece(chess_state, to - inc) == EMPTY &&
      pawn_can_double_push(chess_state, from, colour)) {
    moves[move_count++] = move(from, to, DOUBLE_PAWN_PUSH);
  }
  return move_count;
}

size_t pawn_captures(const chess_state_t* chess_state, move_t* moves,
                     size_t move_count, sq0x88_t from, colour_t colour) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(colour);
  sq0x88_t to = from + inc;
  colour_t enemy_colour = opposite_colour(colour);
  if (is_promoting(chess_state, from, colour)) {
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

size_t pawn_promotions(const chess_state_t* chess_state, move_t* moves,
                       size_t move_count, sq0x88_t from, colour_t colour) {
  // if is promoting
  sq0x88_t inc = pawn_push_increment(colour);
  sq0x88_t to = from + inc;
  if (!is_promoting(chess_state, from, colour)) {
    return move_count;
  }
  if (piece(chess_state, to) == EMPTY) {
    move_count = add_promotion_moves(moves, move_count, from, to, QUIET_MOVE);
  }
  to = from + inc + 1;
  if (!off_the_board(to) && piece_is_enemy(chess_state, to)) {
    move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
  }
  to = from + inc - 1;
  if (!off_the_board(to) && piece_is_enemy(chess_state, to)) {
    move_count = add_promotion_moves(moves, move_count, from, to, CAPTURE);
  }
  return move_count;
}

#pragma endregion

#pragma region Sliding Moves

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
                      size_t move_count, sq0x88_t from, colour_t colour,
                      const sq0x88_t* increments, int increments_count) {
  (void)colour;
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

#pragma endregion

#pragma endregion

size_t generate_moves_internal(const chess_state_t* chess_state,
                                   move_t* moves, colour_t colour) {
  size_t move_count = 0;
  const piece_list_t* piece_lists = get_piece_list(chess_state, colour);
  // king moves
  move_count = king_moves(chess_state, moves, move_count,
                          piece_lists->king_square, colour);

  move_count = castling_moves(chess_state, moves, move_count,
                              piece_lists->king_square, colour);

  // queen moves
  FOR_EACH_PIECE(piece_lists, queen, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square, colour,
                               queen_increments_list, QUEEN_INCREMENTS_COUNT);
  }

  // rook moves
  FOR_EACH_PIECE(piece_lists, rook, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square, colour,
                               rook_increments_list, ROOK_INCREMENTS_COUNT);
  }

  // light bishop moves
  FOR_EACH_PIECE(piece_lists, light_bishop, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square, colour,
                               bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  // dark bishop moves
  FOR_EACH_PIECE(piece_lists, dark_bishop, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square, colour,
                               bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  // knight moves
  FOR_EACH_PIECE(piece_lists, knight, square) {
    move_count = knight_moves(chess_state, moves, move_count, square, colour);
  }

  // pawn moves
  FOR_EACH_PIECE(piece_lists, pawn, square) {
    move_count = pawn_moves(chess_state, moves, move_count, square, colour);
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
  if (is_promoting(chess_state, target - inc, colour)) {
    from = target - inc + 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
      move_count =
          add_promotion_moves(moves, move_count, from, target, CAPTURE);
    }
    from = target - inc - 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
      move_count =
          add_promotion_moves(moves, move_count, from, target, CAPTURE);
    }
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

  // pawn moves
  for (sq0x88_t target = start; target != stop; target += inc) {
    sq0x88_t inc = pawn_push_increment(colour);
    sq0x88_t from;
    piece_t friendly_pawn = PAWN | colour;
    from = target - inc;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
      if (is_promoting(chess_state, from, colour)) {
        move_count =
            add_promotion_moves(moves, move_count, from, target, QUIET_MOVE);
      } else {
        moves[move_count++] = move(from, target, QUIET_MOVE);
      }
    }

    from = target - 2 * inc;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        piece(chess_state, target - inc) == EMPTY &&
        pawn_can_double_push(chess_state, from, colour)) {
      moves[move_count++] = move(from, target, DOUBLE_PAWN_PUSH);
    }
  }

  return move_count;
}

size_t generate_moves_check_internal(const chess_state_t* chess_state, move_t* moves,
                               colour_t colour) {
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

size_t generate_moves(const chess_state_t* chess_state, move_t* moves) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_moves_check_internal(chess_state, moves,
                                         chess_state->friendly_colour);
  } else {
    move_count = generate_moves_internal(chess_state, moves,
                                             chess_state->friendly_colour);
  }

  return move_count;
}
