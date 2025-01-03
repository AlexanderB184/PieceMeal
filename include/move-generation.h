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

enum generator_mode {
  GENERATE_QUIETS = 1,
  GENERATE_CAPTURES = 2,
  GENERATE_PROMOTIONS = 4,
  GENERATE_ALL = GENERATE_QUIETS | GENERATE_CAPTURES | GENERATE_PROMOTIONS,
};

size_t generate_moves(const chess_state_t* chess_state, move_t* moves, enum generator_mode generation_mode);

size_t generate_legal_moves(const chess_state_t* chess_state, move_t* moves, enum generator_mode generation_mode);

#endif