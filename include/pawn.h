#ifndef PAWN_H
#define PAWN_H

#include "chess-lib-types.h"

int is_promoting(const chess_state_t* chess_state, sq0x88_t from);

int pawn_can_double_push(const chess_state_t* chess_state, sq0x88_t from);

int can_capture_enpassent(const chess_state_t*, sq0x88_t from);

#endif