#ifndef FEN_H
#define FEN_H

#include "chess-lib-types.h"

void load_start_position(chess_state_t* chess_state);
void release_position(chess_state_t* chess_state);
void copy_position(chess_state_t* chess_state, const chess_state_t* state_to_copy);
long load_position(chess_state_t* chess_state, const char* buffer);
long save_position(const chess_state_t* chess_state, char* buffer, long size);
void clear_position(chess_state_t* chess_state);

#endif