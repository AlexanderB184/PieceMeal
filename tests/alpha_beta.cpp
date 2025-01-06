#include "../include/chess-lib.h"

#define MINSCORE (-INT16_MAX)
#define MAXSCORE (INT16_MAX)

#define PAWN_SCORE 100
#define KNIGHT_SCORE 320
#define BISHOP_SCORE 330
#define ROOK_SCORE 500
#define QUEEN_SCORE 900
#define CHECKMATE_SCORE (-16000)

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

int abSearch(chess_state_t* chess_state, int alpha, int beta, int depth) {
  if (depth <= 0) {
    return eval(chess_state);
  }

  if (is_draw(chess_state)) {
    return 0;
  }

  move_t moves[256];
  size_t move_count =
      generate_legal_moves(chess_state, moves, chess_state->friendly_colour);

  if (move_count == 0) {
    return is_check(chess_state) ? CHECKMATE_SCORE - depth : 0;
  }

  for (size_t i = 0; i < move_count; i++) {
    make_move(chess_state, moves[i]);
    int score = -abSearch(chess_state, -beta, -alpha, depth - 1);
    unmake_move(chess_state);
    if (score > beta) {
      return beta;
    }

    if (score > alpha) {
      alpha = score;
    }
  }

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
  chess_state_t chess_state = {};
  const char* fen =
      "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1";
  int depth = 1;
  if (argc > 1) {
    depth = atoi(argv[1]);
  }
  if (argc > 2) {
    fen = argv[2];
  }

  load_position(&chess_state, fen);
  move_t move = root_search(&chess_state, depth);
  char buffer[16];
  write_algebraic_notation(buffer, 16, &chess_state, move);
  printf("In position %s the best move at depth %d was found to be %s.\n", fen,
         depth, buffer);
  release_position(&chess_state);
  return 0;
}
