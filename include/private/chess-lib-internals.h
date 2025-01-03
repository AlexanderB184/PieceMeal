#ifndef CHESS_LIB_INTERNALS_H
#define CHESS_LIB_INTERNALS_H

#ifdef __cplusplus
extern "C" {
#endif

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

colour_t piece_colour(const chess_state_t* chess_state, sq0x88_t square);

int piece_is_colour(const chess_state_t* chess_state, sq0x88_t square,
                    colour_t colour);

colour_t opposite_colour(colour_t colour);

sq0x88_t pawn_push_increment(colour_t colour);

const piece_list_t* get_piece_list(const chess_state_t* chess_state, colour_t colour);

int is_legal_internal(const chess_state_t* chess_state, move_t move, colour_t colour);

size_t generate_moves_internal(const chess_state_t* chess_state, move_t * moves, colour_t colour, enum generator_mode generation_mode);
size_t generate_moves_nocheck_internal(const chess_state_t* chess_state, move_t * moves, colour_t colour, enum generator_mode generation_mode);
size_t generate_moves_check_internal(const chess_state_t* chess_state, move_t * moves, colour_t colour, enum generator_mode generation_mode);

#define FOR_EACH_PIECE(PIECE_LIST, PIECE_TYPE, IDENTIFIER)                     \
  for (uint8_t index = 0, IDENTIFIER = (PIECE_LIST)->PIECE_TYPE##_list[index]; \
       index < (PIECE_LIST)->PIECE_TYPE##_count;                               \
       index++, IDENTIFIER = (PIECE_LIST)->PIECE_TYPE##_list[index])

#ifdef __cplusplus
}
#endif

#endif