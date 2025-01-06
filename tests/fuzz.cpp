#define CATCH_CONFIG_MAIN
#include <stdio.h>

#include <catch2/catch_test_macros.hpp>

#include "../include/chess.h"

void testAlgN(chess_state_t* chess_state) {
  move_t moves[256];
  size_t move_count =
      generate_legal_moves(chess_state, moves, chess_state->friendly_colour);
  for (size_t i = 0; i < move_count; i++) {
    int n = 10;
    char buffer[16];
    memset(buffer + n, 0x48, 16-n);
    long len = write_algebraic_notation(buffer, n, chess_state, moves[i]);
    for (int i = n; i < 16; i++) {
      if (buffer[i] != 0x48) {
        printf("buffer overflow detected");
        return;
      }
    }
    if (len == -1) continue;
    CHECK(len > 0);
    if (len <= 0) {
      continue;
    }
    move_t out;
    long rout = read_algebraic_notation(buffer, len, chess_state, &out);
    CHECK(len == rout);
    CHECK(compare_moves(moves[i], out));
  }
}

void testLongAlgN(chess_state_t* chess_state) {
  move_t moves[256];
  size_t move_count =
      generate_legal_moves(chess_state, moves, chess_state->friendly_colour);
  for (size_t i = 0; i < move_count; i++) {
    int n = 8;
    char buffer[16];
    memset(buffer + n, 0x48, 16-n);
    long len = write_long_algebraic_notation(buffer, n, moves[i]);
    for (int i = n; i < 16; i++) {
      if (buffer[i] != 0x48) {
        printf("buffer overflow detected");
        return;
      }
    }
    if (len == -1) continue;
    CHECK(len > 0);
    if (len <= 0) {
      continue;
    }
    buffer[len] = '\0';
    move_t out;
    long rout = read_long_algebraic_notation(buffer, len, chess_state, &out);
    CHECK(len == rout);
    /*if (!compare_moves(moves[i], out)) {
      char fbuf[3];
      char tbuf[3];
      write_square(fbuf, 3, moves[i].from);
      write_square(tbuf, 3, moves[i].to);
      //(sq0x88_t)(out.to - out.from) ==
    pawn_push_increment(chess_state->friendly_colour) * 2;
      //fprintf(stderr, "%s %s %s %d %d %d %d\n", buffer, fbuf, tbuf,
    moves[i].priority_and_flags, out.priority_and_flags, (sq0x88_t)(out.to -
    out.from), (sq0x88_t)(pawn_push_increment(chess_state->friendly_colour) *
    (sq0x88_t)2));
    }*/
    CHECK(compare_moves(moves[i], out));
  }
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
    testAlgN(&chess_state);
  } while (res != NULL);
  fclose(f);
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
    testLongAlgN(&chess_state);
  } while (res != NULL);
  fclose(f);
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
    long loadlen = load_position(&chess_state, buf);
    int n = 64;
    char buf2[128];
    memset(buf2 + n, 0x11, 128-n);
    long savelen = save_position(&chess_state, buf2, n);
    for (int i = n; i < 128; i++) {
      if (buf2[i] != 0x11) {
        printf("buffer overflow detected");
        return;
      }
    }
    if (savelen < 0) continue;
    CHECK(savelen > 0);
    CHECK(loadlen == savelen);
    CHECK(strncmp(buf, buf2, loadlen) == 0);
    if (strncmp(buf, buf2, loadlen) != 0) {
      buf[loadlen] = 0;
      buf2[savelen] = 0;
      printf("%s -> %s\n", buf, buf2);
    }
  } while (res != NULL);
  fclose(f);
}

//r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1
//r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 