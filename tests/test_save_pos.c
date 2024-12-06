#include <stdio.h>
#include <assert.h>
#include <string.h>

#define PRINT_WRITE_ERRORS
#define PRINT_READ_ERRORS

#include "../include/chess-lib.h"

static chess_state_t chess_state;
static chess_state_t other;

int quiet = 0;

void test_recursive(int N) {
  char buffer[1024];

  save_position(&chess_state, buffer, sizeof(buffer));
  if (!quiet) printf("%s\n",buffer);

  load_position(&other, buffer);
  assert(chess_state.zobrist == other.zobrist);
  if (N == 0) return;
  move_t moves[256];

  size_t move_count = generate_legal_moves(&chess_state, moves);
  for (size_t i = 0; i < move_count; i++) {

    make_move(&chess_state, moves[i]);
    test_recursive(N - 1);


    unmake_move(&chess_state);
  }
}

void test(const char* fen_string) {
  static int testno = 0;
  testno++;
  load_position(&chess_state, fen_string);
  test_recursive(3);
  printf("completed test %d!!!\n", testno);
}

int main(int argc, char const* argv[]) {
  if (argc > 1 && strcmp(argv[1], "quiet") == 0) {
    quiet = 1;
  }
  test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  test("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
  test("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  return 0;
}
