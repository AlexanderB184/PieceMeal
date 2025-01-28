#include "../include/chess.h"

#define fallthrough __attribute__((fallthrough))

int eval(const chess_state_t* chess_state) {
  if (is_draw(chess_state)) {
    return 0;
  }
  if (is_stalemate(chess_state)) {
    return 0;
  }
  if (is_checkmate(chess_state)) {
    return CHECKMATE_SCORE;
  }
  return (chess_state->black_to_move ? -1 : 1) *
         (chess_state->white_pieces.pawn_count * PAWN_SCORE +
          chess_state->white_pieces.knight_count * KNIGHT_SCORE +
          chess_state->white_pieces.dark_bishop_count * BISHOP_SCORE +
          chess_state->white_pieces.light_bishop_count * BISHOP_SCORE +
          chess_state->white_pieces.rook_count * ROOK_SCORE +
          chess_state->white_pieces.queen_count * QUEEN_SCORE +
          -chess_state->black_pieces.pawn_count * PAWN_SCORE +
          -chess_state->black_pieces.knight_count * KNIGHT_SCORE +
          -chess_state->black_pieces.dark_bishop_count * BISHOP_SCORE +
          -chess_state->black_pieces.light_bishop_count * BISHOP_SCORE +
          -chess_state->black_pieces.rook_count * ROOK_SCORE +
          -chess_state->black_pieces.queen_count * QUEEN_SCORE);
}

typedef struct movelist_t {
  move_t moves[256];
  int count;
  int stage;
} movelist_t;

move_t next_move(chess_state_t* chess_state, movelist_t* list) {
  move_t selected_move = null_move;
  do {
    switch (list->stage) {
      case 0: {
        list->count = generate_promotions(chess_state, list->moves,
                                          chess_state->friendly_colour);
        list->stage = 1;
        
      }
      fallthrough;
      case 1: {
        if (list->count > 0) break;
        list->count = generate_captures(chess_state, list->moves,
                                        chess_state->friendly_colour);
        list->stage = 2;
      }
      fallthrough;
      case 2: {
        if (list->count > 0) break;
        list->count = generate_quiets(chess_state, list->moves,
                                      chess_state->friendly_colour);
        list->stage = 3;
      }
      fallthrough;
      case 3:
        if (list->count > 0) break;
        return null_move;
      default:
        return null_move;
    }
    selected_move = list->moves[--list->count];
  } while (!is_legal(chess_state, selected_move));
  return selected_move;
}

int abSearch(chess_state_t* chess_state, int alpha, int beta, int depth) {
  if (depth <= 0) {
    return eval(chess_state);
  }

  if (is_draw(chess_state)) {
    return 0;
  }
  movelist_t list = {};

  move_t move = next_move(chess_state, &list);

  if (is_null_move(move)) {
    return is_check(chess_state) ? CHECKMATE_SCORE - depth : 0;
  }

  do {
    make_move(chess_state, move);
    int score = -abSearch(chess_state, -beta, -alpha, depth - 1);
    unmake_move(chess_state);
    if (score > beta) {
      return beta;
    }

    if (score > alpha) {
      alpha = score;
    }
  } while (!is_null_move(move = next_move(chess_state, &list)));

  return alpha;
}

move_t root_search(chess_state_t* chess_state, int depth) {
  if (is_gameover(chess_state)) {
    return null_move;
  }
  move_t moves[256];
  size_t move_count =
      generate_legal_moves(chess_state, moves, chess_state->friendly_colour);
  int best_score = MINSCORE;
  move_t best_move = null_move;
  for (size_t i = 0; i < move_count; i++) {
    make_move(chess_state, moves[i]);
    int score = -abSearch(chess_state, MINSCORE, -best_score, depth);
    unmake_move(chess_state);
    if (score > best_score) {
      best_score = score;
      best_move = moves[i];
    }
  }

  return best_move;
}

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* argv[]) {
  (void)argc;
  (void)argv;
  int depth = 4;
  chess_state_t chess_state = {};
  FILE* f = fopen("../tests/test_positions.fen", "r");
  char* res;
  if (f == NULL) {
    perror("fopen");
    exit(1);
  }
  do {
    char fen[128];
    res = fgets(fen, 128, f);
    if (res == NULL) {
      if (feof(f)) {
        break;
      }
      perror("fgets");
      exit(1);
    }
    printf("%s", res);

    load_position(&chess_state, fen);
    move_t move = root_search(&chess_state, depth);
    char buffer[16];
    write_algebraic_notation(buffer, 16, &chess_state, move);
    printf("In position %s the best move at depth %d was found to be %s.\n",
           fen, depth, buffer);

  } while (1);

  release_position(&chess_state);
  fclose(f);

  exit(0);
}
