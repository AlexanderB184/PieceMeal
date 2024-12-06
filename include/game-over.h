#ifndef GAME_OVER_H
#define GAME_OVER_H

#include "chess-lib-types.h"

int is_draw(const chess_state_t* chess_state);

int is_draw_by_repetition(const chess_state_t* chess_state);

int is_draw_by_insufficient_material(const chess_state_t* chess_state);

int is_draw_by_50_move_rule(const chess_state_t* chess_state);

// is_checkmate and is_stalemate should be avoided for non leaf nodes to avoid duplication of move generation
int is_checkmate(const chess_state_t* chess_state);

// is_checkmate and is_stalemate should be avoided for non leaf nodes to avoid duplication of move generation
int is_stalemate(const chess_state_t* chess_state);

enum gameover_state { ONGOING = 0, CHECKMATE = 1, STALEMATE = -1, DRAW_BY_REPETITION = -2, DRAW_BY_50_MOVE_RULE = -3, DRAW_BY_INSUFFICIENT_MATERIAL = -4};

enum gameover_state is_gameover(const chess_state_t* chess_state);

colour_t get_winner(const chess_state_t* chess_state);

#endif