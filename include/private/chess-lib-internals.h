#ifndef CHESS_LIB_INTERNALS_H
#define CHESS_LIB_INTERNALS_H

#include "../chess-lib-types.h"

// casts a ray starting at square and iterating in the opposite direction to inc
sq0x88_t backwards_ray_cast(const chess_state_t* chess_state, sq0x88_t square,
                            sq0x88_t inc);

// casts a ray starting at square and iterating in the same direction as inc
sq0x88_t forwards_ray_cast(const chess_state_t* chess_state, sq0x88_t from,
                           sq0x88_t inc);

void trace_ply_stack(const chess_state_t* chess_state);

void init_check(chess_state_t* chess_state);

void push_ply_stack(chess_state_t* chess_state, move_t move);

void update_board(chess_state_t* chess_state, move_t move);

void update_rights(chess_state_t* chess_state, move_t move);

void update_enpassent_target(chess_state_t* chess_state, move_t move);

void update_half_move_clock(chess_state_t* chess_state, move_t move);

void update_turn(chess_state_t* chess_state);

  void update_check(chess_state_t*chess_state, move_t move);

void remove_piece(chess_state_t* chess_state, sq0x88_t target);

void place_piece(chess_state_t* chess_state, sq0x88_t target, piece_t piece);

void move_piece(chess_state_t* chess_state, sq0x88_t from, sq0x88_t to);


#endif