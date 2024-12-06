#ifndef MOVE_H
#define MOVE_H

#include "chess-lib-types.h"

// updates the games state
void make_move(chess_state_t* chess_state,move_t move);

// restores the position to the previous state
void unmake_move(chess_state_t* chess_state);


int value_of(piece_t piece);
int value_of_promotion(move_flags_t piece);




#endif