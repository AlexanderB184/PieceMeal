#ifndef CHESS_LIB_CONST_H
#define CHESS_LIB_CONST_H

#include "chess-lib-types.h"


#define RANK_SIZE 16
#define FILE_SIZE 16
#define LEFT_BORDER 0
#define TOP_BORDER 0

#define SQUARE_FROM_RANKFILE(file, rank) \
  ((rank) + TOP_BORDER) * FILE_SIZE + LEFT_BORDER + (file)

  
enum {
  a1 = SQUARE_FROM_RANKFILE(0, 0),
  a2 = SQUARE_FROM_RANKFILE(0, 1),
  a3 = SQUARE_FROM_RANKFILE(0, 2),
  a4 = SQUARE_FROM_RANKFILE(0, 3),
  a5 = SQUARE_FROM_RANKFILE(0, 4),
  a6 = SQUARE_FROM_RANKFILE(0, 5),
  a7 = SQUARE_FROM_RANKFILE(0, 6),
  a8 = SQUARE_FROM_RANKFILE(0, 7),
  b1 = SQUARE_FROM_RANKFILE(1, 0),
  b2 = SQUARE_FROM_RANKFILE(1, 1),
  b3 = SQUARE_FROM_RANKFILE(1, 2),
  b4 = SQUARE_FROM_RANKFILE(1, 3),
  b5 = SQUARE_FROM_RANKFILE(1, 4),
  b6 = SQUARE_FROM_RANKFILE(1, 5),
  b7 = SQUARE_FROM_RANKFILE(1, 6),
  b8 = SQUARE_FROM_RANKFILE(1, 7),
  c1 = SQUARE_FROM_RANKFILE(2, 0),
  c2 = SQUARE_FROM_RANKFILE(2, 1),
  c3 = SQUARE_FROM_RANKFILE(2, 2),
  c4 = SQUARE_FROM_RANKFILE(2, 3),
  c5 = SQUARE_FROM_RANKFILE(2, 4),
  c6 = SQUARE_FROM_RANKFILE(2, 5),
  c7 = SQUARE_FROM_RANKFILE(2, 6),
  c8 = SQUARE_FROM_RANKFILE(2, 7),
  d1 = SQUARE_FROM_RANKFILE(3, 0),
  d2 = SQUARE_FROM_RANKFILE(3, 1),
  d3 = SQUARE_FROM_RANKFILE(3, 2),
  d4 = SQUARE_FROM_RANKFILE(3, 3),
  d5 = SQUARE_FROM_RANKFILE(3, 4),
  d6 = SQUARE_FROM_RANKFILE(3, 5),
  d7 = SQUARE_FROM_RANKFILE(3, 6),
  d8 = SQUARE_FROM_RANKFILE(3, 7),
  e1 = SQUARE_FROM_RANKFILE(4, 0),
  e2 = SQUARE_FROM_RANKFILE(4, 1),
  e3 = SQUARE_FROM_RANKFILE(4, 2),
  e4 = SQUARE_FROM_RANKFILE(4, 3),
  e5 = SQUARE_FROM_RANKFILE(4, 4),
  e6 = SQUARE_FROM_RANKFILE(4, 5),
  e7 = SQUARE_FROM_RANKFILE(4, 6),
  e8 = SQUARE_FROM_RANKFILE(4, 7),
  f1 = SQUARE_FROM_RANKFILE(5, 0),
  f2 = SQUARE_FROM_RANKFILE(5, 1),
  f3 = SQUARE_FROM_RANKFILE(5, 2),
  f4 = SQUARE_FROM_RANKFILE(5, 3),
  f5 = SQUARE_FROM_RANKFILE(5, 4),
  f6 = SQUARE_FROM_RANKFILE(5, 5),
  f7 = SQUARE_FROM_RANKFILE(5, 6),
  f8 = SQUARE_FROM_RANKFILE(5, 7),
  g1 = SQUARE_FROM_RANKFILE(6, 0),
  g2 = SQUARE_FROM_RANKFILE(6, 1),
  g3 = SQUARE_FROM_RANKFILE(6, 2),
  g4 = SQUARE_FROM_RANKFILE(6, 3),
  g5 = SQUARE_FROM_RANKFILE(6, 4),
  g6 = SQUARE_FROM_RANKFILE(6, 5),
  g7 = SQUARE_FROM_RANKFILE(6, 6),
  g8 = SQUARE_FROM_RANKFILE(6, 7),
  h1 = SQUARE_FROM_RANKFILE(7, 0),
  h2 = SQUARE_FROM_RANKFILE(7, 1),
  h3 = SQUARE_FROM_RANKFILE(7, 2),
  h4 = SQUARE_FROM_RANKFILE(7, 3),
  h5 = SQUARE_FROM_RANKFILE(7, 4),
  h6 = SQUARE_FROM_RANKFILE(7, 5),
  h7 = SQUARE_FROM_RANKFILE(7, 6),
  h8 = SQUARE_FROM_RANKFILE(7, 7),
};

static const move_t null_move = {0, 0, 0};

static const piece_t start_board[16 * 16] = {
WHITE_ROOK  , WHITE_KNIGHT, WHITE_BISHOP, WHITE_QUEEN , WHITE_KING  , WHITE_BISHOP, WHITE_KNIGHT, WHITE_ROOK  , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
WHITE_PAWN  , WHITE_PAWN  , WHITE_PAWN  , WHITE_PAWN  , WHITE_PAWN  , WHITE_PAWN  , WHITE_PAWN  , WHITE_PAWN  , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BLACK_PAWN  , BLACK_PAWN  , BLACK_PAWN  , BLACK_PAWN  , BLACK_PAWN  , BLACK_PAWN  , BLACK_PAWN  , BLACK_PAWN  , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BLACK_ROOK  , BLACK_KNIGHT, BLACK_BISHOP, BLACK_QUEEN , BLACK_KING  , BLACK_BISHOP, BLACK_KNIGHT, BLACK_ROOK  , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
};

static const piece_t empty_board[16 * 16] = {
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , EMPTY       , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , BORDER      , 
};

enum {
  INCREMENT_VECTOR_SIZE = 240,
  KING_INCREMENTS_COUNT = 8,
  QUEEN_INCREMENTS_COUNT = 8,
  KNIGHT_INCREMENTS_COUNT = 8,
  BISHOP_INCREMENTS_COUNT = 4,
  ROOK_INCREMENTS_COUNT = 4,
};

// interface for increment vectors

sq0x88_t queen_increment(sq0x88_t from, sq0x88_t to);

sq0x88_t rook_increment(sq0x88_t from, sq0x88_t to);

sq0x88_t bishop_increment(sq0x88_t from, sq0x88_t to);

sq0x88_t knight_increment(sq0x88_t from, sq0x88_t to);

sq0x88_t king_increment(sq0x88_t from, sq0x88_t to);

static const sq0x88_t queen_increment_vector[INCREMENT_VECTOR_SIZE] = {
    239,0,  0,  0,  0,  0,  0,  240,0,  0,  0,  0,  0,  0,  241,0,  
    0,  239,0,  0,  0,  0,  0,  240,0,  0,  0,  0,  0,  241,0,  0,  
    0,  0,  239,0,  0,  0,  0,  240,0,  0,  0,  0,  241,0,  0,  0,  
    0,  0,  0,  239,0,  0,  0,  240,0,  0,  0,  241,0,  0,  0,  0,  
    0,  0,  0,  0,  239,0,  0,  240,0,  0,  241,0,  0,  0,  0,  0,  
    0,  0,  0,  0,  0,  239,0,  240,0,  241,0,  0,  0,  0,  0,  0,  
    0,  0,  0,  0,  0,  0,  239,240,241,0,  0,  0,  0,  0,  0,  0,  
    255,255,255,255,255,255,255,0,  1,  1,  1,  1,  1,  1,  1,  0,  
    0,  0,  0,  0,  0,  0,  15, 16, 17, 0,  0,  0,  0,  0,  0,  0,  
    0,  0,  0,  0,  0,  15, 0,  16, 0,  17, 0,  0,  0,  0,  0,  0,  
    0,  0,  0,  0,  15 ,0,  0,  16, 0,  0,  17, 0,  0,  0,  0,  0,
    0,  0,  0,  15, 0,  0,  0,  16, 0,  0,  0,  17, 0,  0,  0,  0,  
    0,  0,  15, 0,  0,  0,  0,  16, 0,  0,  0,  0,  17, 0,  0,  0,  
    0,  15, 0,  0,  0,  0,  0,  16, 0,  0,  0,  0,  0,  17, 0,  0,  
    15, 0,  0,  0,  0,  0,  0,  16, 0,  0,  0,  0,  0,  0,  17, 0,  
};

static const sq0x88_t rook_increment_vector[INCREMENT_VECTOR_SIZE] = {
    0,  0,  0,  0,  0,  0,  0,  240,0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  240,0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  240,0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  240,0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  240,0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  240,0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  240,0,  0,  0,  0,  0,  0,  0,  0,
    255,255,255,255,255,255,255,0,  1,  1,  1,  1,  1,  1,  1,  0,
    0,  0,  0,  0,  0,  0,  0,  16, 0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  16, 0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  16, 0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  16, 0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  16, 0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  16, 0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  16, 0,  0,  0,  0,  0,  0,  0,  0,
};

static const sq0x88_t bishop_increment_vector[INCREMENT_VECTOR_SIZE] = {
    239,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  241,0,  
    0,  239,0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  241,0,  0,  
    0,  0,  239,0,  0,  0,  0,  0,  0,  0,  0,  0,  241,0,  0,  0,  
    0,  0,  0,  239,0,  0,  0,  0,  0,  0,  0,  241,0,  0,  0,  0,  
    0,  0,  0,  0,  239,0,  0,  0,  0,  0,  241,0,  0,  0,  0,  0,  
    0,  0,  0,  0,  0,  239,0,  0,  0,  241,0,  0,  0,  0,  0,  0,  
    0,  0,  0,  0,  0,  0,  239,0,  241,0,  0,  0,  0,  0,  0,  0,  
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  
    0,  0,  0,  0,  0,  0,  15, 0,  17, 0,  0,  0,  0,  0,  0,  0,  
    0,  0,  0,  0,  0,  15, 0,  0,  0,  17, 0,  0,  0,  0,  0,  0,  
    0,  0,  0,  0,  15, 0,  0,  0,  0,  0,  17, 0,  0,  0,  0,  0,  
    0,  0,  0,  15, 0,  0,  0,  0,  0,  0,  0,  17, 0,  0,  0,  0,  
    0,  0,  15, 0,  0,  0,  0,  0,  0,  0,  0,  0,  17, 0,  0,  0,  
    0,  15, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  17, 0,  0,  
    15, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  17, 0,  
};

static const sq0x88_t knight_increment_vector[INCREMENT_VECTOR_SIZE] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  223,0,  225,0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  238,0,  0,  0,  242,0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  14, 0,  0,  0,  18, 0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  31, 0, 33,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};
static const sq0x88_t king_increment_vector[INCREMENT_VECTOR_SIZE] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  239,240,241,0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  255,0,  1,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  15, 16, 17, 0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static const sq0x88_t white_pawn_push_inc = 16;
static const sq0x88_t black_pawn_push_inc = 240;

static const sq0x88_t king_increments_list[KING_INCREMENTS_COUNT] = {1, 255, 240, 16, 241, 239, 15, 17};
static const sq0x88_t knight_increments_list[KNIGHT_INCREMENTS_COUNT] = {18, 14, 33, 31, 242, 238, 225, 223};
static const sq0x88_t queen_increments_list[QUEEN_INCREMENTS_COUNT] = {1, 255, 240, 16, 241, 239, 15, 17};
static const sq0x88_t bishop_increments_list[BISHOP_INCREMENTS_COUNT] = {241, 239, 15, 17};
static const sq0x88_t rook_increments_list[ROOK_INCREMENTS_COUNT] = {1, 255, 240, 16};

static const zobrist_t zobrist_black_to_move = 0x4cca36c01e2261e5;

static const zobrist_t zobrist_table[12][64] = {
{0x3cfb29e645d2f08c, 0x2be4ea0b751276d6, 0x53ec80475581dce4, 0x152f19e6543501b9, 0x475afde739cefd17, 0x6f9423c41f0e9655, 0x2957549c278937d3, 0x1f971b1a4b59b507, 0x28bc03915e47c4dd, 0x2263c0bb6e28ca85, 0x2902eb4745384371, 0x2e2caca935f8b025, 0x3306fecf0b6c9c5c, 0x393693eb3dab907c, 0x6ad7dd3267c34b7e, 0x2d963c0b092ba589, 0x7e3e1c5f27d30718, 0x7d54e3fc597b2616, 0x2db027cf522a9ca6, 0x0bf999be1283fde2, 0x31929437750b25b6, 0x1c945d8a7b8dbd82, 0x46e772895ae9e8d3, 0x3931adb13c2b78a4, 0x2a54432a6fa3761b, 0x34dbb98c5b956e6c, 0x118e1e9253572e71, 0x5ec3cace63086635, 0x20b3f6b144951d61, 0x2c5868e017fa5eb9, 0x212604430b8bd3e3, 0x335edafb59eea4eb, 0x3369cb011f6420a2, 0x718ebd4930b3bef7, 0x4337bcd96119f2d0, 0x562518877d885707, 0x7916148974ca5110, 0x4fb439e472b97611, 0x2ee4eeb53ffd8712, 0x2fa0fd2d08e5e795, 0x647b5601593931df, 0x2c906051647cb6b9, 0x47851cef76097493, 0x3a9e91f50b542b1f, 0x234e89d8683913a0, 0x73c4e78466f6fad5, 0x40e59fc044748e1b, 0x63d8aebe2723c27f, 0x57d78177744f6ac1, 0x55695d9155676c07, 0x52efc30e1b0f3e50, 0x0fd98f612b8e7618, 0x1e47ec294c05d797, 0x0c035ea95f8dc945, 0x6873b0da4d2cdadf, 0x26660cbe3ba45bd7, 0x202112904cef06db, 0x42f87b6f52f66d0f, 0x5e4a982e67a62f7f, 0x4fdf43207d970d64, 0x648e083901992207, 0x460db02243a42aa4, 0x6ac7ed232573a7f9, 0x19c312ba29e65ee0, },
{0x7f4dcae7429f6e9a, 0x5daeaceb6f2c704b, 0x1abae664523d8df5, 0x1e43658c6d883c4c, 0x4d1605913902d28d, 0x062fad6c2a46c436, 0x65eb200d3589b66b, 0x0278bd462c95ba2b, 0x7f8c273a060c329d, 0x6db2621d457138b5, 0x430846195dd6bf69, 0x5f6fe1703d91a53d, 0x0135cfe127964e52, 0x4d09f64b257d9192, 0x4f63f0736bfdbd04, 0x2e9d2b9f66cd0905, 0x55f979514eb1bb5a, 0x20ef49500c4bd88a, 0x79d414d670b45fb5, 0x29b732423f32aedc, 0x6979731246ea1a67, 0x7c73d0d22fe6dfaf, 0x5c7c99da4f64931f, 0x5570c5bd7eec8e18, 0x445dc6ce5c08c114, 0x39df807d432327da, 0x00b4cd1707660ce7, 0x2efc5b3a194f61ed, 0x3eccf38001ea9cf8, 0x6de859fc7c065185, 0x62d35a8b0e30e3f3, 0x5ce29f4d1c85859b, 0x28d15e2538ccd3dc, 0x298133917dd1e89d, 0x3d04977a22a572fb, 0x698f8d62533865d3, 0x031f4582267e0a8c, 0x75e29dac66035e34, 0x64efec4d5f9bdf5c, 0x3ba4a0714b536369, 0x0e768b43294db31b, 0x30b3c002758420ee, 0x0ed382dc0f2b585a, 0x1115f5525fb01b3c, 0x5bb66cc24da0765c, 0x5bd15a4f7efe4f4e, 0x1b83d4ea3e89c74d, 0x77569b2938b3f99c, 0x3685e23a4455330f, 0x66faa60b20d7ceba, 0x7410348d738a79b4, 0x1a088440508a336d, 0x368d91a2772f7a10, 0x56cb596c0feb21ec, 0x5b3e85551b7d7def, 0x44cb310a126ff9dd, 0x07f41acc69b51098, 0x78e068f2757ef10c, 0x552f0c4916c79da8, 0x6468140409f65e44, 0x08f4ad9330e5790b, 0x6f6f405840396e53, 0x78ed67ef2478827d, 0x68cdb58c66c5db81, },
{0x079daa3b2f734a29, 0x22fdc3dd4fc85b97, 0x20528f057baddec8, 0x72dd58d83d06481e, 0x4cf16a0a56e020a7, 0x725d9e9649a8b245, 0x5c18ac22282fef60, 0x11e4fff83728cfa0, 0x2ca7c0ac640cc6ee, 0x7ad464960ac568eb, 0x14bbc72f01d6ccf5, 0x32bc46005f3c789a, 0x1f75e6ed1db074c2, 0x4228f73f222b8658, 0x08f161d918634edd, 0x47d699062af6accc, 0x7abf0863108f0c14, 0x0c3ceadd6ad45ce4, 0x27daa5021b119768, 0x71f1b80f7f1a43b5, 0x48c2f5fa74cc0f0c, 0x1cfbfe6c644f56a5, 0x1b78264524dba21d, 0x08e8690b2ee0fe65, 0x39a66750481fe6f2, 0x49f6b3e703bccda2, 0x62f9463c4e622e7f, 0x6c12a3427cb2f9e8, 0x1ede8040026f2d2a, 0x1ad27c072e3b9a81, 0x5932474d27cfe21a, 0x385eee2e62a9150d, 0x4d7d71f153f14fb1, 0x6f02e719449bd90b, 0x43b61cc1755816f3, 0x6a24260060f49f29, 0x4543f5ce0c7912bb, 0x3154b4d80720246c, 0x360122d160bc1c14, 0x28dc03063a3d1de4, 0x3df9eb866fa78a21, 0x3e09b8a172d2b6ed, 0x6f85b0d520f331c2, 0x23625eec2a1c5be3, 0x5857f6640e643116, 0x363413303e34daf3, 0x20ddf001318a3db2, 0x057b8d636e93015e, 0x332eda6a6e5b61f2, 0x63b378e6747e747c, 0x557313a576e4f72b, 0x035e09e64dd79ee6, 0x54f7c3521ab70974, 0x7b73258834b2bebf, 0x6eefdca30af8e624, 0x7aa07045244f288e, 0x1721df7b2ce9c829, 0x4ddcf9eb38aa28e6, 0x62c684c906a79051, 0x150bc167713f58d8, 0x2f7433cb3b1e7b2e, 0x6ca8b8e04b3fd497, 0x39d2d5f5505223cc, 0x3ead85bf72244643, },
{0x66a2babf6d01b05f, 0x63e6a78a2260fea5, 0x70389d8b3c15ce65, 0x56ccd7d96744b171, 0x1bf77030453060dd, 0x50294701523ffd61, 0x768f25ef0ae74cd3, 0x37d114fc4ac9b747, 0x0373e02d0db1056a, 0x145895bb05ae0ee7, 0x76ed67bf663a64f7, 0x2158e02529645722, 0x74a42bb926619b8b, 0x76b3bf570e019905, 0x0025df482e7701af, 0x1b78b20e35614516, 0x57c243bb66c89a07, 0x22de686c7f5f5999, 0x66a40b0a47fae146, 0x0d2b422479ab4045, 0x4beb3da6029b7b3a, 0x0d82c80d5d548925, 0x281e406c427a6395, 0x502b69004553dd09, 0x4b01ebf02b92209a, 0x11cc85916483febb, 0x0de855de41ef53b0, 0x6850ef3b332565b6, 0x4126febb028c8197, 0x310383465f04ae92, 0x1465f3a9414cde03, 0x2815780a4c7c3555, 0x4bdb8eee6c283764, 0x342318ab4af3e077, 0x449f20bc327f99f8, 0x351b1532414e5acf, 0x1ea2e3f4108a5e63, 0x5304c1f8429ddd3f, 0x07f1ba4846c12461, 0x725344fb23302af8, 0x07b429b452f3a638, 0x14e2f9e8041fca8c, 0x37453042159c7f92, 0x182901297d33e923, 0x5c3897b6786c2efd, 0x39b90d00492c8470, 0x15a8b9c5709e8b5f, 0x5cc6c2c361ce850a, 0x2cc26581618448b3, 0x1403e2ab10e9db6e, 0x5238363d7161863e, 0x01ebe4a1491ef7dd, 0x0bbcd51c70db1a32, 0x379c3e9354f0a699, 0x7820d19213ae8f64, 0x66a2359c29ef838e, 0x2e0f4e1a7fd4fb46, 0x15717ad87b852f85, 0x78b918a865547e5c, 0x5dc0ad592d9a7c01, 0x76c7007154f1b05e, 0x45903bbd1779ba59, 0x79483f630c6fba36, 0x6df402e92256fe81, },
{0x3340d9ef260aa4e5, 0x176c2b2301f7e594, 0x4b16dd710579102d, 0x76542a5f19580fc4, 0x6e48b65d56d3b28d, 0x6a8241f12df068f2, 0x57dfec80666987ef, 0x663e83355124778e, 0x4ca9a71305ef3a9a, 0x6b43b8f67baffe0d, 0x294a7a0f4562bfbb, 0x1a54701a4904664f, 0x607e20a820117a80, 0x2c8134b75fe4abd7, 0x023baa5859c6600b, 0x7fd104f01a7537a0, 0x1c6d1d35357c8448, 0x3af59475173d3013, 0x30953fd76783faa6, 0x3e57ad343149bed4, 0x5f3a27c61eddf635, 0x05477e2428d9ef25, 0x79fe66b3371a1446, 0x3d094ee06b86015a, 0x6735ff6746a80dc6, 0x0c0acd82284d07d6, 0x71516e2510807976, 0x3091f3f7265f3d9c, 0x0643e97351cf8ecd, 0x2b95eed85d1328ae, 0x7788604e087f93cc, 0x3dfc18142b66f3c9, 0x42a423dc13f57d83, 0x7b79782a78f1ac89, 0x2a3b6b5d733963b4, 0x121759e939d1255e, 0x62ab148309759323, 0x408fa769175ed80d, 0x02e4d9675ca97b37, 0x235188fd7d98f649, 0x25e5fe1f6a1ad8cf, 0x7a9b52452f5c567f, 0x55bb941b17376c44, 0x6906fb112b2d463c, 0x08406eea5bff7d8f, 0x647f115b149ce9e9, 0x4003ddb27fc8cf39, 0x13be4cbc227b296f, 0x1b6cd5f802a8018f, 0x75e165430f37c4e6, 0x4908ea4445a84155, 0x4f1dd47807f8bf2c, 0x1f5797392bb3fec8, 0x085d79ff0fad7be1, 0x0d46722a223c70a1, 0x0c5749702baf02fc, 0x5b0b597c332c7049, 0x4a63dc8d06f29bb5, 0x321fe1f230c6ed97, 0x0cc66b26336ad79e, 0x4807c1873a6050dc, 0x3a29201571457c81, 0x13c0a5f0080b9f3a, 0x0ab3a0c94de76cd2, },
{0x5d1f31b82f2d7be8, 0x74d5bd3d0095060c, 0x088dc53826281bfd, 0x51dc1ac543f391b5, 0x53a10d9627e55c71, 0x4a21cd125a3994c4, 0x05e897c060e77fc0, 0x1413f00956791682, 0x5d6bb23860f3f13c, 0x11baded45e77cc96, 0x11e2a4340f8b942a, 0x49ebe5061e8149fa, 0x0fc6c67c59ea65bc, 0x61f604f60415051c, 0x51fc71ee23876c6c, 0x52b4e8556ca9a5bf, 0x6d3eabcb2f1ba3a6, 0x5543bfa3478aa592, 0x0b7e374875cc7103, 0x1db1cd74271fda68, 0x01596f2c5f1f44de, 0x4006c49f67d39a87, 0x3e4cb109074206ed, 0x6835f829541ab4a8, 0x3292813f1bb86341, 0x2b43f76b79f0d6fd, 0x187220f844752573, 0x1e5f8b2f752fdc72, 0x7944e18e2838e774, 0x4bc053e000559025, 0x6cff35e44b41537c, 0x7a5cf7221e753c35, 0x65ffe1c85a3de1af, 0x500a52b24fa0b6c6, 0x76c0912e717e1910, 0x509d5dee6dbc2027, 0x558fbaae781a005b, 0x7f5c074810a4228d, 0x64bed73613dc6bb7, 0x2f94cef96791ff71, 0x6182d66f17515875, 0x5bc67de85ad8c664, 0x5008a2d679f4f767, 0x222ddedb7a260918, 0x7a7b993d494d8464, 0x148ed7e06dee32bb, 0x0c636ef1677acf22, 0x41b8b0d10eebcf03, 0x5e8c85c9726350b9, 0x63e169c911c30384, 0x7f7f23ab554d16f7, 0x4d671752347ec7b7, 0x4522ea45550ede59, 0x68eb4a104cc31e9a, 0x34551e0c29e1c17b, 0x413319f018801909, 0x7358df6e15d7f47b, 0x0fccebe21cf997d8, 0x171fa0f043618244, 0x0caf06a931facabd, 0x1fe8fd78119b3a2e, 0x79160950213dde89, 0x3029ad8c2c4c6c69, 0x1eafbd223aceba21, },
{0x4c91bda50eb63355, 0x64034a4d029126eb, 0x370feea34c10e150, 0x211fbfa9316a619f, 0x7e2d803a7c32d8e8, 0x26149a630a0b09ba, 0x228b22c332829e46, 0x485a92c16747b453, 0x04414c2b15e40231, 0x5945847658277ea3, 0x0a2249601b60ed1c, 0x2cfc274a65f48b1f, 0x073269a82a0b46d8, 0x5657b3422612309a, 0x60e0eabb375c1735, 0x46124a8a75077064, 0x779897502d72a861, 0x798389b12a1594d7, 0x5b7ff6772ea885f3, 0x2adb5edb1aa3495b, 0x24ae531559ad76b1, 0x0c3014f750eff93e, 0x3837ad91473975d8, 0x5d1d780a548aa7b8, 0x2cb2265b3c78f9bc, 0x57d9e6d83662fc80, 0x1c57879f36d46fbb, 0x60dfb69304d60e22, 0x2ae83ebc2389f147, 0x5ae6087c373769d5, 0x2c3eda3a0bc92978, 0x393bd1d920f85307, 0x4b0de7de23d7718a, 0x527ff77d32bf5b8a, 0x4d62a4e5268dde55, 0x003b55067d5b5658, 0x4e4b4f967210f7fa, 0x394a6dd30c6b69fd, 0x60f611b50682fd27, 0x42fbf6e31667e5dd, 0x4ccae25d0da83810, 0x447ca7cb1ad5ddbc, 0x1fabebde692269fc, 0x0cac5b43255c5e5f, 0x5c93c8344a942a9b, 0x565d5413679263c0, 0x088ab6c708d2a26e, 0x2caa13f80f9925ec, 0x4258817653989ea5, 0x7a267cfb7f2a0b75, 0x7c8561cd0fbb265c, 0x01cc1e567a61d201, 0x06cd3bff4ad0b163, 0x5153ae8a3b168c29, 0x517e720667c34db4, 0x756b85c5144fa56e, 0x2f25832a1e495463, 0x076bbe5f39e82d90, 0x5f448bef4ed16f08, 0x196599a3141819a3, 0x7baa7d633bd85424, 0x44aaf6926fc2edb6, 0x7f5c13a20435342a, 0x57cdd2cf71550a8b, },
{0x707f160041b49519, 0x516fbb7551f44fca, 0x4c5621cc6d0477ce, 0x37d52931533bd9cb, 0x0e5265f553235dcb, 0x3ae6ab7f0928d7bc, 0x1d787d2a5fd0d7fb, 0x7e1a2c5f30523144, 0x6a3a5ed54c9e0054, 0x1b6f6f5c0585eabe, 0x199e0461497eeac4, 0x05573ee834d50900, 0x2497f6b6154881c4, 0x197db5ee4a02357b, 0x3b57400623f40a59, 0x65a89f72714b88be, 0x433fd8882bd65606, 0x18dacdd437185ae7, 0x0a5434b20f95fa54, 0x62b9581f50aff706, 0x59d8cec218a69aa7, 0x787772a31da0039f, 0x4df234e377514bec, 0x43ef4c4076919f02, 0x7c1789c0382c93b8, 0x01ab7e7d5f5ebb9c, 0x1433c49c15b58e22, 0x2afe0fe60702bd65, 0x5104f2e038cbbb53, 0x5cbfc5ac447bc5d5, 0x35c74e930c5c32e6, 0x383288ed4268651e, 0x7980c0057907271b, 0x089d2170510d56c1, 0x21bd4dc703d4f4b7, 0x1c7b8f5f6b56798f, 0x08f67d2e7b961c89, 0x72e7687514f30202, 0x0b84a10456e8b212, 0x0f1545ca36d6b4b5, 0x16357052079c2ac4, 0x1d51b8e610c0c447, 0x17c381ad2a6934ee, 0x6334f041484fc8cd, 0x0ccb8ea268c8748d, 0x7524a7743ff4b5ed, 0x025d1b0b4292dd35, 0x3b9a04502d573061, 0x7e6487227bdddb10, 0x7fb2cfc8443725c0, 0x2f8d9f502021d4ea, 0x1bb7f1731c2e5f27, 0x3121612938841c7e, 0x0f6cce900e9f59e9, 0x45760e9e3ca6022d, 0x44422cf11e82145b, 0x2f42d8a25bab7ef0, 0x0614b3df6193e5d8, 0x29e3aea547065a4f, 0x2fcecedd6949a420, 0x293e5a0e36af3d47, 0x79421a7c24f37651, 0x524aa6b22b9b7519, 0x2779502a34dc1ecc, },
{0x7913448d50af2dd4, 0x70d102be272c1ff2, 0x435a7f1928a0e3dd, 0x6125005b0c88f432, 0x1b284e1b747be042, 0x3121e26f7091ceec, 0x0f13e347609e5cb9, 0x3c49dbaa75640f60, 0x56f7f5383e56bbe9, 0x055d1639425e8f89, 0x2ba833a900dba3dd, 0x378ae124352be516, 0x5a1f5b6754e68db7, 0x008202d130ccfba0, 0x65a91a6d2c6a0219, 0x7d192fed27fb52fb, 0x4f2772ed5ebc5efa, 0x075d42d76dea32ac, 0x7a7326de1281f206, 0x06fdd24868824332, 0x5914121e159b74f9, 0x7639d1b2381fb4b7, 0x2d83c4176827f565, 0x267eb14f3283ad5c, 0x74e23ce5047bb950, 0x05575d2d2bdbc788, 0x6107ac9e208a708f, 0x7570fe463ce23e52, 0x6daf39f23b270805, 0x67910a1e75f30117, 0x1dee54125358545f, 0x3214b35964aa3a0b, 0x52946cb76d15c6ff, 0x7f97b9053971f630, 0x21f439634d079395, 0x62a3088e06958b4d, 0x3eb540047b084b81, 0x633040e758dcda41, 0x0b60879d6c39041c, 0x70b4bd6c09aef236, 0x358ab9be0042c483, 0x20cd3512760c1a99, 0x32ee58eb1692665c, 0x51b96e61163e3358, 0x0c313470209d92de, 0x73f5e73d394a787f, 0x1df4b28a2a1f8882, 0x17354f82260a9a97, 0x5f7c90c770891f42, 0x3d90b2d716cd0887, 0x1d6293d50170ca2a, 0x7c7915ac2033bb66, 0x791095a75c17d3d9, 0x4850d7f55fa95693, 0x695848c904711d44, 0x04b3e1c739059561, 0x2f11affb1ee30287, 0x357568e3258116d9, 0x3bbf4a32620008e6, 0x029d9bc4072ed744, 0x40794fc347f07ea2, 0x7210072476938302, 0x1c9e1d995e6e024d, 0x4ef7218f094556a6, },
{0x20125f2e7c1aae60, 0x7d8b788b0c87d467, 0x2cbb8fcd3d74f303, 0x198cc6dc7a048e37, 0x59ade4ca25cc2574, 0x2a3d42b861dd9ed2, 0x1ae3343343062d93, 0x61e9301a2ef12480, 0x54723b5949f4e42e, 0x2bf4ed15175e98fd, 0x1e8d70411031858b, 0x5822042d2e9288d9, 0x25260bdb5f06c004, 0x3d74c2514a320b52, 0x537761f841c42974, 0x3dded7d50c6be3e1, 0x18f3b8487389c126, 0x30feb4293b6a5060, 0x356ede9745af4815, 0x6b7b6d894a8b7b06, 0x2c6919d80f1cc361, 0x5222f0f415b8b041, 0x44a9d4c1474c4e0b, 0x1141323a340c210e, 0x4b6aba0b191c101b, 0x294d95a63d361f4f, 0x6bc8a82869f82a4c, 0x48feea50016f99d4, 0x4ba1a52610eeb404, 0x52b2dd780673aca1, 0x12df90821f19071e, 0x12a2c8451091b54d, 0x4bfc05ad2bd348ca, 0x718290df43a17c6e, 0x0e2cf774016ae444, 0x1087a7a55cfdfe68, 0x72b6aeaa3a96114c, 0x01e25f5862aa9899, 0x16b6b9a73760836b, 0x507c938613239192, 0x5059b0e1622173b2, 0x4c199dfe79ca292d, 0x7b39c3013c225909, 0x4d110d0d1518884e, 0x1b8c34f046db6827, 0x65f46f451fc3ea86, 0x30559fd32e6bc572, 0x5a3f0e3d7897378a, 0x3c38b3f97c51a581, 0x7dbc89c54bc19f1c, 0x28bf9d854a65ab6d, 0x04fbbcba0e44316b, 0x70eeca041b764c2f, 0x52d6cf9a06de1c12, 0x1a01ada407a583ac, 0x69c6f75e23536321, 0x1d1d8c4e6a5b5e85, 0x267db78e35e0955d, 0x4af91dab18574f4f, 0x5f32b776738ec49c, 0x1352af226685529b, 0x14f1180e452726bb, 0x3dbe5e4643a84ef5, 0x3ff9f4766f30264b, },
{0x3af1c56779f7123f, 0x445cbdac3db67e3c, 0x4bfaafa763b162ec, 0x7f27af1b49587a66, 0x503696783ce979ab, 0x448efd5751fe7eb6, 0x7551e1d76a38441c, 0x5493a2a12e55f4b6, 0x64368a13126f6e25, 0x2ac6bd747b115a30, 0x6ea01ecc2f2fa7be, 0x15b4fa5a09f974ea, 0x4f209ba501f2cdee, 0x459b1ce32aa61268, 0x19d638b30cdef9eb, 0x06d60c2a0595115a, 0x434b8f9654c7fe1a, 0x387961074b32c9d7, 0x148b443d0f463f3d, 0x4c2fb8e837a11022, 0x099f8ed864c1dab6, 0x4efa1ed210beb640, 0x3f14aaf67ef170af, 0x1160ded4238dc174, 0x1e9f1ba4234b3509, 0x527adcc73c279c48, 0x462111320d3f3a70, 0x0f32085e682fd721, 0x12d5e9891541acd8, 0x2220a6c354cd2541, 0x5a62369b2cac223c, 0x0174205728f6b2ee, 0x74297cc51dadc631, 0x2cf4056e39ed815e, 0x718e918008b4c102, 0x6d769bb87923be57, 0x09e274977b2e2059, 0x7a1f91083c70ba8b, 0x5ffe7bff48f71f8d, 0x6c4254960b806fdd, 0x47a80c257e9d97a3, 0x0bdcd2133ebd315d, 0x26ed087f0dc91d58, 0x230aca301b0eda71, 0x6fdbffb239c2f208, 0x666f1445452b70f3, 0x6e2223e14a3e364e, 0x67ebfc7f67e3349c, 0x21d0b5fa624ba0a6, 0x6b0061a914e001ee, 0x0e03c045135f477a, 0x0e8d67d35876fd61, 0x14e7b7ec17e634dc, 0x60dd546908acf8dc, 0x142d68b974e633eb, 0x7383cb8e4d1fa8ff, 0x0bdcda5c5bd574de, 0x699e92367f609da1, 0x1a6f781232c9e2db, 0x6c8cd4e40ca95c66, 0x51d4cd5a0a4b77c5, 0x5489ae1352fbe929, 0x3adf1dc53ff6f13b, 0x224291e23c75aa92, },
{0x5155ac805cafd3bf, 0x700f1b390d42f38b, 0x65b9f0ec5f596cc5, 0x773fa1a17e9c830d, 0x07497be97aa1a8d9, 0x6f87dcc4581cf60a, 0x253c9f091b76e4a2, 0x774c5980630ba853, 0x626c45f431197966, 0x63e35c4160eaebb7, 0x6d94481d7cdbbe07, 0x072735cc50703125, 0x236c1a4e3f691577, 0x7f6006b35bb0e3df, 0x18268e715e4b3813, 0x3afb0bd221a29895, 0x2ee58c20697c3af2, 0x48d5a7b72b0a270c, 0x29a6aa19149f7d0c, 0x0f4125e540154959, 0x18323f6330f02602, 0x4c670aa47ec902aa, 0x61d4aafd3d6ede6d, 0x6e8857d343b36424, 0x249e4fdb4440f0f1, 0x411caef8526bb414, 0x22dbe53a123297f9, 0x519bad704843e4c4, 0x23f4c8a34647ff88, 0x2493379c50fbb423, 0x729e4cb83c1b5715, 0x259792075f8e436e, 0x0a986a7a2183d8d8, 0x362355e56e6d39be, 0x2e828317343f1493, 0x652f3a9545647bca, 0x442d7e7446b4c27b, 0x0423a0e831964539, 0x7549a95e26022971, 0x6a431a6372abf8bb, 0x4517accf19e7f939, 0x2c1a91322b5fc95b, 0x73a3ae2067f39209, 0x2e3b91927db63ea3, 0x4eb1f2c6179876c3, 0x53b3cdd852cec92e, 0x325d0c9c41503f7f, 0x62d41857794b5fdf, 0x67b8999e3cf57717, 0x71348baa18f76e3c, 0x5e5bea07163b1cb5, 0x5cefdf305663c640, 0x07fa0b792289687b, 0x488b91ed61138018, 0x53bf78d37d43b4d7, 0x172bae1132ceac50, 0x5e2e75ab18d725a3, 0x00cab7ac43463f43, 0x40fc7de651d223cb, 0x696a9a8f2f06493e, 0x01d5126c0fae70ad, 0x50feb02c3d1e6867, 0x3669c84734321f09, 0x7127962033d2c883, },
};

#define MOVE_PRIORITY_MAX 0xFFF

#endif