#include "../../include/chess.h"
#include "../../include/zobrist.h"

zobrist_t zobrist_flip_turn(zobrist_t zobrist) {
  return zobrist ^ zobrist_black_to_move;
}

zobrist_t zobrist_flip_piece(zobrist_t zobrist, piece_t piece,
                             sq0x88_t square) {
  const int8_t piece_to_zobrist_index_map[17] = {
    -1,
    0,
    1,
    -1,
    2,
    -1,
    -1,
    -1,
    3,
    -1,
    -1,
    -1,
    4,
    -1,
    -1,
    -1,
    5,
  };
  int piece_index = piece_to_zobrist_index_map[piece & PIECE_MASK] + ((piece & COLOUR_MASK) == WHITE ? 0 : 6);
  int square_index = sq0x88_to_sq8x8(square);
  return zobrist ^ zobrist_table[piece_index][square_index];
}

zobrist_t zobrist_move_piece(zobrist_t zobrist, piece_t piece,
                             sq0x88_t from, sq0x88_t to) {
  const int8_t piece_to_zobrist_index_map[17] = {
    -1,
    0,
    1,
    -1,
    2,
    -1,
    -1,
    -1,
    3,
    -1,
    -1,
    -1,
    4,
    -1,
    -1,
    -1,
    5,
  };
  int piece_index = piece_to_zobrist_index_map[piece & PIECE_MASK] + ((piece & COLOUR_MASK) == WHITE ? 0 : 6);
  int from_index = sq0x88_to_sq8x8(from);
  int to_index = sq0x88_to_sq8x8(to);
  return zobrist ^ zobrist_table[piece_index][from_index] ^ zobrist_table[piece_index][to_index];

}