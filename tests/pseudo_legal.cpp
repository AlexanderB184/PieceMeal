#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../include/chess.h"

chess_state_t chess_state;

TEST_CASE("out of bounds", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(H1 + 1, H1 + 3, QUIET_MOVE)) == 0);
}
TEST_CASE("null move", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A1, A1, QUIET_MOVE)) == 0);
}
TEST_CASE("empty square", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A3, B3, QUIET_MOVE)) == 0);
}
TEST_CASE("capturing own piece", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A1, B1, QUIET_MOVE)) == 0);
}
TEST_CASE("capturing own piece with capture flag", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A1, B1, CAPTURE)) == 0);
}
TEST_CASE("invalid capture movement", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A2, A7, CAPTURE)) == 0);
}
TEST_CASE("invalid enpassent capture", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A2, A5, ENPASSENT)) == 0);
}
TEST_CASE("pawn push", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A2, A3, QUIET_MOVE)) == 1);
}
TEST_CASE("invalid pawn push", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A2, B3, QUIET_MOVE)) == 0);
}
TEST_CASE("capturing nothing", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A2, B3, CAPTURE)) == 0);
}
TEST_CASE("pawn push with invalid flag", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A2, A3, QUEEN_PROMOTION)) == 0);
}
TEST_CASE("double pawn push", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A2, A4, DOUBLE_PAWN_PUSH)) == 1);
}
TEST_CASE("double pawn push without flag", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A2, A4, QUIET_MOVE)) == 0);
}
TEST_CASE("knight move", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(B1, C3, QUIET_MOVE)) == 1);
}
TEST_CASE("invalid flag on knight move", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(B1, C3, DOUBLE_PAWN_PUSH)) == 0);
}
TEST_CASE("invalid knight move", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(B1, C4, QUIET_MOVE)) == 0);
}
TEST_CASE("invalid bishop move", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(C1, C4, QUIET_MOVE)) == 0);
}
TEST_CASE("invalid rook move", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(A1, C4, QUIET_MOVE)) == 0);
}
TEST_CASE("invalid queen move", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(D1, C4, QUIET_MOVE)) == 0);
}
TEST_CASE("invalid king move", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(E1, E3, QUIET_MOVE)) == 0);
}
TEST_CASE("blocked king castle", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(E1, G1, KING_CASTLE)) == 0);
}
TEST_CASE("blocked queen castle", "[PSEUDO LEGAL MOVE]") {
  load_start_position(&chess_state);
  CHECK(is_pseudo_legal(&chess_state, move(E1, C1, QUEEN_CASTLE)) == 0);
}
TEST_CASE("castle", "[PSEUDO LEGAL MOVE]") {
  load_position(
      &chess_state,
      "rnbqk1nr/ppp2ppp/3p4/2B1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 4");
  CHECK(is_pseudo_legal(&chess_state, move(E1, G1, KING_CASTLE)) == 1);
}
TEST_CASE("invalid castle, wrong piece", "[PSEUDO LEGAL MOVE]") {
  load_position(
      &chess_state,
      "rnbqk1nr/ppp2ppp/3p4/2B1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 4");
  CHECK(is_pseudo_legal(&chess_state, move(D1, G1, KING_CASTLE)) == 0);
}
TEST_CASE("invalid castle, wrong square", "[PSEUDO LEGAL MOVE]") {
  load_position(
      &chess_state,
      "rnbqk1nr/ppp2ppp/3p4/2B1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 4");
  CHECK(is_pseudo_legal(&chess_state, move(E1, H1, KING_CASTLE)) == 0);
}
TEST_CASE("bishop capture check", "[PSEUDO LEGAL MOVE]") {
  load_position(
      &chess_state,
      "rnbqk1nr/ppp2ppp/3p4/2B1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 4");
  CHECK(is_pseudo_legal(&chess_state, move(C4, F7, CAPTURE)) == 1);
}
TEST_CASE("invalid, should be marked as CAPTURE", "[PSEUDO LEGAL MOVE]") {
  load_position(
      &chess_state,
      "rnbqk1nr/ppp2ppp/3p4/2B1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 4");
  CHECK(is_pseudo_legal(&chess_state, move(C4, F7, QUIET_MOVE)) == 0);
}
TEST_CASE("queen move", "[PSEUDO LEGAL MOVE]") {
  load_position(
      &chess_state,
      "rnbqk1nr/ppp2ppp/3p4/2B1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 4");
  CHECK(is_pseudo_legal(&chess_state, move(D1, E2, QUIET_MOVE)) == 1);
}
TEST_CASE("rook move", "[PSEUDO LEGAL MOVE]") {
  load_position(
      &chess_state,
      "rnbqk1nr/ppp2ppp/3p4/2B1p3/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 0 4");

  CHECK(is_pseudo_legal(&chess_state, move(H1, F1, QUIET_MOVE)) == 1);
}
TEST_CASE("king castle in check", "[PSEUDO LEGAL MOVE]") {
  load_position(
      &chess_state,
      "rnB1k2r/ppp2ppp/3p1n2/2B1p3/2BPP3/5Q2/PPP2PPP/RNq1K2R w KQkq - 0 7");
  CHECK(is_pseudo_legal(&chess_state, move(E1, G1, KING_CASTLE)) == 0);
}
TEST_CASE("king move in check", "[PSEUDO LEGAL MOVE]") {
  load_position(
      &chess_state,
      "rnB1k2r/ppp2ppp/3p1n2/2B1p3/2BPP3/5Q2/PPP2PPP/RNq1K2R w KQkq - 0 7");
  CHECK(is_pseudo_legal(&chess_state, move(E1, E2, QUIET_MOVE)) ==
        1);  // king move
}

TEST_CASE("non interposing move in check", "[PSEUDO LEGAL MOVE]") {
  load_position(
      &chess_state,
      "rnB1k2r/ppp2ppp/3p1n2/2B1p3/2BPP3/5Q2/PPP2PPP/RNq1K2R w KQkq - 0 7");
  CHECK(is_pseudo_legal(&chess_state, move(C4, F7, CAPTURE)) ==
        0);  // non interposing move
}

TEST_CASE("interposing move", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/K4r2/4P1P1/3P2k1/8/8/8/8 w - - 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(E6, E7, QUIET_MOVE)) == 1);
}
TEST_CASE("capture checking piece", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/K4r2/4P1P1/3P2k1/8/8/8/8 w - - 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(E6, F7, CAPTURE)) == 1);
}
TEST_CASE("invalid double pawn push tp interpose", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/K4r2/4P1P1/3P2k1/8/8/8/8 w - - 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(D5, D7, DOUBLE_PAWN_PUSH)) == 0);
}
TEST_CASE("interpose falls short", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/K4r2/4P1P1/3P2k1/8/8/8/8 w - - 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(D5, D6, QUIET_MOVE)) == 0);
}
TEST_CASE("interpose on wrong side of sliding piece", "[PSEUDO LEGAL MOVE]") {
  CHECK(is_pseudo_legal(&chess_state, move(G6, G7, QUIET_MOVE)) == 0);
}

TEST_CASE("non interposable check", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/k4n2/4P3/3P2K1/8/8/8/8 w - - 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(E6, E7, QUIET_MOVE)) == 0);
}
TEST_CASE("capturing knight check", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/k4n2/4P3/3P2K1/8/8/8/8 w - - 0 1");

  CHECK(is_pseudo_legal(&chess_state, move(E6, F7, CAPTURE)) == 1);
}

TEST_CASE("capturing knight check during double check", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/k4n2/4P2b/3P2K1/8/8/8/8 w - - 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(E6, F7, CAPTURE)) == 0);
}
TEST_CASE("capturing checking bishop with king", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/k4n2/4P2b/3P2K1/8/8/8/8 w - - 0 1");

  CHECK(is_pseudo_legal(&chess_state, move(G5, H6, CAPTURE)) == 1);
}
TEST_CASE("escaping double check", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/k4n2/4P2b/3P2K1/8/8/8/8 w - - 0 1");

  CHECK(is_pseudo_legal(&chess_state, move(G5, G6, QUIET_MOVE)) == 1);
}

TEST_CASE("queen castle", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state,
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KBNR w KQkq - 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(E1, C1, QUEEN_CASTLE)) == 1);
}
TEST_CASE("invalid queen castle", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state,
                "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/R3KBNR w KQkq - 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(F1, C1, QUEEN_CASTLE)) == 0);
}

TEST_CASE("promotion missing promotion flag", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/3P4/8/8/5K2/8/5k2/8 w - - 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(D7, D8, QUIET_MOVE)) == 0);
}
TEST_CASE("knight promotion", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/3P4/8/8/5K2/8/5k2/8 w - - 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(D7, D8, KNIGHT_PROMOTION)) == 1);
}
TEST_CASE("bishop promotion", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/3P4/8/8/5K2/8/5k2/8 w - - 0 1");

  CHECK(is_pseudo_legal(&chess_state, move(D7, D8, BISHOP_PROMOTION)) == 1);
}
TEST_CASE("rook promotion", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/3P4/8/8/5K2/8/5k2/8 w - - 0 1");

  CHECK(is_pseudo_legal(&chess_state, move(D7, D8, ROOK_PROMOTION)) == 1);
}
TEST_CASE("queen promotion", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/3P4/8/8/5K2/8/5k2/8 w - - 0 1");

  CHECK(is_pseudo_legal(&chess_state, move(D7, D8, QUEEN_PROMOTION)) == 1);
}

TEST_CASE("enpassent capture checking pawn", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/8/8/3Pp3/5K2/8/5k2/8 w - e6 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(D5, E6, ENPASSENT)) == 1);
}

TEST_CASE("invalid enpassent capture, not on target", "[PSEUDO LEGAL MOVE]") {
  load_position(&chess_state, "8/8/8/3Pp3/5K2/8/5k2/8 w - e6 0 1");
  CHECK(is_pseudo_legal(&chess_state, move(D5, C6, ENPASSENT)) == 0);
}