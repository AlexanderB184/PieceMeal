/*
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/chess-lib.h"
#include "../include/private/chess-lib-internals.h"

void clear_position(chess_state_t* chess_state) {
  if (chess_state->ply_stack) {
    free(chess_state->ply_stack);
  }
  memset(chess_state, 0, sizeof(*chess_state));
  memcpy(chess_state->board, empty_board, sizeof(empty_board));

  chess_state->friendly_colour = WHITE;
  chess_state->enemy_colour = BLACK;
  chess_state->friendly_pieces = &chess_state->white_pieces;
  chess_state->enemy_pieces = &chess_state->black_pieces;
  chess_state->up_increment = 16, chess_state->down_increment = -16;
}

void init_ply_stack(chess_state_t* chess_state) {
  chess_state->ply_stack_capacity = chess_state->ply_counter + 50ull;
  chess_state->ply_stack =
      malloc(sizeof(ply_stack_item_t) * chess_state->ply_stack_capacity);
}

void load_start_position(chess_state_t* chess_state) {
  load_position(chess_state,
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

void copy_position(chess_state_t* chess_state,
                   const chess_state_t* state_to_copy) {
  if (chess_state->ply_stack) {
    free(chess_state->ply_stack);
  }
  memcpy(chess_state, state_to_copy, sizeof(chess_state_t));
  chess_state->ply_stack =
      malloc(sizeof(ply_stack_item_t) * chess_state->ply_stack_capacity);
  memcpy(chess_state->ply_stack, state_to_copy->ply_stack,
         sizeof(ply_stack_item_t) * chess_state->ply_stack_capacity);
  if (chess_state->black_to_move) {
    chess_state->friendly_pieces = &chess_state->black_pieces;
    chess_state->enemy_pieces = &chess_state->white_pieces;
  } else {
    chess_state->friendly_pieces = &chess_state->white_pieces;
    chess_state->enemy_pieces = &chess_state->black_pieces;
  }
}

piece_t parse_piece(char letter) {
  switch (letter) {
    case 'K':
      return WHITE_KING;
    case 'Q':
      return WHITE_QUEEN;
    case 'R':
      return WHITE_ROOK;
    case 'B':
      return WHITE_BISHOP;
    case 'N':
      return WHITE_KNIGHT;
    case 'P':
      return WHITE_PAWN;
    case 'k':
      return BLACK_KING;
    case 'q':
      return BLACK_QUEEN;
    case 'r':
      return BLACK_ROOK;
    case 'b':
      return BLACK_BISHOP;
    case 'n':
      return BLACK_KNIGHT;
    case 'p':
      return BLACK_PAWN;
    default:
      return EMPTY;
  }
}

sq0x88_t parse_square(const char* s) {
  if (*s < 'a' || *s > 'h') return -1;
  sq0x88_t sq = *s - 'a';
  s++;
  if (*s < '1' || *s > '8') return -1;
  sq += (*s - '1') * 16;
  return sq;
}

int parse_row(chess_state_t* chess_state, const char* fen_string, size_t* index,
              size_t rank) {
  size_t file = 0;
  while (file < 8) {
    char letter = fen_string[*index];

    if (letter >= '1' && letter <= '8') {
      file += letter - '0';  // empty

      if (fen_string[++*index] == '\0') {
        return 1;
      }

      continue;
    }

    piece_t piece_to_add = parse_piece(letter);

    if (piece_to_add == EMPTY) {
      return 1;
    }

    place_piece(chess_state, rank + file, piece_to_add);

    file++;
    if (fen_string[++*index] == '\0') {
      return 1;
    }
  }
  return file != 8;  // error if file is not exactly 8
}

#define FEN_PARSE_ERROR()                                                  \
  fprintf(stderr,                                                          \
          "ERROR: failed to parse FEN string \"%s\".\n Error occurred at " \
          "byte offset "                                                   \
          "%zu\n",                                                         \
          fen_string != NULL ? fen_string : "NULL", index);                \
  return NULL;

#define FEN_PARSE_ASSERT(cond) \
  if (!(cond)) {               \
    FEN_PARSE_ERROR();         \
  }

#define FEN_PARSE_NEXT(string, index) \
  FEN_PARSE_ASSERT((string)[++(index)] != 0);

#define FEN_PARSE_EXPECT(string, index, token)    \
  FEN_PARSE_ASSERT((string)[(index)] == (token)); \
  FEN_PARSE_NEXT((string), (index));

const char* load_position(chess_state_t* chess_state, const char* fen_string) {
  size_t index = 0;

  FEN_PARSE_ASSERT(fen_string != NULL);

  clear_position(chess_state);

  // pieces

  for (int rank = 7 * 16; rank >= 0; rank -= 16) {
    int error = parse_row(chess_state, fen_string, &index, rank);
    FEN_PARSE_ASSERT(error == 0);
    if (rank != 0) {
      FEN_PARSE_EXPECT(fen_string, index, '/');
    }
  }
  FEN_PARSE_EXPECT(fen_string, index, ' ');  // skip space

  // next to move

  switch (fen_string[index]) {
    case 'w':
      break;
    case 'b':
      update_turn(chess_state);
      break;
    default:
      FEN_PARSE_ERROR();
  }
  FEN_PARSE_NEXT(fen_string, index);

  FEN_PARSE_EXPECT(fen_string, index, ' ');  // skip space

  // castle rights

  if (fen_string[index] == '-') {
    FEN_PARSE_NEXT(fen_string, index);
  } else {
    if (fen_string[index] == 'K') {
      chess_state->castle_rights |= WHITE_KING_SIDE;
      FEN_PARSE_NEXT(fen_string, index);
    }
    if (fen_string[index] == 'Q') {
      chess_state->castle_rights |= WHITE_QUEEN_SIDE;
      FEN_PARSE_NEXT(fen_string, index);
    }
    if (fen_string[index] == 'k') {
      chess_state->castle_rights |= BLACK_KING_SIDE;
      FEN_PARSE_NEXT(fen_string, index);
    }
    if (fen_string[index] == 'q') {
      chess_state->castle_rights |= BLACK_QUEEN_SIDE;
      FEN_PARSE_NEXT(fen_string, index);
    }
  }

  FEN_PARSE_EXPECT(fen_string, index, ' ');  // skip space

  // en passent

  if (fen_string[index] == '-') {
    chess_state->enpassent_target = -1;

    FEN_PARSE_NEXT(fen_string, index);
  } else {
    FEN_PARSE_ASSERT(fen_string[index] >= 'a' && fen_string[index] <= 'h');
    chess_state->enpassent_target = (sq0x88_t)(fen_string[index] - 'a');
    FEN_PARSE_NEXT(fen_string, index);
    FEN_PARSE_ASSERT(fen_string[index] == '3' || fen_string[index] == '6');
    if (fen_string[index] == '3') {
      chess_state->enpassent_target += (sq0x88_t)32;  // rank 3
    } else {
      chess_state->enpassent_target += (sq0x88_t)80;  // rank 6
    }
    FEN_PARSE_NEXT(fen_string, index);
  }

  FEN_PARSE_EXPECT(fen_string, index, ' ');  // skip space

  // half move clock

  FEN_PARSE_ASSERT(fen_string[index] >= '0' && fen_string[index] <= '9');
  int half_move_clock = fen_string[index] - '0';
  FEN_PARSE_NEXT(fen_string, index);
  if (fen_string[index] >= '0' && fen_string[index] <= '9') {
    half_move_clock = 10 * half_move_clock + fen_string[index] - '0';
    FEN_PARSE_NEXT(fen_string, index);
  }
  chess_state->half_move_clock = half_move_clock;

  FEN_PARSE_EXPECT(fen_string, index, ' ');  // skip space

  // full move counter
  FEN_PARSE_ASSERT(fen_string[index] >= '1' && fen_string[index] <= '9');
  int full_move_counter = fen_string[index] - '0';
  index++;
  while (fen_string[index] >= '0' && fen_string[index] <= '9') {
    FEN_PARSE_ASSERT(fen_string[index] >= '0' && fen_string[index] <= '9');
    full_move_counter = 10 * full_move_counter + fen_string[index] - '0';
    index++;
  }
  chess_state->ply_counter =
      (full_move_counter - 1) * 2 + chess_state->black_to_move;
  chess_state->ply_of_last_irreversible_move = chess_state->ply_counter;

  init_check(chess_state);
  init_ply_stack(chess_state);
  return fen_string + index;
}
*/