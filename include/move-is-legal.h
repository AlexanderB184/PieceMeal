#ifndef IS_LEGAL_H
#define IS_LEGAL_H

#include "chess-lib.h"

// assumes the position is not in check.
// returns true if the move doesn't move an absolutely pinned piece
int is_legal(const chess_state_t* chess_state, move_t move);

// checks move rules, does not check if piece is pinned, must be combined with
// is_legal for full validation
int is_pseudo_legal(const chess_state_t* chess_state, move_t move);

// check if a move is fully legal in the given position
int is_valid_move(const chess_state_t* chess_state, move_t move);

// helper functions for move validation

int is_promoting(const chess_state_t* chess_state, sq0x88_t from);

int pawn_can_double_push(const chess_state_t* chess_state, sq0x88_t from);

int can_capture_enpassent(const chess_state_t*, sq0x88_t from);

int can_castle_king_side(const chess_state_t* chess_state);

int can_castle_queen_side(const chess_state_t* chess_state);

#endif