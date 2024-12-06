#include "../include/chess-lib.h"
#include "../include/private/chess-lib-internals.h"

zobrist_t zobrist_flip_turn(zobrist_t zobrist) {
  return zobrist ^ zobrist_black_to_move;
}

zobrist_t zobrist_flip_piece(zobrist_t zobrist, piece_t piece,
                             sq0x88_t square) {
  int piece_index;
  sq8x8_t square_index = sq0x88_to_sq8x8(square);
  switch (piece) {
    default: return zobrist;
    case WHITE_PAWN:   piece_index = 0;  break;
    case WHITE_KNIGHT: piece_index = 1;  break;
    case WHITE_BISHOP: piece_index = 2;  break;
    case WHITE_ROOK:   piece_index = 3;  break;
    case WHITE_QUEEN:  piece_index = 4;  break;
    case WHITE_KING:   piece_index = 5;  break;
    case BLACK_PAWN:   piece_index = 6;  break;
    case BLACK_KNIGHT: piece_index = 7;  break;
    case BLACK_BISHOP: piece_index = 8;  break;
    case BLACK_ROOK:   piece_index = 9;  break;
    case BLACK_QUEEN:  piece_index = 10; break;
    case BLACK_KING:   piece_index = 11; break;
  }

  return zobrist ^ zobrist_table[piece_index][square_index];
}