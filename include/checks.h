#ifndef CHECKS_H
#define CHECKS_H

#include "chess-lib-types.h"

// returns true if the players piece at square can be pseudo captured
int under_attack(const chess_state_t* chess_state, sq0x88_t square, piece_t player);

// returns true if there is atleast one piece threatening the king
int is_check(const chess_state_t* chess_state);

// returns true if two (or more) pieces are threatening the king
int is_double_check(const chess_state_t* chess_state);

// returns true if the king is checked through a discovered attack
int is_discover_check(const chess_state_t* chess_state);

// returns the square of the checking piece, if there is no checking piece it
// will return 0x88. if the position is double check it has undefined value and
// should not be used.
sq0x88_t checking_square(const chess_state_t* chess_state);

#endif