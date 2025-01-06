#define CATCH_CONFIG_MAIN
#include <stdio.h>

#include <catch2/catch_test_macros.hpp>

#include "../include/chess.h"

int check_duplicates(move_t* moves, size_t move_count) {
  for (size_t i = 0; i < move_count; i++) {
    for (size_t j = i + 1; j < move_count; j++) {
      if (compare_moves(moves[i], moves[j])) return 0;
    }
  }
  return 1;
}

int check_diff(move_t* moves1, size_t move_count1, move_t* moves2,
               size_t move_count2) {
                int fail = 0;
  for (size_t i = 0; i < move_count1; i++) {
    int found_match = 0;
    for (size_t j = 0; j < move_count2; j++) {
      if (compare_moves(moves1[i], moves2[j])) {
        found_match = 1;
        break;
      }
    }
    if (!found_match) {
      char buffer[16];
      write_long_algebraic_notation(buffer, 16, moves1[i]);
      printf("%s\n", buffer);
      fail = 1;
    }
  }
  return fail;
}

size_t filter_captures(move_t* moves, size_t move_count) {
  for (size_t i = 0; i < move_count; i++) {
    if (!is_capture(moves[i]) || is_promotion(moves[i])) {
      moves[i--] = moves[--move_count];
    }
  }
  return move_count;
}

size_t filter_quiets(move_t* moves, size_t move_count) {
  for (size_t i = 0; i < move_count; i++) {
    if (is_capture(moves[i]) || is_promotion(moves[i])) {
      moves[i--] = moves[--move_count];
    }
  }
  return move_count;
}

size_t filter_promotions(move_t* moves, size_t move_count) {
  for (size_t i = 0; i < move_count; i++) {
    if (!is_promotion(moves[i])) {
      moves[i--] = moves[--move_count];
    }
  }
  return move_count;
}

void test(chess_state_t* chess_state) {
  move_t m1[256];
  move_t m2[256];
  move_t m3[256];
  move_t m4[256];
  size_t c1 = generate_legal_moves(chess_state, m1, WHITE);
  size_t c2 = generate_legal_captures(chess_state, m2, WHITE);
  size_t c3 = generate_legal_quiets(chess_state, m3, WHITE);
  size_t c4 = generate_legal_promotions(chess_state, m4, WHITE);
  CHECK(c1 == c2 + c3 + c4);
  c1 = filter_captures(m1, c1);
  CHECK(!check_diff(m1, c1, m2, c2));
  c1 = generate_legal_moves(chess_state, m1, WHITE);
  c1 = filter_quiets(m1, c1);
  CHECK(!check_diff(m1, c1, m3, c3));
  c1 = generate_legal_moves(chess_state, m1, WHITE);
  c1 = filter_promotions(m1, c1);
  CHECK(!check_diff(m1, c1, m4, c4));
}

TEST_CASE() {
  FILE* f = fopen("../tests/test_positions.fen", "r");
  if (f == NULL) {
    perror("fopen");
    return;
  }
  chess_state_t chess_state = {};
  char* res;
  do {
    char buf[128];
    res = fgets(buf, 128, f);
    if (res == NULL) break;
    printf("%s", res);
    load_position(&chess_state, buf);
    test(&chess_state);
  } while (res != NULL);
  fclose(f);
}