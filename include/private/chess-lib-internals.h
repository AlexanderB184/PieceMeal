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

size_t generate_moves_internal(const chess_state_t* chess_state,
                               const piece_list_t* piece_lists, move_t* moves);

size_t generate_captures_internal(const chess_state_t* chess_state,
                                  const piece_list_t* piece_lists,
                                  move_t* moves);

size_t generate_quiets_internal(const chess_state_t* chess_state,
                                const piece_list_t* piece_lists, move_t* moves);

size_t generate_promotions_internal(const chess_state_t* chess_state,
                                    const piece_list_t* piece_lists,
                                    move_t* moves);

// for each style loop for a specific piece in a piece list
// can be queen, rook, light_bishop, dark_bishop, knight or pawn
// cannot be king or bishop
// IDENTIFIER will be the name for the variable storing the position of the
// current piece, the index of the piece is saved as index
#define FOR_EACH_PIECE(PIECE_LIST, PIECE_TYPE, IDENTIFIER)                     \
  for (uint8_t index = 0, IDENTIFIER = (PIECE_LIST)->PIECE_TYPE##_list[index]; \
       index < (PIECE_LIST)->PIECE_TYPE##_count;                               \
       index++, IDENTIFIER = (PIECE_LIST)->PIECE_TYPE##_list[index])

#endif