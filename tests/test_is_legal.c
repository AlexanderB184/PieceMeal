#include <stdio.h>

#include "../include/chess-lib.h"

static chess_state_t chess_state;

void test(const char* position, move_t move, int expected_result) {
    static int testno = 0;
    testno++;
  load_position(&chess_state, position);
  int legality = is_legal(&chess_state, move);
  if (legality == expected_result) {
    printf("passed %d\n", testno);
  } else {
    printf("failed %d: expected: %d got: %d\n", testno, expected_result, legality);
  }
}

int main() {
  test("3r4/8/7k/8/8/3P4/8/3K4 w - - 0 1", move(d3, d4, QUIET_MOVE), 1);
  test("3r4/8/7k/8/8/3P4/4K3/8 w - - 0 1", move(d3, d4, QUIET_MOVE), 1);
  test("3r4/8/7k/8/8/3PK3/8/8 w - - 0 1", move(e3, d4, QUIET_MOVE), 0);
  test("8/8/7k/8/8/r2PK3/8/8 w - - 0 1", move(d3, d4, QUIET_MOVE), 0);
  test("8/8/7k/8/8/r2PK3/8/8 w - - 0 1", move(e3, d4, QUIET_MOVE), 1);
  test("8/8/7k/8/8/b2PK3/8/8 w - - 0 1", move(d3, d4, QUIET_MOVE), 1);
  test("8/8/7k/8/8/b2PK3/8/8 w - - 0 1", move(e3, d4, QUIET_MOVE), 1);
  test("8/8/7k/8/2b5/3P4/4K3/8 w - - 0 1", move(d3, d4, QUIET_MOVE), 0);
  test("8/8/7k/8/2b5/3P4/4K3/8 w - - 0 1", move(d3, c4, CAPTURE), 1);
  test("8/8/7k/1b6/2p5/3P4/4K3/8 w - - 0 1", move(d3, c4, CAPTURE), 1);
  test("8/8/7k/1b6/2p5/3P4/4K3/8 w - - 0 1", move(d3, d4, QUIET_MOVE), 1);
  test("8/8/7k/1b6/8/2pP4/4K3/8 w - - 0 1", move(e2, d2, QUIET_MOVE), 0);
  test("8/4r3/7k/1b2N3/8/3P4/4K3/8 w - - 0 1", move(e5, f3, QUIET_MOVE), 0);
  test("8/4r3/7k/4N2b/8/3P4/4K3/8 w - - 0 1", move(e5, f3, QUIET_MOVE), 0);
  test("8/4r3/7k/7b/8/3P1N2/4K3/8 w - - 0 1", move(e2, d2, QUIET_MOVE), 1);
  test("8/4r3/7k/7b/8/3P1N2/4K3/8 w - - 0 1", move(e2, e1, QUIET_MOVE), 0);
  test("8/8/7k/7b/8/3PrN2/4K3/8 w - - 0 1", move(e2, e3, CAPTURE), 1);
  test("8/8/5r1k/7b/8/3P4/3N4/R3K2R w KQ - 0 1", move(e1, c1, QUEEN_CASTLE), 0);
  test("8/8/5r1k/7b/8/3P4/3N4/R3K2R w KQ - 0 1", move(e1, g1, KING_CASTLE), 0);
  test("7k/8/6rb/8/8/3P4/8/R3K2R w KQ - 0 1", move(e1, c1, QUEEN_CASTLE), 0);
  test("7k/8/6rb/8/8/3P4/8/R3K2R w KQ - 0 1", move(e1, g1, KING_CASTLE), 0);
  test("7k/8/6r1/6b1/8/3P4/8/R3K2R w KQ - 0 1", move(e1, g1, KING_CASTLE), 1);
  test("7k/8/6r1/6b1/8/3P2n1/8/R3K2R w KQ - 0 1", move(e1, g1, KING_CASTLE), 0);
  test("8/8/7k/3Pp3/8/8/8/4K3 w - e6 0 2", move(d5, e6, ENPASSENT), 1);
  test("3r4/8/7k/3Pp3/8/8/8/3K4 w - e6 0 2", move(d5, e6, ENPASSENT), 0);
  test("3r2b1/8/7k/3Pp3/8/1K6/8/8 w - e6 0 2", move(d5, e6, ENPASSENT), 1);
  test("3r2b1/8/7k/1K1Pp2q/8/8/8/8 w - e6 0 2", move(d5, e6, ENPASSENT), 0);
  test("3r2b1/8/7k/2qPpK2/8/8/8/8 w - e6 0 2", move(d5, e6, ENPASSENT), 0);
  test("1b1r4/8/7k/3Pp3/8/6K1/8/8 w - e6 0 2", move(d5, e6, ENPASSENT), 0);
  test("b2r4/8/7k/3Pp3/8/8/6K1/8 w - e6 0 2", move(d5, e6, ENPASSENT), 0);
  test("b2r4/8/7k/3Pp3/6p1/8/6K1/8 w - e6 0 2", move(g2, g3, QUIET_MOVE), 1);
  test("b2r4/8/7k/3Pp3/6p1/8/6K1/8 w - e6 0 2", move(g2, h3, QUIET_MOVE), 0);
  test("b2r4/8/7k/3Pp3/6p1/8/6K1/8 w - e6 0 2", move(g2, f3, QUIET_MOVE), 0);
  test("b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2", move(g2, f3, QUIET_MOVE), 0);
  test("b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2", move(g2, g3, QUIET_MOVE), 0);
  test("b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2", move(g2, h3, QUIET_MOVE), 0);
  test("b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2", move(g2, h2, QUIET_MOVE), 1);
  test("b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2", move(g2, h1, QUIET_MOVE), 0);
  test("b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2", move(g2, f2, QUIET_MOVE), 1);
  test("b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2", move(g2, f1, QUIET_MOVE), 1);
  test("b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2", move(g2, g1, QUIET_MOVE), 1);
  return 0;
}
