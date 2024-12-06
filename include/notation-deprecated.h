#ifndef CHESS_NOTATION_H
#define CHESS_NOTATION_H

#include "chess-lib-inlines.h"
#include "chess-lib-types.h"

int write_square(char* buffer, size_t buffer_size, sq0x88_t square);
sq0x88_t read_square(const char* buffer, size_t buffer_size);

int write_piece(char* buffer, size_t buffer_size, piece_t piece);
piece_t read_piece(const char* buffer, size_t buffer_size);

int write_algebraic_notation(char* buffer, size_t buffer_size,
                             const chess_state_t* chess_state, move_t move);
move_t read_algebraic_notation(const char* buffer, size_t buffer_size,
                               const chess_state_t* chess_state);

int write_long_algebraic_notation(char* buffer, size_t buffer_size,
                                  move_t move);
move_t read_long_algebraic_notation(const char* buffer, size_t buffer_size,
                                    const chess_state_t* chess_state);

size_t write_movetext(char* buffer, size_t buffer_size,
                      const chess_state_t* chess_state);

int write_pgn(char* buffer, size_t buffer_size,
              const chess_state_t* chess_state, const char* white_name,
              const char* black_name, const char* fen);

#endif