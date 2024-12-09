#ifndef CHESS_STATE_H
#define CHESS_STATE_H

#include "chess-lib-types.h"

// loads default chess starting position
void load_start_position(chess_state_t* chess_state);

// release resources
void release_position(chess_state_t* chess_state);

// clones the position
void copy_position(chess_state_t* chess_state, const chess_state_t* state_to_copy);

// clears the position to an empty board
void clear_position(chess_state_t* chess_state);

// load game from fen string
long load_position(chess_state_t* chess_state, const char* fen_string);

// write fen string of current position
long save_position(const chess_state_t* chess_state, char* buffer, long buffer_size);

// updates the games state
void make_move(chess_state_t* chess_state,move_t move);

// restores the position to the previous state
void unmake_move(chess_state_t* chess_state);

#endif