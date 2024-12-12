#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>


#include <stdio.h>

#include "../include/chess-lib.h"

const int valid_flags[] = {
    QUIET_MOVE,
    DOUBLE_PAWN_PUSH,
    QUEEN_CASTLE,
    KING_CASTLE,
    CAPTURE,
    ENPASSENT,
    KNIGHT_PROMOTION,
    BISHOP_PROMOTION,
    ROOK_PROMOTION,
    QUEEN_PROMOTION,
    KNIGHT_CAPTURE_PROMOTION,
    BISHOP_CAPTURE_PROMOTION,
    ROOK_CAPTURE_PROMOTION,
    QUEEN_CAPTURE_PROMOTION,
};

size_t gen_moves(const chess_state_t* position, move_t* moves) {
  size_t count = 0;
  for (int from = 0; from < 64; from++) {
    for (int to = 0; to < 64; to++) {
      for (int flags = 0; flags < 14; flags++) {
        move_t m = move(sq8x8_to_sq0x88(from), sq8x8_to_sq0x88(to),
                        valid_flags[flags]);
        if (is_pseudo_legal(position, m)) {
          moves[count++] = m;
        }
      }
    }
  }
  return count;
}

size_t find_diff(move_t* m1, size_t c1, move_t* m2, size_t c2, move_t* diff) {
  size_t c3 = 0;
  for (int i = 0; i < (int)c1; i++) {
    int found = 0;
    for (int j = 0; j < (int)c2; j++) {
      if (compare_moves(m1[i], m2[j])) {
        found = 1;
        break;
      }
    }
    if (!found) {
      diff[c3++] = m1[i];
    }
  }
  return c3;
}

void recursive_test(chess_state_t* position, int depth) {
  if (depth == 0) return;
  move_t moves[256];
  move_t moves2[256];
  size_t count = gen_moves(position, moves);
  size_t count2 = generate_moves(position, moves2);
  CHECK(count == count2);
  
  if (count != count2) {
    char buffer[1000];
    write_movetext_debug(buffer, 2000, position);
    printf("%s\n", buffer);
    printf("normally generated: %ld, with is_pseudo_legal: %ld\n", count2,
           count);
    move_t diff[256];
    size_t count3 = find_diff(moves, count, moves2, count2, diff);
    printf("\nipl not in normal: ");
    for (int i = 0; i < (int)count3; i++) {
      char buffer[16];
      write_piece(buffer, 16, piece(position, diff[i].from));
      write_long_algebraic_notation(buffer + 1, 15, diff[i]);
      printf("%s ", buffer);
    }
    count3 = find_diff(moves2, count2, moves, count, diff);
    printf("\nnormal not in ipl: ");
    for (int i = 0; i < (int)count3; i++) {
      char buffer[16];
      write_piece(buffer, 16, piece(position, diff[i].from));
      write_long_algebraic_notation(buffer + 1, 15, diff[i]);
      printf("%s ", buffer);
    }
    printf("\n");
  } else {
    for (int i = 0; i < (int)count2; i++) {
      if (!is_legal(position, moves2[i])) continue;
      make_move(position, moves2[i]);
      recursive_test(position, depth - 1);
      unmake_move(position);
    }
  }
}

int main() {
  chess_state_t position = {};
  load_position(&position,
                "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  recursive_test(&position, 5);
  return 0;
}
