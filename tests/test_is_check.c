#include <stdio.h>

#include "../include/chess-lib.h"

static chess_state_t chess_state;

void test(const char* fen, move_t move, int expected_check,
          int expected_discover) {
  static int testno = 0;
  testno++;
  load_position(&chess_state, fen);
  make_move(&chess_state, move);
  int check = chess_state.n_checks;
  if (check == expected_check &&
      chess_state.discovered_check == expected_discover) {
    printf("passed %d\n", testno);
  } else {
    printf(
        "failed %d: expected checks: %d got: %d expected discover: %d got: "
        "%d\n",
        testno, expected_check, check, expected_discover,
        chess_state.discovered_check);
  }
}

int main() {
  test("8/4k3/8/8/8/4K3/8/8 w - - 0 1", move(e3, e4, QUIET_MOVE), 0, 0);
  test("8/4k3/8/8/8/2N1K3/8/8 w - - 0 1", move(c3, d5, QUIET_MOVE), 1, 0);
  test("8/4k3/8/8/8/2N1K1B1/8/8 w - - 0 1", move(g3, h4, QUIET_MOVE), 1, 0);
  test("8/4k3/8/8/8/2N1K1B1/8/1R6 w - - 0 1", move(b1, b7, QUIET_MOVE), 1, 0);
  test("8/4k3/8/5P2/8/2N1K1B1/8/1R6 w - - 0 1", move(f5, f6, QUIET_MOVE), 1, 0);
  test("8/4k3/8/5P2/8/4K3/8/4R3 w - - 0 1", move(e3, e4, QUIET_MOVE), 0, 0);
  test("8/4k3/8/5P2/8/4K3/8/4R3 w - - 0 1", move(e3, f3, QUIET_MOVE), 1, 1);
  test("8/4k3/8/4P3/8/4K3/8/4R3 w - - 0 1", move(e3, f3, QUIET_MOVE), 0, 0);
  test("8/4k3/8/6P1/8/4BK2/8/4R3 w - - 0 1", move(e3, c5, QUIET_MOVE), 2, 1);
  test("5k2/8/8/8/8/8/8/R3K2R w K - 0 1", move(e1, g1, KING_CASTLE), 1, 0);
  test("5k2/8/8/8/8/8/8/R3K2R w K - 0 1", move(e1, c1, QUEEN_CASTLE), 0, 0);
  test("3k4/8/8/8/8/8/8/R3K2R w K - 0 1", move(e1, c1, QUEEN_CASTLE), 1, 0);
  test("8/8/8/4k3/8/8/5P2/4K3 w - - 0 1", move(f2, f4, DOUBLE_PAWN_PUSH), 1, 0);
  test("8/5k2/8/3Pp3/8/8/8/4K3 w - e6 0 1", move(d5, e6, ENPASSENT), 1, 0);
  test("8/2k5/8/3Pp3/8/8/7B/4K3 w - e6 0 1", move(d5, e6, ENPASSENT), 1, 1);
  test("8/8/8/1k1Pp2R/8/8/8/4K3 w - e6 0 1", move(d5, e6, ENPASSENT), 1, 1);
  test("8/8/8/1R1Pp2k/8/8/8/4K3 w - e6 0 1", move(d5, e6, ENPASSENT), 1, 1);
  test("8/6k1/8/3Pp3/8/8/1B6/4K3 w - e6 0 1", move(d5, e6, ENPASSENT), 1, 1);
  test("8/2p5/3p4/1K6/1R3p1k/8/4P1P1/8 b - - 0 9", move(c7, c6, QUIET_MOVE), 1,
       0);
  test("7k/4P3/8/1K1p4/8/8/8/8 w - - 0 1", move(e7, e8, QUEEN_PROMOTION), 1, 0);
  test("8/4P1k1/8/1K1p4/8/8/8/8 w - - 0 1", move(e7, e8, KNIGHT_PROMOTION), 1,
       0);

  test("8/4K3/8/8/8/4k3/8/8 b - - 0 1", move(e3, e4, QUIET_MOVE), 0, 0);
  test("8/4K3/8/8/8/2n1k3/8/8 b - - 0 1", move(c3, d5, QUIET_MOVE), 1, 0);
  test("8/4K3/8/8/8/2n1k1b1/8/8 b - - 0 1", move(g3, h4, QUIET_MOVE), 1, 0);
  test("8/4K3/8/8/8/2n1k1b1/8/1r6 b - - 0 1", move(b1, b7, QUIET_MOVE), 1, 0);

  test("7k/4p3/8/8/3K4/8/8/8 b - - 0 1", move(e7, e6, QUIET_MOVE), 0, 0);
  test("7k/4p3/8/8/3K4/8/8/8 b - - 0 1", move(e7, e5, DOUBLE_PAWN_PUSH), 1, 0);
  test("7k/4p3/3P4/4K3/8/8/8/8 b - - 0 1", move(e7, d6, CAPTURE), 1, 0);
  test("4r2k/4p3/3P4/4K3/8/8/8/8 b - - 0 1", move(e7, d6, CAPTURE), 2, 1);

  test("8/4k3/8/3P4/2K5/8/8/8 w - - 0 1", move(d5, d6, QUIET_MOVE), 1, 0);
  test("8/8/8/5k2/2K5/8/4P3/8 w - - 0 1", move(e2, e4, DOUBLE_PAWN_PUSH), 1, 0);

  test("8/8/8/R2pPk2/2K5/8/8/8 w - d6 0 1", move(e5, d6, ENPASSENT), 1, 1);
  test("8/2k5/8/R2pP3/2K5/8/8/8 w - d6 0 1", move(e5, d6, ENPASSENT), 1, 0);
  test("8/4k3/8/3pP3/2K5/8/4R3/8 w - d6 0 1", move(e5, d6, ENPASSENT), 2, 1);
  test("8/5k2/8/3pP3/8/1B6/4R3/2K5 w - d6 0 1", move(e5, d6, ENPASSENT), 1, 1);

  test("8/5k2/8/8/8/8/8/4K2R w K - 0 1", move(e1, g1, KING_CASTLE), 1, 0);
  test("8/3k4/8/8/8/8/8/R3K3 w Q - 0 1", move(e1, c1, QUEEN_CASTLE), 1, 0);
  test("8/8/8/8/8/8/8/R3K2k w Q - 0 1", move(e1, c1, QUEEN_CASTLE), 1, 0);

  return 0;
}
