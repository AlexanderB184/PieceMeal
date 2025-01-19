#define CATCH_CONFIG_MAIN
#include <stdio.h>

#include <catch2/catch_test_macros.hpp>

#include "../include/chess.h"

static chess_state_t chess_state;

TEST_CASE("Draw by threefold repetition", "[IS GAMEOVER]") {
  load_start_position(&chess_state);
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(G1, F3, QUIET_MOVE));
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(G8, F6, QUIET_MOVE));
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(F3, G1, QUIET_MOVE));
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(F6, G8, QUIET_MOVE));
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(G1, F3, QUIET_MOVE));
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(G8, F6, QUIET_MOVE));
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(F3, G1, QUIET_MOVE));
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(F6, G8, QUIET_MOVE));
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(G1, F3, QUIET_MOVE));
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(G8, F6, QUIET_MOVE));
  CHECK(is_draw_by_repetition(&chess_state));
}

TEST_CASE("Draw by threefold repetition 2", "[IS GAMEOVER]") {
  load_start_position(&chess_state);
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(E2, E4, DOUBLE_PAWN_PUSH));
  CHECK(!is_draw_by_repetition(&chess_state));
  make_move(&chess_state, move(E7, E5, DOUBLE_PAWN_PUSH));
  CHECK(!is_draw_by_repetition(&chess_state));
  for (int i = 0; i < 2; i++) {
    CHECK(!is_draw_by_repetition(&chess_state));
    make_move(&chess_state, move(E1, E2, QUIET_MOVE));
    CHECK(!is_draw_by_repetition(&chess_state));
    make_move(&chess_state, move(E8, E7, QUIET_MOVE));
    CHECK(!is_draw_by_repetition(&chess_state));
    make_move(&chess_state, move(E2, E1, QUIET_MOVE));
    CHECK(!is_draw_by_repetition(&chess_state));
    make_move(&chess_state, move(E7, E8, QUIET_MOVE));
  }
  CHECK(is_draw_by_repetition(&chess_state));
}

TEST_CASE("Stalemate", "[IS GAMEOVER]") {
  load_position(&chess_state, "k7/8/8/8/8/8/5q2/7K w - - 0 1");
  CHECK(is_stalemate(&chess_state));
}

TEST_CASE("Checkmate", "[IS GAMEOVER]") {
  load_position(&chess_state, "k7/8/8/7r/8/8/5q2/7K w - - 0 1");
  CHECK(is_checkmate(&chess_state));
}

TEST_CASE("Fifty Move Rule", "[IS GAMEOVER]") {
  load_position(&chess_state, "k7/8/8/7r/8/8/4q3/7K w - - 100 1");
}

TEST_CASE("Fifty Move Rule Checkmate", "[IS GAMEOVER]") {
  // enforce that checkmate takes precedence over draw by 50 move rule
  load_position(&chess_state, "k7/8/8/7r/8/8/7q/7K w - - 100 1");
  CHECK(is_checkmate(&chess_state));
  CHECK(is_draw_by_50_move_rule(&chess_state));
  CHECK(is_gameover(&chess_state) == CHECKMATE);
}

TEST_CASE("Fifty Move Rule 2", "[IS GAMEOVER]") {
  load_position(&chess_state, "k7/8/8/7r/8/8/4q3/7K w - - 99 1");
  CHECK(!is_draw_by_50_move_rule(&chess_state));
  make_move(&chess_state, move(H1, G1, QUIET_MOVE));
  CHECK(is_draw_by_50_move_rule(&chess_state));
}

TEST_CASE("Insufficient Material 1", "[IS GAMEOVER]") {
  load_position(&chess_state, "8/8/8/1K4k1/8/8/8/8 w - - 0 1");
  CHECK(is_draw_by_insufficient_material(&chess_state));
}

TEST_CASE("Insufficient Material 2", "[IS GAMEOVER]") {
  load_position(&chess_state, "8/8/8/1K4k1/8/8/5N2/8 w - - 0 1");
  CHECK(is_draw_by_insufficient_material(&chess_state));
}

TEST_CASE("Insufficient Material 3", "[IS GAMEOVER]") {
  load_position(&chess_state, "8/8/4n3/1K4k1/8/8/5N2/8 w - - 0 1");
  CHECK(is_draw_by_insufficient_material(&chess_state));
}

TEST_CASE("Insufficient Material 4", "[IS GAMEOVER]") {
  load_position(&chess_state, "8/8/3B4/1K4k1/8/8/8/8 w - - 0 1");
  CHECK(is_draw_by_insufficient_material(&chess_state));
}

TEST_CASE("Insufficient Material 5", "[IS GAMEOVER]") {
  load_position(&chess_state, "8/8/4R3/1K4k1/8/8/8/8 w - - 0 1");
  CHECK(!is_draw_by_insufficient_material(&chess_state));
}

TEST_CASE("Insufficient Material 6", "[IS GAMEOVER]") {
  load_position(&chess_state, "8/8/4P3/1K4k1/8/8/8/8 w - - 0 1");
  CHECK(!is_draw_by_insufficient_material(&chess_state));
}

TEST_CASE("Insufficient Material 7", "[IS GAMEOVER]") {
  load_position(&chess_state, "8/4q3/4P3/1K4k1/8/8/8/8 w - - 0 1");
  CHECK(!is_draw_by_insufficient_material(&chess_state));
}

TEST_CASE("Insufficient Material 8", "[IS GAMEOVER]") {
  load_position(&chess_state, "5B2/8/8/1K4k1/8/8/4B3/8 w - - 0 1");
  CHECK(!is_draw_by_insufficient_material(&chess_state));
}

TEST_CASE("Insufficient Material 9", "[IS GAMEOVER]") {
  load_position(&chess_state, "8/5B2/8/1K4k1/8/8/4B3/8 w - - 0 1");
  CHECK(!is_draw_by_insufficient_material(&chess_state));
}

TEST_CASE("Insufficient Material 10", "[IS GAMEOVER]") {
  load_position(&chess_state, "8/4N3/8/1K4k1/8/4N3/8/8 w - - 0 1");
  CHECK(!is_draw_by_insufficient_material(&chess_state));
}