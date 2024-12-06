#ifndef CHESS_NOTATION_H
#define CHESS_NOTATION_H

#include "chess-lib-inlines.h"
#include "chess-lib-types.h"

long skip_whitespace(const char* buffer);

// writes a square in format file rank, e.g. a1, h4, c8
long write_square(char* buffer, size_t buffer_size, sq0x88_t square);

// reads a square in format file rank, e.g. a1, h4, c8
long read_square(const char* buffer, size_t buffer_size, sq0x88_t* square);

// writes the letter code for a piece, e.g. P, K, N
long write_piece(char* buffer, size_t buffer_size, piece_t piece);

// reads the letter code for a piece, e.g. P, K, N
long read_piece(const char* buffer, size_t buffer_size, piece_t* piece);

long write_algebraic_notation(char* buffer, size_t buffer_size,
                              chess_state_t* chess_state, move_t move);

long read_algebraic_notation(const char* buffer, size_t buffer_size,
                             const chess_state_t* chess_state, move_t* move);

// write to buffer in long algebraic notation
// e.g. e7e9Q or e1b3
long write_long_algebraic_notation(char* buffer, size_t buffer_size,
                                   move_t move);
// read from buffer a move in long algebraic notation
// e.g. e7e9Q or e1b3
long read_long_algebraic_notation(const char* buffer, size_t buffer_size,
                                  const chess_state_t* chess_state,
                                  move_t* move);

// writes a sequence of space seperated moves in algebraic notation.
long write_movetext(char* buffer, size_t buffer_size,
                    const chess_state_t* chess_state, const char* start_position);

// when debugging the chess_state struct may be in an invalid state after an illegal move,
// using write_movetext to traverse the plystack may be dangerous so instead use this version.
// Since write_long_algebraic_notation does not depend on the chess_state, all moves can be written out
// in a traversal without cloning the game and making moves.
long write_movetext_debug(char* buffer, size_t buffer_size,
                    const chess_state_t* chess_state);

// writes the pgn out to a buffer
long write_pgn(char* buffer, size_t buffer_size,
               const chess_state_t* chess_state, const char* event,
               const char* site, const char* date, const char* round,
               const char* white_name, const char* black_name, const char* fen);

#endif