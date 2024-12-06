#include "../include/chess-lib.h"
#include "../include/private/chess-lib-internals.h"

enum piece_values {
  PAWN_VALUE = 10,
  BISHOP_VALUE = 33,
  KNIGHT_VALUE = 32,
  ROOK_VALUE = 50,
  QUEEN_VALUE = 90,
  KING_VALUE = 200,
};

int value_of(piece_t piece) {
  switch (piece & PIECE_MASK) {
    case PAWN:
      return PAWN_VALUE;
    case KNIGHT:
      return KNIGHT_VALUE;
    case BISHOP:
      return BISHOP_VALUE;
    case ROOK:
      return ROOK_VALUE;
    case QUEEN:
      return QUEEN_VALUE;
    case KING:
      return KING_VALUE;
    default:
      return 0;
  }
}

int value_of_promotion(move_flags_t piece) {
  switch (piece & 0b11) {
    case KNIGHT_PROMOTE_TO:
      return KNIGHT_VALUE;
    case BISHOP_PROMOTE_TO:
      return BISHOP_VALUE;
    case ROOK_PROMOTE_TO:
      return ROOK_VALUE;
    case QUEEN_PROMOTE_TO:
      return QUEEN_VALUE;
    default:
      return 0;
  }
}

#define PRIORITY_MAX 0x0FFF



