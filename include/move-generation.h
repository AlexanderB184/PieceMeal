#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include "chess-lib-types.h"

// incremental move generation
// all move gen is divided into 3 categories
// generate promotions
// generate captures (excluding capture promotions)
// generate quiet moves
// generate moves performs all 3 of these together
// each produces psuedo legal moves that MUST be checked for legality before
// being played each has a legal variant that performs this for the user
// although this is not required and incremental legality checking is supported
// and even encouraged

size_t generate_moves(const chess_state_t* chess_state, move_t* moves);
size_t generate_captures(const chess_state_t* chess_state, move_t* moves);
size_t generate_promotions(const chess_state_t* chess_state, move_t* moves);
size_t generate_quiet(const chess_state_t* chess_state, move_t* moves);

size_t generate_legal_moves(const chess_state_t* chess_state, move_t* moves);
size_t generate_legal_captures(const chess_state_t* chess_state, move_t* moves);
size_t generate_legal_promotions(const chess_state_t* chess_state, move_t* moves);
size_t generate_legal_quiets(const chess_state_t* chess_state, move_t* moves);


#endif