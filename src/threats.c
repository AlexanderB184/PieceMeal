
#include <stdio.h>

#include "../include/chess.h"

int discover_check_after_move(const chess_state_t* chess_state,
                              sq0x88_t king_square,
                              sq0x88_t revealing_piece_from,
                              sq0x88_t revealing_piece_to) {
  sq0x88_t inc = queen_increment(revealing_piece_from, king_square);
  if (!inc) return 0;
  // check if piece is still in the same "line" with the King as before
  if (inc == queen_increment(revealing_piece_to, king_square)) return 0;

  // if another piece is between this piece and the king it is not a reveal
  if (forwards_ray_cast(chess_state, revealing_piece_from, inc) != king_square)
    return 0;

  // get the piece that is revealed (if one exists)
  piece_t revealed_square =
      backwards_ray_cast(chess_state, revealing_piece_from, inc);

  if (off_the_board(revealed_square) ||
      piece_is_enemy(chess_state, revealed_square))
    return 0;

  piece_t revealed_piece = piece(chess_state, revealed_square);
  // check piece can move to the king square
  if (!((revealed_piece & BISHOP) &&
        bishop_increment(revealed_square, king_square)) &&
      !((revealed_piece & ROOK) &&
        rook_increment(revealed_square, king_square))) {
    return 0;
  }
  return 1;
}

int discover_enpassent_check_after_move(const chess_state_t* chess_state,
                                        sq0x88_t king_square,
                                        sq0x88_t pawn_move_from,
                                        sq0x88_t pawn_move_to,
                                        sq0x88_t captured_pawn) {
  sq0x88_t inc = queen_increment(captured_pawn, king_square);
  if (!inc) return 0;
  // check if piece is still in the same "line" with the King as before
  if (inc == queen_increment(pawn_move_to, king_square)) return 0;

  // if another piece is between this piece and the king it is not a reveal
  piece_t square = forwards_ray_cast(chess_state, captured_pawn, inc);
  if (square == pawn_move_from) {
    square = forwards_ray_cast(chess_state, square, inc);
  }
  if (square != king_square)
    return 0;

  // get the piece that is revealed (if one exists)
  piece_t revealed_square = backwards_ray_cast(chess_state, captured_pawn, inc);

  if (revealed_square == pawn_move_from) {
    revealed_square = backwards_ray_cast(chess_state, revealed_square, inc);
  }

  if (off_the_board(revealed_square) ||
      piece_is_enemy(chess_state, revealed_square))
    return 0;

  piece_t revealed_piece = piece(chess_state, revealed_square);
  // check piece can move to the king square
  if (!((revealed_piece & BISHOP) &&
        bishop_increment(revealed_square, king_square)) &&
      !((revealed_piece & ROOK) &&
        rook_increment(revealed_square, king_square))) {
    return 0;
  }
  return 1;
}

// initialize variables
// if move is castle, check if moved rook is checking opposing king
// check if the moved piece can move to the kingsquare
// check if the moved piece was obstructing a sliding piece which is now
// revealed
int is_check_after_move(const chess_state_t* chess_state, move_t move) {
  sq0x88_t king_square;
  if (chess_state->black_to_move) {
    king_square = chess_state->white_pieces.king_square;
  } else {
    king_square = chess_state->black_pieces.king_square;
  }
  piece_t moved_piece;
  if (is_promotion(move)) {
    moved_piece = get_promotes_to(move);
  } else {
    moved_piece = piece(chess_state, move.from);
  }
  sq0x88_t to = get_to(move);
  sq0x88_t from = get_from(move);

  if (is_queen_castle(move)) {
    sq0x88_t rook_moved_to = to + (sq0x88_t)1;
    sq0x88_t inc = rook_increment(rook_moved_to, king_square);
    if (inc &&
        forwards_ray_cast(chess_state, rook_moved_to, inc) == king_square) {
      return 1;
    }
  }
  if (is_king_castle(move)) {
    sq0x88_t rook_moved_to = to - (sq0x88_t)1;
    sq0x88_t inc = rook_increment(rook_moved_to, king_square);
    if (inc &&
        forwards_ray_cast(chess_state, rook_moved_to, inc) == king_square) {
      return 1;
    }
  }
  // if moved piece has bishop flag set, i.e. it is a bishop or queen
  if (moved_piece &
      BISHOP) {  // <- maybe should be an API for this to abstract it
    sq0x88_t inc = bishop_increment(to, king_square);
    if (inc) {
      sq0x88_t square = forwards_ray_cast(chess_state, to, inc);
      if (square == king_square) {
        return 1;
      }
      if (square == from &&
          forwards_ray_cast(chess_state, from, inc) == king_square) {
        return 1;
      }
    }
  }
  // if moved piece has rook flag set, i.e. it is a bishop or queen
  if (moved_piece & ROOK) {
    sq0x88_t inc = rook_increment(to, king_square);
    sq0x88_t square = forwards_ray_cast(chess_state, to, inc);
    if (square == king_square) {
      return 1;
    }
    if (square == from &&
        forwards_ray_cast(chess_state, from, inc) == king_square) {
      return 1;
    }
  }
  if ((moved_piece & KNIGHT) && knight_increment(to, king_square)) {
    return 1;
  }

  // pawn threats
  sq0x88_t pawn_inc = pawn_push_increment(chess_state->friendly_colour);
  if ((moved_piece & PAWN) && ((sq0x88_t)(to + pawn_inc + 1) == king_square ||
                               (sq0x88_t)(to + pawn_inc - 1) == king_square)) {
    return 1;
  }
  // consider reveals
  if (discover_check_after_move(chess_state, king_square, from, to)) {
    return 1;
  }

  if (is_enpassent(move) &&
      discover_enpassent_check_after_move(chess_state, king_square, from, to, to - pawn_inc)) {
    return 1;
  }

  return 0;
}

void update_discover_check(chess_state_t* chess_state, sq0x88_t king_square,
                           sq0x88_t revealing_piece_from,
                           sq0x88_t revealing_piece_to) {
  sq0x88_t inc = queen_increment(revealing_piece_from, king_square);
  if (!inc) return;
  // check if piece is still in the same "line" with the King as before
  if (inc == queen_increment(revealing_piece_to, king_square)) return;

  // if another piece is between this piece and the king it is not a reveal
  if (forwards_ray_cast(chess_state, revealing_piece_from, inc) != king_square)
    return;

  // get the piece that is revealed (if one exists)
  piece_t revealed_square =
      backwards_ray_cast(chess_state, revealing_piece_from, inc);

  if (off_the_board(revealed_square) ||
      piece_is_friendly(chess_state, revealed_square))
    return;

  piece_t revealed_piece = piece(chess_state, revealed_square);
  // check piece can move to the king square
  if (!((revealed_piece & BISHOP) &&
        bishop_increment(revealed_square, king_square)) &&
      !((revealed_piece & ROOK) &&
        rook_increment(revealed_square, king_square))) {
    return;
  }
  chess_state->check_square = revealed_square;
  chess_state->n_checks++;
  chess_state->discovered_check = 1;
}

// initialize variables
// if move is castle, check if moved rook is checking opposing king
// check if the moved piece can move to the kingsquare
// check if the moved piece was obstructing a sliding piece which is now
// revealed

void update_check(chess_state_t* chess_state, move_t move) {
  sq0x88_t king_square;
  if (chess_state->black_to_move) {
    king_square = chess_state->black_pieces.king_square;
  } else {
    king_square = chess_state->white_pieces.king_square;
  }

  chess_state->check_square = 0x88;
  chess_state->n_checks = 0;
  chess_state->discovered_check = 0;

  piece_t moved_piece = piece(chess_state, move.to);
  sq0x88_t to = get_to(move);
  sq0x88_t from = get_from(move);

  if (is_queen_castle(move)) {
    sq0x88_t rook_moved_to = to + (sq0x88_t)1;
    sq0x88_t inc = rook_increment(rook_moved_to, king_square);
    if (inc &&
        forwards_ray_cast(chess_state, rook_moved_to, inc) == king_square) {
      chess_state->check_square = rook_moved_to;
      chess_state->n_checks++;
    }
  }
  if (is_king_castle(move)) {
    sq0x88_t rook_moved_to = to - (sq0x88_t)1;
    sq0x88_t inc = rook_increment(rook_moved_to, king_square);
    if (inc &&
        forwards_ray_cast(chess_state, rook_moved_to, inc) == king_square) {
      chess_state->check_square = rook_moved_to;
      chess_state->n_checks++;
    }
  }
  // if moved piece has bishop flag set, i.e. it is a bishop or queen
  if (moved_piece &
      BISHOP) {  // <- maybe should be an API for this to abstract it
    sq0x88_t inc = bishop_increment(to, king_square);
    if (inc && forwards_ray_cast(chess_state, to, inc) == king_square) {
      chess_state->check_square = to;
      chess_state->n_checks++;
    }
  }
  // if moved piece has rook flag set, i.e. it is a bishop or queen
  if (moved_piece & ROOK) {
    sq0x88_t inc = rook_increment(to, king_square);
    if (inc && forwards_ray_cast(chess_state, to, inc) == king_square) {
      chess_state->check_square = to;
      chess_state->n_checks++;
    }
  }
  if (moved_piece & KNIGHT) {
    sq0x88_t inc = knight_increment(to, king_square);
    if (inc) {
      chess_state->check_square = to;
      chess_state->n_checks++;
    }
  }

  // pawn threats
  sq0x88_t pawn_inc = chess_state->down_increment;
  if ((moved_piece & PAWN) && ((sq0x88_t)(to + pawn_inc + 1) == king_square ||
                               (sq0x88_t)(to + pawn_inc - 1) == king_square)) {
    chess_state->check_square = to;
    chess_state->n_checks++;
  }
  // consider reveals
  update_discover_check(chess_state, king_square, from, to);

  if (!chess_state->discovered_check && is_enpassent(move)) {
    update_discover_check(chess_state, king_square, to - pawn_inc, to);
  }

  if (is_double_check(chess_state) && !is_discover_check(chess_state)) {
    fprintf(stderr, "invalid double check!!!\n");
  }
}

int is_under_attack(const chess_state_t* chess_state, sq0x88_t square,
                    piece_t colour) {
  colour_t attacker = opposite_colour(colour);
  const piece_list_t* enemy_pieces = get_piece_list(chess_state, attacker);
  const piece_list_t* friendly_pieces = get_piece_list(chess_state, colour);
  sq0x88_t enemy_king_square = enemy_pieces->king_square;
  sq0x88_t king_square = friendly_pieces->king_square;

  // king moves
  if (king_increment(enemy_king_square, square)) return 1;

  // queen moves
  for (int p = 0; p < enemy_pieces->queen_count; p++) {
    sq0x88_t from = enemy_pieces->queen_list[p];
    sq0x88_t inc = queen_increment(from, square);
    if (inc == 0) continue;
    sq0x88_t next = backwards_ray_cast(chess_state, square, inc);
    if (next == from) return 1;
    if (next == king_square &&
        backwards_ray_cast(chess_state, next, inc) == from)
      return 1;
  }

  // rook moves
  for (int p = 0; p < enemy_pieces->rook_count; p++) {
    sq0x88_t from = enemy_pieces->rook_list[p];
    sq0x88_t inc = rook_increment(from, square);
    if (inc == 0) continue;
    sq0x88_t next = backwards_ray_cast(chess_state, square, inc);
    if (next == from) return 1;
    if (next == king_square &&
        backwards_ray_cast(chess_state, next, inc) == from)
      return 1;
  }

  if (is_light_square(square)) {
    // light bishop moves
    for (int p = 0; p < enemy_pieces->light_bishop_count; p++) {
      sq0x88_t from = enemy_pieces->light_bishop_list[p];
      sq0x88_t inc = bishop_increment(from, square);
      if (inc == 0) continue;
      sq0x88_t next = backwards_ray_cast(chess_state, square, inc);
      if (next == from) return 1;
      if (next == king_square &&
          backwards_ray_cast(chess_state, next, inc) == from)
        return 1;
    }
  } else {
    // dark bishop moves
    for (int p = 0; p < enemy_pieces->dark_bishop_count; p++) {
      sq0x88_t from = enemy_pieces->dark_bishop_list[p];
      sq0x88_t inc = bishop_increment(from, square);
      if (inc == 0) continue;
      sq0x88_t next = backwards_ray_cast(chess_state, square, inc);
      if (next == from) return 1;
      if (next == king_square &&
          backwards_ray_cast(chess_state, next, inc) == from)
        return 1;
    }
  }

  // knight moves
  for (int p = 0; p < enemy_pieces->knight_count; p++) {
    sq0x88_t from = enemy_pieces->knight_list[p];
    if (knight_increment(from, square)) return 1;
  }

  // pawn captures
  sq0x88_t inc = pawn_push_increment(attacker);
  sq0x88_t from;
  from = square - inc + (sq0x88_t)1;
  if (!off_the_board(from) && piece(chess_state, from) == (attacker | PAWN))
    return 1;
  from = square - inc - (sq0x88_t)1;
  if (!off_the_board(from) && piece(chess_state, from) == (attacker | PAWN))
    return 1;

  return 0;
}

void init_check(chess_state_t* chess_state) {
  piece_list_t* pl;
  piece_t opposing_player;
  sq0x88_t king_square;
  if (chess_state->black_to_move) {
    pl = &chess_state->white_pieces;
    opposing_player = WHITE;
    king_square = chess_state->black_pieces.king_square;
  } else {
    pl = &chess_state->black_pieces;
    opposing_player = BLACK;
    king_square = chess_state->white_pieces.king_square;
  }

  chess_state->check_square = 0x88;
  chess_state->discovered_check = 0;
  chess_state->n_checks = 0;

  // queen moves
  for (int p = 0; p < pl->queen_count; p++) {
    sq0x88_t from = pl->queen_list[p];
    sq0x88_t inc = queen_increment(from, king_square);
    if (inc && forwards_ray_cast(chess_state, from, inc) == king_square) {
      chess_state->check_square = from;
      chess_state->n_checks++;
    }
  }

  // rook moves
  for (int p = 0; p < pl->rook_count; p++) {
    sq0x88_t from = pl->rook_list[p];
    sq0x88_t inc = rook_increment(from, king_square);
    if (inc && forwards_ray_cast(chess_state, from, inc) == king_square) {
      chess_state->check_square = from;
      chess_state->n_checks++;
    }
  }

  if (is_light_square(king_square)) {
    // light bishop moves
    for (int p = 0; p < pl->light_bishop_count; p++) {
      sq0x88_t from = pl->light_bishop_list[p];
      sq0x88_t inc = bishop_increment(from, king_square);
      if (inc && forwards_ray_cast(chess_state, from, inc) == king_square) {
        chess_state->check_square = from;
        chess_state->n_checks++;
      }
    }
  } else {
    // dark bishop moves
    for (int p = 0; p < pl->dark_bishop_count; p++) {
      sq0x88_t from = pl->dark_bishop_list[p];
      sq0x88_t inc = bishop_increment(from, king_square);
      if (inc && forwards_ray_cast(chess_state, from, inc) == king_square) {
        chess_state->check_square = from;
        chess_state->n_checks++;
      }
    }
  }

  // knight moves
  for (int p = 0; p < pl->knight_count; p++) {
    sq0x88_t from = pl->knight_list[p];
    if (knight_increment(from, king_square)) {
      chess_state->check_square = from;
      chess_state->n_checks++;
    }
  }

  // pawn captures
  sq0x88_t inc = chess_state->down_increment;
  sq0x88_t from;
  from = king_square - inc + 1;
  if (!off_the_board(from) &&
      piece(chess_state, from) == (opposing_player | PAWN)) {
    chess_state->check_square = from;
    chess_state->n_checks++;
  }
  from = king_square - inc - 1;
  if (!off_the_board(from) &&
      piece(chess_state, from) == (opposing_player | PAWN)) {
    chess_state->check_square = from;
    chess_state->n_checks++;
  }
}

int is_check(const chess_state_t* chess_state) {
  return chess_state->n_checks > 0;
}

int is_double_check(const chess_state_t* chess_state) {
  return chess_state->n_checks > 1;
}

int is_discover_check(const chess_state_t* chess_state) {
  return chess_state->discovered_check;
}

sq0x88_t checking_square(const chess_state_t* chess_state) {
  return chess_state->check_square;
}