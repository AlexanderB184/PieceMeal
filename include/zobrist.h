#include "chess-lib-types.h"

// zobrist own inverse function to incrementally update the running zobrist hash of the current position
zobrist_t zobrist_flip_turn(zobrist_t zobrist);

// zobrist own inverse function to incrementally update the running zobrist hash of the current position
zobrist_t zobrist_flip_piece(zobrist_t zobrist, piece_t piece, sq0x88_t square);