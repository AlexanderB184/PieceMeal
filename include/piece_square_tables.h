#ifndef PIECE_SQUARE_TABLES_H
#define PIECE_SQUARE_TABLES_H

#include "chess.h"

centipawn_t pawn_score_table[64] = {
      0,   0,   0,   0,   0,   0,   0,   0,
    100, 100,  75,  25,  25, 100, 100, 100,
     50,  50,  50,  50,  50,  25,  50,  50,
      0,   0,   0,  75,  75,  50,   0,   0,
     50, -10, -10, 100, 100,  75, -10,  50,
    100,  50,  50, 150, 150, 100,  50, 100,
    250, 250, 250, 250, 250, 250, 250, 250,
      0,   0,   0,   0,   0,   0,   0,   0,
};

centipawn_t knight_score_table[64] = {
    -75, -50, -25, -25, -25, -25, -50, -75,
    -50, -25,  25,  25,  25,  25, -25, -50,
    -25,  25,  75,  75,  75,  75,  25, -25,
    -25,  25,  75, 100, 100,  75,  25, -25,
    -25,  25,  75, 100, 100,  75,  25, -25,
    -25,  25,  75,  75,  75,  75,  25, -25,
    -50, -25,  25,  25,  25,  25, -25, -50,
    -75, -50, -25, -25, -25, -25, -50, -75,
};

centipawn_t bishop_score_table[64] = {

};

centipawn_t rook_score_table[64] = {

};

centipawn_t queen_score_table[64] = {

};

centipawn_t king_score_table[64] = {

};

#endif // PIECE_SQUARE_TABLES_H