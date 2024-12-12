#define CATCH_CONFIG_MAIN
#include <stdio.h>

#include <catch2/catch_test_macros.hpp>

#include "../include/chess-lib.h"

static chess_state_t chess_state;

void test(const char* fen, move_t move, int expected_check,
          int expected_discover) {
  load_position(&chess_state, fen);
  make_move(&chess_state, move);
  CHECK(chess_state.n_checks == expected_check);
  CHECK(chess_state.discovered_check == expected_discover);
}

TEST_CASE("Quiet Non Checking Move", "[IS CHECK]") {
  test("8/4k3/8/8/8/4K3/8/8 w - - 0 1", move(e3, e4, QUIET_MOVE), 0, 0);
}

TEST_CASE("Knight Check", "[IS CHECK]") {
  test("8/4k3/8/8/8/2N1K3/8/8 w - - 0 1", move(c3, d5, QUIET_MOVE), 1, 0);
}

TEST_CASE("Bishop Check", "[IS CHECK]") {
  test("8/4k3/8/8/8/2N1K1B1/8/8 w - - 0 1", move(g3, h4, QUIET_MOVE), 1, 0);
}

TEST_CASE("Rook Check", "[IS CHECK]") {
  test("8/4k3/8/8/8/2N1K1B1/8/1R6 w - - 0 1", move(b1, b7, QUIET_MOVE), 1, 0);
}

TEST_CASE("Pawn Check", "[IS CHECK]") {
  test("8/4k3/8/5P2/8/2N1K1B1/8/1R6 w - - 0 1", move(f5, f6, QUIET_MOVE), 1, 0);
}

TEST_CASE("Moving along pin", "[IS CHECK]") {
  test("8/4k3/8/5P2/8/4K3/8/4R3 w - - 0 1", move(e3, e4, QUIET_MOVE), 0, 0);
}

TEST_CASE("Discover Check", "[IS CHECK]") {
  test("8/4k3/8/5P2/8/4K3/8/4R3 w - - 0 1", move(e3, f3, QUIET_MOVE), 1, 1);
}
TEST_CASE("Blocked Discover Check", "[IS CHECK]") {
  test("8/4k3/8/4P3/8/4K3/8/4R3 w - - 0 1", move(e3, f3, QUIET_MOVE), 0, 0);
}
TEST_CASE("Double Check", "[IS CHECK]") {
  test("8/4k3/8/6P1/8/4BK2/8/4R3 w - - 0 1", move(e3, c5, QUIET_MOVE), 2, 1);
}
TEST_CASE("King Castle Check", "[IS CHECK]") {
  test("5k2/8/8/8/8/8/8/R3K2R w K - 0 1", move(e1, g1, KING_CASTLE), 1, 0);
}
TEST_CASE("Queen Castle No Check", "[IS CHECK]") {
  test("5k2/8/8/8/8/8/8/R3K2R w K - 0 1", move(e1, c1, QUEEN_CASTLE), 0, 0);
}
TEST_CASE("Queen Castle Check Check", "[IS CHECK]") {
  test("3k4/8/8/8/8/8/8/R3K2R w K - 0 1", move(e1, c1, QUEEN_CASTLE), 1, 0);
}
TEST_CASE("Double Pawn Push Check", "[IS CHECK]") {
  test("8/8/8/4k3/8/8/5P2/4K3 w - - 0 1", move(f2, f4, DOUBLE_PAWN_PUSH), 1, 0);
}
TEST_CASE("Enpassent Check", "[IS CHECK]") {
  test("8/5k2/8/3Pp3/8/8/8/4K3 w - e6 0 1", move(d5, e6, ENPASSENT), 1, 0);
}
TEST_CASE("Enpassent Reveal 1", "[IS CHECK]") {
  test("8/2k5/8/3Pp3/8/8/7B/4K3 w - e6 0 1", move(d5, e6, ENPASSENT), 1, 1);
}
TEST_CASE("Enpassent Reveal 2", "[IS CHECK]") {
  test("8/8/8/1k1Pp2R/8/8/8/4K3 w - e6 0 1", move(d5, e6, ENPASSENT), 1, 1);
}
TEST_CASE("Enpassent Reveal 3", "[IS CHECK]") {
  test("8/8/8/1R1Pp2k/8/8/8/4K3 w - e6 0 1", move(d5, e6, ENPASSENT), 1, 1);
}
TEST_CASE("Enpassent Reveal 4", "[IS CHECK]") {
  test("8/6k1/8/3Pp3/8/8/1B6/4K3 w - e6 0 1", move(d5, e6, ENPASSENT), 1, 1);
}
TEST_CASE("Black Pawn Check", "[IS CHECK]") {
  test("8/2p5/3p4/1K6/1R3p1k/8/4P1P1/8 b - - 0 9", move(c7, c6, QUIET_MOVE), 1,
       0);
}
TEST_CASE("Queen Promotion Check", "[IS CHECK]") {
  test("7k/4P3/8/1K1p4/8/8/8/8 w - - 0 1", move(e7, e8, QUEEN_PROMOTION), 1, 0);
}
TEST_CASE("Knight Promotion Check", "[IS CHECK]") {
  test("8/4P1k1/8/1K1p4/8/8/8/8 w - - 0 1", move(e7, e8, KNIGHT_PROMOTION), 1,
       0);
}