#define CATCH_CONFIG_MAIN
#include <stdio.h>

#include <catch2/catch_test_macros.hpp>

#include "../include/chess.h"

static chess_state_t chess_state;

TEST_CASE("pawn push along pin", "[LEGAL MOVE]") {
  load_position(&chess_state, "3r4/8/7k/8/8/3P4/8/3K4 w - - 0 1");
  CHECK(is_legal(&chess_state, move(D3, D4, QUIET_MOVE)) == 1);
}
TEST_CASE("unpinned pawn push", "[LEGAL MOVE]") {
  load_position(&chess_state, "3r4/8/7k/8/8/3P4/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(D3, D4, QUIET_MOVE)) == 1);
}
TEST_CASE("unpinned pawn push 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "3r4/8/7k/8/8/3PK3/8/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(E3, D4, QUIET_MOVE)) == 0);
}
TEST_CASE("pinned pawn push", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/8/8/r2PK3/8/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(D3, D4, QUIET_MOVE)) == 0);
}
TEST_CASE("move out of pin", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/8/8/r2PK3/8/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(E3, D4, QUIET_MOVE)) == 1);
}
TEST_CASE("unpinned pawn push, wrong pinning piece", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/8/8/b2PK3/8/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(D3, D4, QUIET_MOVE)) == 1);
}
TEST_CASE("unpinned pawn push, wrong pinning piece 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/8/8/b2PK3/8/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(E3, D4, QUIET_MOVE)) == 1);
}
TEST_CASE("bishop pin", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/8/2b5/3P4/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(D3, D4, QUIET_MOVE)) == 0);
}
TEST_CASE("capture pinning bishop", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/8/2b5/3P4/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(D3, C4, CAPTURE)) == 1);
}
TEST_CASE("double blocked bishop pin", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/1b6/2p5/3P4/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(D3, C4, CAPTURE)) == 1);
}
TEST_CASE("double blocked bishop pin 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/1b6/2p5/3P4/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(D3, D4, QUIET_MOVE)) == 1);
}
TEST_CASE("bishop pin 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/1b6/8/2pP4/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(E2, D2, QUIET_MOVE)) == 0);
}
TEST_CASE("pinned knight", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/4r3/7k/1b2N3/8/3P4/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(E5, F3, QUIET_MOVE)) == 0);
}
TEST_CASE("reveal attack to block check", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/4r3/7k/4N2b/8/3P4/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(E5, F3, QUIET_MOVE)) == 0);
}
TEST_CASE("move out of pin 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/4r3/7k/7b/8/3P1N2/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(E2, D2, QUIET_MOVE)) == 1);
}
TEST_CASE("move along pin", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/4r3/7k/7b/8/3P1N2/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(E2, E1, QUIET_MOVE)) == 0);
}
TEST_CASE("capture checking piece", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/7b/8/3PrN2/4K3/8 w - - 0 1");
  CHECK(is_legal(&chess_state, move(E2, E3, CAPTURE)) == 1);
}
TEST_CASE("queen castle blocked", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/5r1k/7b/8/3P4/3N4/R3K2R w KQ - 0 1");
  CHECK(is_legal(&chess_state, move(E1, C1, QUEEN_CASTLE)) == 0);
}
TEST_CASE("king castle blocked", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/5r1k/7b/8/3P4/3N4/R3K2R w KQ - 0 1");
  CHECK(is_legal(&chess_state, move(E1, G1, KING_CASTLE)) == 0);
}
TEST_CASE("queen castle blocked 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "7k/8/6rb/8/8/3P4/8/R3K2R w KQ - 0 1");
  CHECK(is_legal(&chess_state, move(E1, C1, QUEEN_CASTLE)) == 0);
}
TEST_CASE("king castle blocked 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "7k/8/6rb/8/8/3P4/8/R3K2R w KQ - 0 1");
  CHECK(is_legal(&chess_state, move(E1, G1, KING_CASTLE)) == 0);
}
TEST_CASE("king castle", "[LEGAL MOVE]") {
  load_position(&chess_state, "7k/8/6r1/6b1/8/3P4/8/R3K2R w KQ - 0 1");
  CHECK(is_legal(&chess_state, move(E1, G1, KING_CASTLE)) == 1);
}
TEST_CASE("king castle blocked 3", "[LEGAL MOVE]") {
  load_position(&chess_state, "7k/8/6r1/6b1/8/3P2n1/8/R3K2R w KQ - 0 1");
  CHECK(is_legal(&chess_state, move(E1, G1, KING_CASTLE)) == 0);
}
TEST_CASE("enpassent", "[LEGAL MOVE]") {
  load_position(&chess_state, "8/8/7k/3Pp3/8/8/8/4K3 w - e6 0 2");
  CHECK(is_legal(&chess_state, move(D5, E6, ENPASSENT)) == 1);
}
TEST_CASE("enpassent blocked by verticle rook pin", "[LEGAL MOVE]") {
  load_position(&chess_state, "3r4/8/7k/3Pp3/8/8/8/3K4 w - e6 0 2");
  CHECK(is_legal(&chess_state, move(D5, E6, ENPASSENT)) == 0);
}
TEST_CASE("enpassent not blocked by NE diagonal bishop pin", "[LEGAL MOVE]") {
  load_position(&chess_state, "3r2b1/8/7k/3Pp3/8/1K6/8/8 w - e6 0 2");
  CHECK(is_legal(&chess_state, move(D5, E6, ENPASSENT)) == 1);
}
TEST_CASE("enpassent blocked by horizontal queen pin", "[LEGAL MOVE]") {
  load_position(&chess_state, "3r2b1/8/7k/1K1Pp2q/8/8/8/8 w - e6 0 2");
  CHECK(is_legal(&chess_state, move(D5, E6, ENPASSENT)) == 0);
}
TEST_CASE("enpassent blocked by horizontal queen pin 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "3r2b1/8/7k/2qPpK2/8/8/8/8 w - e6 0 2");
  CHECK(is_legal(&chess_state, move(D5, E6, ENPASSENT)) == 0);
}
TEST_CASE("enpassent blocked by NW diagonal bishop pin", "[LEGAL MOVE]") {
  load_position(&chess_state, "1b1r4/8/7k/3Pp3/8/6K1/8/8 w - e6 0 2");
  CHECK(is_legal(&chess_state, move(D5, E6, ENPASSENT)) == 0);
}
TEST_CASE("enpassent blocked by NW diagonal bishop pin 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/7k/3Pp3/8/8/6K1/8 w - e6 0 2");
  CHECK(is_legal(&chess_state, move(D5, E6, ENPASSENT)) == 0);
}
TEST_CASE("king move", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/7k/3Pp3/6p1/8/6K1/8 w - e6 0 2");
  CHECK(is_legal(&chess_state, move(G2, G3, QUIET_MOVE)) == 1);
}
TEST_CASE("king move blocked by pawn", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/7k/3Pp3/6p1/8/6K1/8 w - e6 0 2");
  CHECK(is_legal(&chess_state, move(G2, H3, QUIET_MOVE)) == 0);
}
TEST_CASE("king move blocked by pawn 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/7k/3Pp3/6p1/8/6K1/8 w - e6 0 2");
  CHECK(is_legal(&chess_state, move(G2, F3, QUIET_MOVE)) == 0);
}
TEST_CASE("king move along bishop attack", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2");
  CHECK(is_legal(&chess_state, move(G2, F3, QUIET_MOVE)) == 0);
}
TEST_CASE("king move too close to opposing king", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2");
  CHECK(is_legal(&chess_state, move(G2, G3, QUIET_MOVE)) == 0);
}
TEST_CASE("king move too close to opposing king 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2");
  CHECK(is_legal(&chess_state, move(G2, H3, QUIET_MOVE)) == 0);
}
TEST_CASE("king move safe", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2");
  CHECK(is_legal(&chess_state, move(G2, H2, QUIET_MOVE)) == 1);
}
TEST_CASE("king move along bishop attack 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2");
  CHECK(is_legal(&chess_state, move(G2, H1, QUIET_MOVE)) == 0);
}
TEST_CASE("king move safe 2", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2");
  CHECK(is_legal(&chess_state, move(G2, F2, QUIET_MOVE)) == 1);
}
TEST_CASE("king move safe 3", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2");
  CHECK(is_legal(&chess_state, move(G2, F1, QUIET_MOVE)) == 1);
}
TEST_CASE("king move safe 4", "[LEGAL MOVE]") {
  load_position(&chess_state, "b2r4/8/8/4P3/7k/8/6K1/8 w - - 0 2");
  CHECK(is_legal(&chess_state, move(G2, G1, QUIET_MOVE)) == 1);
}
