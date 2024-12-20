
#include <stdlib.h>

#include "../include/chess-lib.h"
#include "../include/private/chess-lib-internals.h"

#pragma region Helper Functions

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

// king helper functions

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

// flags should either be QUIET_MOVE or CAPTURE, add_promotion_moves will handle the promotion flags
size_t add_promotion_moves(move_t* moves, size_t move_count, sq0x88_t from, sq0x88_t to, int flags) {
  moves[move_count++] = move(from, to, flags | QUEEN_PROMOTION);
  moves[move_count++] = move(from, to, flags | ROOK_PROMOTION);
  moves[move_count++] = move(from, to, flags | BISHOP_PROMOTION);
  moves[move_count++] = move(from, to, flags | KNIGHT_PROMOTION);
  return move_count;
}

size_t pawn_moves(const chess_state_t* chess_state, move_t* moves,
                  size_t move_count, sq0x88_t from) {
  // if is promoting
  sq0x88_t inc = chess_state->up_increment;
  sq0x88_t to = from + inc;
  if (is_promoting(chess_state, from)) {
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

size_t generate_psuedo_legal_moves(const chess_state_t* chess_state,
                                   move_t* moves) {
  size_t move_count = 0;

  // king moves
  move_count = king_moves(chess_state, moves, move_count,
                          chess_state->friendly_pieces->king_square);

  move_count = castling_moves(chess_state, moves, move_count,
                              chess_state->friendly_pieces->king_square);

  // queen moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, queen, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square,
                               queen_increments_list, QUEEN_INCREMENTS_COUNT);
  }

  // rook moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, rook, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square,
                               rook_increments_list, ROOK_INCREMENTS_COUNT);
  }

  // light bishop moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, light_bishop, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square,
                               bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  // dark bishop moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, dark_bishop, square) {
    move_count = sliding_moves(chess_state, moves, move_count, square,
                               bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  // knight moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, knight, square) {
    move_count = knight_moves(chess_state, moves, move_count, square);
  }

  // pawn moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, pawn, square) {
    move_count = pawn_moves(chess_state, moves, move_count, square);
  }

  return move_count;
}

int sliding_can_reach(const chess_state_t* chess_state, sq0x88_t from,
                      sq0x88_t target, sq0x88_t inc) {
  return inc && backwards_ray_cast(chess_state, target, inc) == from;
}

size_t generate_captures_of(const chess_state_t* chess_state, move_t* moves,
                            size_t move_count, sq0x88_t target) {
  // queen moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, queen, square) {
    if (sliding_can_reach(chess_state, square, target,
                          queen_increment(square, target))) {
      moves[move_count++] = move(square, target, CAPTURE);
    }
  }

  // rook moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, rook, square) {
    if (sliding_can_reach(chess_state, square, target,
                          rook_increment(square, target))) {
      moves[move_count++] = move(square, target, CAPTURE);
    }
  }

  if (is_light_square(target)) {
    // light bishop moves
    FOR_EACH_PIECE(chess_state->friendly_pieces, light_bishop, square) {
      if (sliding_can_reach(chess_state, square, target,
                            bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, CAPTURE);
      }
    }
  } else {
    // dark bishop moves
    FOR_EACH_PIECE(chess_state->friendly_pieces, dark_bishop, square) {
      if (sliding_can_reach(chess_state, square, target,
                            bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, CAPTURE);
      }
    }
  }

  // knight moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, knight, square) {
    if (knight_increment(square, target)) {
      moves[move_count++] = move(square, target, CAPTURE);
    }
  }

  // pawn captures
  sq0x88_t inc = chess_state->up_increment;
  sq0x88_t from;
  piece_t friendly_pawn = chess_state->friendly_colour | PAWN;
  if (is_promoting(chess_state, target - inc)) {
    from = target - inc + 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
      move_count = add_promotion_moves(moves, move_count, from ,target, CAPTURE);
    }
    from = target - inc - 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
      move_count = add_promotion_moves(moves, move_count, from ,target, CAPTURE);
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

  if (enpassent_target(chess_state) == target + chess_state->up_increment) {
    from = target + 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        can_capture_enpassent(chess_state, from)) {
      moves[move_count++] =
          move(from, enpassent_target(chess_state), ENPASSENT);
    }
    from = target - 1;
    if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
        can_capture_enpassent(chess_state, from)) {
      moves[move_count++] =
          move(from, enpassent_target(chess_state), ENPASSENT);
    }
  }

  return move_count;
}

size_t generate_interposing_moves(const chess_state_t* chess_state,
                                  move_t* moves, size_t move_count,
                                  sq0x88_t target) {
  // queen moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, queen, square) {
    if (sliding_can_reach(chess_state, square, target,
                          queen_increment(square, target))) {
      moves[move_count++] = move(square, target, QUIET_MOVE);
    }
  }

  // rook moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, rook, square) {
    if (sliding_can_reach(chess_state, square, target,
                          rook_increment(square, target))) {
      moves[move_count++] = move(square, target, QUIET_MOVE);
    }
  }

  if (is_light_square(target)) {
    // light bishop moves
    FOR_EACH_PIECE(chess_state->friendly_pieces, light_bishop, square) {
      if (sliding_can_reach(chess_state, square, target,
                            bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  } else {
    // dark bishop moves
    FOR_EACH_PIECE(chess_state->friendly_pieces, dark_bishop, square) {
      if (sliding_can_reach(chess_state, square, target,
                            bishop_increment(square, target))) {
        moves[move_count++] = move(square, target, QUIET_MOVE);
      }
    }
  }

  // knight moves
  FOR_EACH_PIECE(chess_state->friendly_pieces, knight, square) {
    if (knight_increment(square, target)) {
      moves[move_count++] = move(square, target, QUIET_MOVE);
    }
  }

  // pawn moves
  sq0x88_t inc = chess_state->up_increment;
  sq0x88_t from;
  piece_t friendly_pawn = PAWN | chess_state->friendly_colour;
  from = target - inc;
  if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn) {
    if (is_promoting(chess_state, from)) {
      move_count = add_promotion_moves(moves, move_count, from, target, QUIET_MOVE);
    } else {
      moves[move_count++] = move(from, target, QUIET_MOVE);
    }
  }

  from = target - 2 * inc;
  if (!off_the_board(from) && piece(chess_state, from) == friendly_pawn &&
      piece(chess_state, target - inc) == EMPTY &&
      pawn_can_double_push(chess_state, from)) {
    moves[move_count++] = move(from, target, DOUBLE_PAWN_PUSH);
  }

  return move_count;
}

size_t generate_check_evasions(const chess_state_t* chess_state,
                               move_t* moves) {
  // if more than 1 attacker, only king moves
  // if only 1 attacker, capture of attacker, block of attacker, king moves
  size_t move_count = 0;

  sq0x88_t king_square = chess_state->friendly_pieces->king_square;

  move_count = king_moves(chess_state, moves, move_count, king_square);

  if (is_double_check(chess_state)) {
    return move_count;
  }

  piece_t checking_piece = piece(chess_state, checking_square(chess_state));

  move_count = generate_captures_of(chess_state, moves, move_count,
                                    checking_square(chess_state));

  // if checking piece isn't a sliding piece cant be interposed
  if (!(checking_piece & (BISHOP | ROOK | QUEEN))) {
    return move_count;
  }

  sq0x88_t inc = queen_increment(king_square, checking_square(chess_state));
  // move along interpose line and find all interposing moves
  for (sq0x88_t interposing_square = king_square + inc;
       interposing_square != checking_square(chess_state);
       interposing_square += inc) {
    move_count = generate_interposing_moves(chess_state, moves, move_count,
                                            interposing_square);
  }

  return move_count;
}

size_t generate_legal_moves(const chess_state_t* chess_state, move_t* moves) {
  size_t move_count = generate_moves(chess_state, moves);

  for (size_t i = 0; i < move_count; i++) {
    if (!is_legal(chess_state, moves[i])) {
      moves[i--] = moves[--move_count];
    }
  }

  return move_count;
}

size_t generate_moves(const chess_state_t* chess_state, move_t* moves) {
  size_t move_count;

  if (is_check(chess_state)) {
    move_count = generate_check_evasions(chess_state, moves);
  } else {
    move_count = generate_psuedo_legal_moves(chess_state, moves);
  }

  return move_count;
}
/**
size_t psuedo_legal_knight_move_count(const chess_state_t* chess_state,

                                      sq0x88_t from) {
  colour_t colour = piece(chess_state, from) & COLOUR_MASK;
  size_t move_count = 0;
  for (int i = 0; i < KNIGHT_INCREMENTS_COUNT; i++) {
    sq0x88_t to = from + knight_increments_list[i];
    if (off_the_board(to)) {
      continue;
    }
    if (piece(chess_state, to) & colour) {
      continue;
    }
    move_count++;
  }
  return move_count;
}

size_t psuedo_legal_king_move_count(const chess_state_t* chess_state,
                                    sq0x88_t king_square) {
  colour_t colour = piece(chess_state, king_square) & COLOUR_MASK;
  size_t move_count = 0;
  for (int i = 0; i < KING_INCREMENTS_COUNT; i++) {
    sq0x88_t to = king_square + king_increments_list[i];
    if (off_the_board(to)) {
      continue;
    }
    if (piece(chess_state, to) & colour) {
      continue;
    }
    move_count++;
  }

  return move_count;
}

size_t castling_move_count(const chess_state_t* chess_state,
                           sq0x88_t king_square) {
  colour_t colour = piece(chess_state, king_square) & COLOUR_MASK;
  size_t move_count = 0;
  // castling
  if ((colour == WHITE && (chess_state->castle_rights & WHITE_KING_SIDE)) ||
      (colour == BLACK && (chess_state->castle_rights & BLACK_KING_SIDE))) {
    if (piece(chess_state, king_square + 1) == EMPTY &&
        piece(chess_state, king_square + 2) == EMPTY) {
      move_count++;
    }
  }
  if ((colour == WHITE && (chess_state->castle_rights & WHITE_QUEEN_SIDE)) ||
      (colour == BLACK && (chess_state->castle_rights & BLACK_QUEEN_SIDE))) {
    if (piece(chess_state, king_square - 1) == EMPTY &&
        piece(chess_state, king_square - 2) == EMPTY &&
        piece(chess_state, king_square - 3) == EMPTY) {
      move_count++;
    }
  }
  return move_count;
}

#include <stdio.h>
size_t psuedo_legal_pawn_move_count(const chess_state_t* chess_state,
                                    sq0x88_t from) {
  size_t move_count = 0;
  // if is promoting
  sq0x88_t inc = chess_state->up_increment;
  sq0x88_t to = from + inc;
  if (is_promoting(chess_state, from)) {
    if (piece(chess_state, to) == EMPTY) {
      move_count += 4;
    }
    to = from + inc + 1;
    if (!off_the_board(to) &&
        (piece(chess_state, from) & COLOUR_MASK) ==
            ((piece(chess_state, to) & COLOUR_MASK) ^ COLOUR_MASK)) {
      move_count += 4;
    }
    to = from + inc - 1;
    if (!off_the_board(to) &&
        (piece(chess_state, from) & COLOUR_MASK) ==
            ((piece(chess_state, to) & COLOUR_MASK) ^ COLOUR_MASK)) {
      move_count += 4;
    }
    return move_count;
  }

  if (!off_the_board(to) && piece(chess_state, to) == EMPTY) {
    move_count++;
  }
  to = from + inc + 1;
  if (!off_the_board(to) &&
      (piece(chess_state, from) & COLOUR_MASK) ==
          ((piece(chess_state, to) & COLOUR_MASK) ^ COLOUR_MASK)) {
    move_count++;
  }
  to = from + inc - 1;
  if (!off_the_board(to) &&
      (piece(chess_state, from) & COLOUR_MASK) ==
          ((piece(chess_state, to) & COLOUR_MASK) ^ COLOUR_MASK)) {
    move_count++;
  }
  to = from + 2 * inc;
  if (!off_the_board(to) && piece(chess_state, to) == EMPTY &&
      piece(chess_state, to - inc) == EMPTY &&
      pawn_can_double_push(chess_state, from)) {
    move_count++;
  }

  if (!off_the_board(enpassent_target(chess_state)) &&
      can_capture_enpassent(chess_state, from)) {
    move_count++;
  }

  return move_count;
}

size_t psuedo_legal_sliding_move_count(const chess_state_t* chess_state,
                                       sq0x88_t from,
                                       const sq0x88_t* increments,
                                       int increments_count) {
  size_t move_count = 0;
  for (sq0x88_t i = 0; i < increments_count; i++) {
    sq0x88_t inc = increments[i];
    sq0x88_t to;
    for (to = from + inc; !off_the_board(to) && piece(chess_state, to) == EMPTY;
         to += inc) {
      move_count++;
    }
    if ((off_the_board(to)) || ((piece(chess_state, from) & COLOUR_MASK) ==
                                (piece(chess_state, to) & COLOUR_MASK)))
      continue;

    move_count++;
  }
  return move_count;
}

size_t psuedo_legal_move_count(const chess_state_t* chess_state,
                               const piece_list_t* piece_list) {
  size_t move_count = 0;

  // king moves
  move_count +=
      psuedo_legal_king_move_count(chess_state, piece_list->king_square);

  move_count += castling_move_count(chess_state, piece_list->king_square);

  // queen moves
  for (int queen_index = 0; queen_index < piece_list->queen_count;
       queen_index++) {
    move_count += psuedo_legal_sliding_move_count(
        chess_state, piece_list->queen_list[queen_index], queen_increments_list,
        QUEEN_INCREMENTS_COUNT);
  }
  // rook moves
  for (int rook_index = 0; rook_index < piece_list->rook_count; rook_index++) {
    move_count += psuedo_legal_sliding_move_count(
        chess_state, piece_list->rook_list[rook_index], rook_increments_list,
        ROOK_INCREMENTS_COUNT);
  }

  // light bishop moves
  for (int light_bishop_index = 0;
       light_bishop_index < piece_list->light_bishop_count;
       light_bishop_index++) {
    move_count += psuedo_legal_sliding_move_count(
        chess_state, piece_list->light_bishop_list[light_bishop_index],
        bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  // dark bishop moves
  for (int dark_bishop_index = 0;
       dark_bishop_index < piece_list->dark_bishop_count; dark_bishop_index++) {
    move_count += psuedo_legal_sliding_move_count(
        chess_state, piece_list->dark_bishop_list[dark_bishop_index],
        bishop_increments_list, BISHOP_INCREMENTS_COUNT);
  }

  // knight moves
  for (int knight_index = 0;
       knight_index < chess_state->friendly_pieces->knight_count;
       knight_index++) {
    sq0x88_t from = piece_list->knight_list[knight_index];
    move_count += psuedo_legal_knight_move_count(chess_state, from);
  }

  // pawn moves
  for (int pawn_index = 0; pawn_index < piece_list->pawn_count; pawn_index++) {
    sq0x88_t from = piece_list->pawn_list[pawn_index];
    move_count += psuedo_legal_pawn_move_count(chess_state, from);
  }

  return move_count;
}
 */