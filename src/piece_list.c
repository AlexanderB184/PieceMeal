
#include <assert.h>
#include <stdio.h>

#include "../include/chess-lib.h"
#include "../include/private/chess-lib-internals.h"

#define pl_remove(list, count, indices, index) \
  (list)[index] = (list)[--(count)];           \
  (indices)[sq0x88_to_sq8x8((list)[index])] = index;

#define pl_add(list, count, indices, target)    \
  (list)[count] = target;                       \
  (indices)[sq0x88_to_sq8x8(target)] = (count); \
  (count)++;

void remove_piece(chess_state_t* chess_state, sq0x88_t target) {
  piece_t piece = chess_state->board[target];
  chess_state->zobrist = zobrist_flip_piece(chess_state->zobrist, piece, target);

  piece_list_t* pl;
  if (piece & WHITE) {
    pl = &chess_state->white_pieces;
  } else if (piece & BLACK) {
    pl = &chess_state->black_pieces;
  } else {
    assert(0 && "piece has no colour");
    return;
  }
  int index = pl->indices_list[sq0x88_to_sq8x8(target)];
  switch (piece & PIECE_MASK) {
    case PAWN:
      pl_remove(pl->pawn_list, pl->pawn_count, pl->indices_list, index);
      break;
    case KNIGHT:
      pl_remove(pl->knight_list, pl->knight_count, pl->indices_list, index);
      break;
    case BISHOP:
      if (is_light_square(target)) {
        pl_remove(pl->light_bishop_list, pl->light_bishop_count,
                  pl->indices_list, index);
      } else {
        pl_remove(pl->dark_bishop_list, pl->dark_bishop_count, pl->indices_list,
                  index);
      }
      break;
    case ROOK:
      pl_remove(pl->rook_list, pl->rook_count, pl->indices_list, index);
      break;
    case QUEEN:
      pl_remove(pl->queen_list, pl->queen_count, pl->indices_list, index);
      break;
    default:
      assert(0 && "invalid piece code");
      break;
  }

  chess_state->board[target] = EMPTY;
}

void place_piece(chess_state_t* chess_state, sq0x88_t target, piece_t piece) {
  chess_state->zobrist = zobrist_flip_piece(chess_state->zobrist, piece, target);
  piece_list_t* pl;
  if (piece & WHITE) {
    pl = &chess_state->white_pieces;
  } else if (piece & BLACK) {
    pl = &chess_state->black_pieces;
  } else {
    // printf("%d,%x, %x\n", board.move_number, target, piece);

    assert(0 && "piece has no colour");
    return;
  }
  switch (piece & PIECE_MASK) {
    case KING:
      pl->king_square = target;
      break;
    case PAWN:
      pl_add(pl->pawn_list, pl->pawn_count, pl->indices_list, target);
      break;
    case KNIGHT:
      pl_add(pl->knight_list, pl->knight_count, pl->indices_list, target);
      break;
    case BISHOP:
      if (is_light_square(target)) {
        pl_add(pl->light_bishop_list, pl->light_bishop_count, pl->indices_list,
               target);

      } else {
        pl_add(pl->dark_bishop_list, pl->dark_bishop_count, pl->indices_list,
               target);
      }
      break;
    case ROOK:
      pl_add(pl->rook_list, pl->rook_count, pl->indices_list, target);
      break;
    case QUEEN:
      pl_add(pl->queen_list, pl->queen_count, pl->indices_list, target);
      break;
    default:
      assert(0 && "invalid piece code");
      break;
  }

  chess_state->board[target] = piece;
}

void move_piece(chess_state_t* chess_state, sq0x88_t from, sq0x88_t to) {
  piece_t piece = chess_state->board[from];
  chess_state->zobrist = zobrist_flip_piece(chess_state->zobrist, piece, from);
  chess_state->zobrist = zobrist_flip_piece(chess_state->zobrist, piece, to);

  piece_list_t* pl;
  if (piece & WHITE) {
    pl = &chess_state->white_pieces;
  } else if (piece & BLACK) {
    pl = &chess_state->black_pieces;
  } else {
    trace_ply_stack(chess_state);//printf("%d,%x, %x, %x\n", board.move_number, from, to, piece);
    assert(0 && "piece has no colour");
    return;
  }
  if ((piece & PIECE_MASK) == KING) {
    pl->king_square = to;
    chess_state->board[to] = piece;
    chess_state->board[from] = EMPTY;
    return;
  }
  int index = pl->indices_list[sq0x88_to_sq8x8(from)];
  pl->indices_list[sq0x88_to_sq8x8(to)] = index;
  switch (piece & PIECE_MASK) {
    case PAWN:
      pl->pawn_list[index] = to;
      break;
    case KNIGHT:
      pl->knight_list[index] = to;
      break;
    case BISHOP:
      if (is_light_square(from)) {
        // printf("%d\n", index);
        pl->light_bishop_list[index] = to;
      } else {
        // printf("%d\n", index);

        pl->dark_bishop_list[index] = to;
      }
      break;
    case ROOK:
      pl->rook_list[index] = to;
      break;
    case QUEEN:
      pl->queen_list[index] = to;
      break;
    default:
      assert(0 && "invalid piece code");
      break;
  }

  chess_state->board[to] = piece;
  chess_state->board[from] = EMPTY;
}
