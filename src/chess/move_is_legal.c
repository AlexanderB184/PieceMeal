#include <stdio.h>
#include <stdlib.h>

#include "../../include/chess.h"

int is_pinned(const chess_state_t* chess_state, move_t move, colour_t colour) {
  sq0x88_t from = get_from(move);
  sq0x88_t to = get_to(move);
  const piece_list_t* piece_list = get_piece_list(chess_state, colour);

  sq0x88_t king_square = piece_list->king_square;
  // checking moved piece is not pinned
  sq0x88_t inc = queen_increment(king_square, from);
  // piece is not aligned with king
  if (inc == 0) return 0;
  // piece is on same line with king
  if (inc == queen_increment(king_square, to)) return 0;
  // piece between moved piece and king
  if (forwards_ray_cast(chess_state, king_square, inc) != from) return 0;
  // find potential pinning piece
  sq0x88_t pinning_square = forwards_ray_cast(chess_state, from, inc);
  if (off_the_board(pinning_square) ||
      piece_is_colour(chess_state, pinning_square, colour))
    return 0;
  // only sliding pieces can pin
  if (((piece(chess_state, pinning_square) & BISHOP) &&
       bishop_increment(king_square, pinning_square)) ||
      ((piece(chess_state, pinning_square) & ROOK) &&
       rook_increment(king_square, pinning_square))) {
    return 1;
  }
  return 0;
}

int is_pinned_enpassent(const chess_state_t* chess_state, move_t move, colour_t colour) {
  sq0x88_t from = get_from(move);
  const piece_list_t* piece_list = get_piece_list(chess_state, colour);
  sq0x88_t king_square = piece_list->king_square;
  sq0x88_t inc;
  sq0x88_t pawn_inc = pawn_push_increment(colour);

  inc = bishop_increment(
      king_square, enpassent_target(chess_state) - pawn_inc);
  sq0x88_t captured_pawn =
      enpassent_target(chess_state) - pawn_inc;
  // bishop pin
  if (inc) {
    if (forwards_ray_cast(chess_state, king_square, inc) != captured_pawn)
      return 0;

    sq0x88_t pinning_square =
        forwards_ray_cast(chess_state, captured_pawn, inc);

    if (off_the_board(pinning_square) ||
        piece_is_colour(chess_state, pinning_square, colour))
      return 0;

    if (piece(chess_state, pinning_square) & BISHOP) return 1;

    return 0;
  }

  inc = rook_increment(king_square, captured_pawn);

  if (inc == 0) return 0;

  if (inc != rook_increment(king_square, from)) return 0;

  sq0x88_t square = forwards_ray_cast(chess_state, king_square, inc);

  if (square != captured_pawn && square != from) return 0;

  square = forwards_ray_cast(chess_state, square + inc, inc);

  if (off_the_board(square) || piece_is_colour(chess_state, square, colour)) return 0;

  if (piece(chess_state, square) & ROOK) return 1;

  return 0;
}

void trace_ply_stack(const chess_state_t* chess_state) {
  char buffer[512];
  fprintf(stdout, "error at ply %d %d\n", chess_state->ply_counter, chess_state->black_to_move);
  for (int i = 0; i < chess_state->ply_counter; i++) {
    move_t move = chess_state->ply_stack[i].move;
    fprintf(stdout, "%d %d %d\n", move.from, move.to, move.priority_and_flags);
    
  }
  write_movetext_debug(buffer, 512, chess_state);
  fprintf(stdout, "%s\n", buffer);
}

int is_legal_king_move(const chess_state_t* chess_state, move_t move, colour_t colour) {
  sq0x88_t from = get_from(move);
  sq0x88_t to = get_to(move);

  if (is_under_attack(chess_state, to, colour)) return 0;
  if (is_queen_castle(move)) {
    return !is_under_attack(chess_state, from - 1, colour);
  }
  if (is_king_castle(move)) {
    return !is_under_attack(chess_state, from + 1, colour);
  }
  return 1;
}

int is_moving_out_of_check(const chess_state_t* chess_state, move_t move,
                           sq0x88_t king_square, colour_t colour) {
  if (colour != chess_state->friendly_colour) return 1;
  sq0x88_t from = get_from(move);
  sq0x88_t to = get_to(move);
  if (!is_enpassent(move)) {
    if (is_capture(move) && to != checking_square(chess_state)) {
      return 0;
    }
    if (!is_capture(move)) {
      sq0x88_t inc = queen_increment(king_square, to);
      if (inc == 0 ||
          inc != queen_increment(to, checking_square(chess_state))) {
        return 0;
      }
    }
  } else {
    sq0x88_t inc = queen_increment(king_square, to);
    if (rankfile_to_sq0x88(sq0x88_to_rank07(from), sq0x88_to_file07(to)) !=
            checking_square(chess_state) &&
        (inc == 0 ||
         inc != queen_increment(to, checking_square(chess_state)))) {
      return 0;
    }
  }
  return 1;
}

// checks if pseudo legal move is legal, assumes position is not in check
int is_legal(const chess_state_t* chess_state, move_t move) {
  return is_legal_internal(chess_state, move, chess_state->friendly_colour);
}

int is_legal_internal(const chess_state_t* chess_state, move_t move, colour_t colour) {
  sq0x88_t from = get_from(move);

  // if (piece(chess_state, to) & KING) {
  //   trace_ply_stack(chess_state);
  //   printf("can capture king\n");
  //   abort();
  // }
  const piece_list_t* piece_list = get_piece_list(chess_state, colour);
  sq0x88_t king_square = piece_list->king_square;

  if (from == king_square) {  // king moves
    return is_legal_king_move(chess_state, move, colour);
  }
  //if (is_check(chess_state) && !is_moving_out_of_check(chess_state, move, king_square, colour)) {
  //  return 0;
  //}
  if (is_enpassent(move)) {
    if (is_pinned_enpassent(chess_state, move, colour)) return 0;
  }

  return !is_pinned(chess_state, move, colour);
}

int is_pseudo_legal_king_move(const chess_state_t* chess_state, move_t move, colour_t colour) {
  if (is_king_castle(move)) {
    return can_castle_king_side(chess_state, colour) && !is_check(chess_state);
  }
  if (is_queen_castle(move)) {
    return can_castle_queen_side(chess_state, colour) && !is_check(chess_state);
  }
  return king_increment(get_from(move), get_to(move)) != 0;
}

int is_pseudo_legal_knight_move(const chess_state_t* chess_state, move_t move) {
  (void)chess_state;
  return knight_increment(get_from(move), get_to(move)) != 0;
}

int is_pseudo_legal_pawn_move(const chess_state_t* chess_state, move_t move, colour_t colour) {
  sq0x88_t from = get_from(move);
  sq0x88_t to = get_to(move);
  sq0x88_t inc = pawn_push_increment(colour);
  if (is_double_pawn_push(move)) {
    return (sq0x88_to_rank07(from) == 1 || sq0x88_to_rank07(from) == 6) && (sq0x88_t)(from + 2 * inc) == to && piece_is_empty(chess_state, from + inc) &&
           piece_is_empty(chess_state, to);
  }
  if (is_enpassent(move) && enpassent_target(chess_state) != to && colour == chess_state->friendly_colour) {
    return 0;
  }
  if ((sq0x88_to_rank07(to) == 0 || sq0x88_to_rank07(to) == 7) && !is_promotion(move)) {
    return 0;
  }
  if (is_capture(move)) {
    return (to == (sq0x88_t)(from + inc + 1) || to == (sq0x88_t)(from + inc - 1));
  }
  return (to == (sq0x88_t)(from + inc) && piece_is_empty(chess_state, to));
}

int is_pseudo_legal_sliding_move(const chess_state_t* chess_state, move_t move,
                                 sq0x88_t inc) {
  sq0x88_t from = get_from(move);
  sq0x88_t to = get_to(move);
  return (backwards_ray_cast(chess_state, to, inc) == from);
}

int is_pseudo_legal_internal(const chess_state_t* chess_state, move_t move, colour_t colour) {
  sq0x88_t from = get_from(move);
  sq0x88_t to = get_to(move);
  colour_t enemy_colour = opposite_colour(colour);

  if (off_the_board(from) || off_the_board(to)) {
    return 0;  // move is out of bounds
  }
  if (is_null_move(move)) {
    return 0;  // null move
  }
  if (!piece_is_colour(chess_state, from, colour) ||
      piece_is_colour(chess_state, to, colour)) {
    // moving piece that isn't ours or doesn't exist or to square
    // occupied by our piece
    return 0;
  }

  // validate move flags
  if (is_promotion(move) && ((piece(chess_state, from) & PIECE_MASK) != PAWN ||
      !is_promoting(from, colour))) {
    return 0;
  }
  if (is_enpassent(move) && ((piece(chess_state, from) & PIECE_MASK) != PAWN ||
      enpassent_target(chess_state) != to)) {
    return 0;
  }
  if (is_queen_castle(move) &&
      ((piece(chess_state, from) & PIECE_MASK) != KING ||
      !can_castle_queen_side(chess_state, colour) || to != from - 2)) {
    return 0;
  }
  if (is_king_castle(move) && ((piece(chess_state, from) & PIECE_MASK) != KING ||
      !can_castle_king_side(chess_state, colour) || to != from + 2)) {
    return 0;
  }
  // is a capture
  if (!is_enpassent(move) && is_capture(move) &&
      !piece_is_colour(chess_state, to, enemy_colour)) {
    return 0;
  }
  if (!is_capture(move) && piece_is_colour(chess_state, to, enemy_colour)) {
    return 0;
  }
  if (is_double_pawn_push(move) && (piece(chess_state, from) & PIECE_MASK) != PAWN) {
    return 0;
  }

  // piece movement is valid
  switch (piece(chess_state, from) & PIECE_MASK) {
    case PAWN:
      
      if (!is_pseudo_legal_pawn_move(chess_state, move, colour)) {
        return 0;
      }
      break;
    case KNIGHT:
      if (!is_pseudo_legal_knight_move(chess_state, move)) {
        return 0;
      }
      break;
    case BISHOP: {
      sq0x88_t inc = bishop_increment(from, to);
      if (!inc || !is_pseudo_legal_sliding_move(chess_state, move, inc)) {
        return 0;
      }
    } break;
    case ROOK: {
      sq0x88_t inc = rook_increment(from, to);
      if (!inc || !is_pseudo_legal_sliding_move(chess_state, move, inc)) {
        return 0;
      }
    } break;
    case QUEEN: {
      sq0x88_t inc = queen_increment(from, to);
      if (!inc || !is_pseudo_legal_sliding_move(chess_state, move, inc)) {
        return 0;
      }
    } break;
    case KING:
      if (!is_pseudo_legal_king_move(chess_state, move, colour)) {
        return 0;
      }
      break;
    default:
      return 0;
  }

  // check rules for psuedo legal (ignoring pin checks & moving king to
  // threatened square)
  if (!is_check(chess_state)) {
    return 1;
  }

  if ((piece(chess_state, from) & PIECE_MASK) == KING) {
    return 1;
  }

  if (is_double_check(chess_state)) {
    return 0;
  }

  sq0x88_t check_square = checking_square(chess_state);
  // capturing checking piece
  if (is_enpassent(move) && check_square == enpassent_target(chess_state) - pawn_push_increment(colour)) {
    return 1;
  }
  if (is_capture(move) && !is_enpassent(move)) {
    return to == check_square;
  }
  // checker is not interposable
  if ((piece(chess_state, check_square) & QUEEN) == 0) {
    return 0;
  }
  sq0x88_t king_square = get_piece_list(chess_state, colour)->king_square;
  sq0x88_t interpose_inc = queen_increment(king_square, to);
  // interposing between checker and king
  if (interpose_inc == 0) {
    return 0;
  }
  if (interpose_inc != queen_increment(to, check_square)) {
    return 0;
  }
  
  return 1;
}

int is_pseudo_legal(const chess_state_t* chess_state, move_t move) {
  return is_pseudo_legal_internal(chess_state, move, chess_state->friendly_colour);
}