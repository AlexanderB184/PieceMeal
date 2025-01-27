
#include <assert.h>
#include <stdio.h>

#include "../include/chess.h"

void remove_piece(chess_state_t* chess_state, sq0x88_t target) {
  piece_t piece = chess_state->board[target];
  chess_state->zobrist =
      zobrist_flip_piece(chess_state->zobrist, piece, target);

  piece_list_t* pl;

  if (piece & WHITE) {
    pl = &chess_state->white_pieces;
  } else if (piece & BLACK) {
    pl = &chess_state->black_pieces;
  } else {
    trace_ply_stack(chess_state);
    assert(0 && "piece has no colour");
    return;
  }

  uint8_t index = pl->indices_list[sq0x88_to_sq8x8(target)];
  uint8_t* piece_count;
  sq0x88_t* piece_list;

  switch (piece & PIECE_MASK) {
    case PAWN:
      piece_count = &pl->pawn_count;
      piece_list = pl->pawn_list;
      break;
    case KNIGHT:
      piece_count = &pl->knight_count;
      piece_list = pl->knight_list;
      break;
    case BISHOP:
      if (is_light_square(target)) {
        piece_count = &pl->light_bishop_count;
        piece_list = pl->light_bishop_list;
      } else {
        piece_count = &pl->dark_bishop_count;
        piece_list = pl->dark_bishop_list;
      }
      break;
    case ROOK:
      piece_count = &pl->rook_count;
      piece_list = pl->rook_list;
      break;
    case QUEEN:
      piece_count = &pl->queen_count;
      piece_list = pl->queen_list;
      break;
    default:
      trace_ply_stack(chess_state);
      assert(0 && "invalid piece code");
      break;
  }
  piece_list[index] = piece_list[--(*piece_count)];
  pl->indices_list[sq0x88_to_sq8x8(piece_list[index])] = index;
  chess_state->board[target] = EMPTY;
}

void place_piece(chess_state_t* chess_state, sq0x88_t target, piece_t piece) {
  chess_state->zobrist =
      zobrist_flip_piece(chess_state->zobrist, piece, target);
  piece_list_t* pl;
  if (piece & WHITE) {
    pl = &chess_state->white_pieces;
  } else if (piece & BLACK) {
    pl = &chess_state->black_pieces;
  } else {
    trace_ply_stack(chess_state);
    assert(0 && "piece has no colour");
    return;
  }
  if (piece & KING) {
    pl->king_square = target;
      chess_state->board[target] = piece;
      return;
  }

  sq0x88_t* piece_list;
  uint8_t* piece_count;
  switch (piece & PIECE_MASK) {
    case PAWN:
      piece_list = pl->pawn_list;
      piece_count = &pl->pawn_count;
      break;
    case KNIGHT:
       piece_list = pl->knight_list;
      piece_count = &pl->knight_count;
      break;
    case BISHOP:
      if (is_light_square(target)) {
         piece_list = pl->light_bishop_list;
         piece_count = &pl->light_bishop_count;
      } else {
         piece_list = pl->dark_bishop_list;
         piece_count = &pl->dark_bishop_count;
      }
      break;
    case ROOK:
       piece_list = pl->rook_list;
       piece_count = &pl->rook_count;
      break;
    case QUEEN:
       piece_list = pl->queen_list;
       piece_count = &pl->queen_count;
      break;
    default:
      assert(0 && "invalid piece code");
      break;
  }
  piece_list[*piece_count] = target;                       
  pl->indices_list[sq0x88_to_sq8x8(target)] = *piece_count; 
  (*piece_count)++;
  chess_state->board[target] = piece;
}

void move_piece(chess_state_t* chess_state, sq0x88_t from, sq0x88_t to) {
  piece_t piece = chess_state->board[from];
  chess_state->zobrist = zobrist_move_piece(chess_state->zobrist, piece, from, to);

  piece_list_t* pl;
  if (piece & WHITE) {
    pl = &chess_state->white_pieces;
  } else if (piece & BLACK) {
    pl = &chess_state->black_pieces;
  } else {
    trace_ply_stack(chess_state);
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
        pl->light_bishop_list[index] = to;
      } else {
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
