#define CATCH_CONFIG_MAIN
#include <stdio.h>

#include <catch2/catch_test_macros.hpp>

#include "../include/chess.h"

int compare(const chess_state_t* lhs, const chess_state_t* rhs) {
    CHECK(lhs->zobrist == rhs->zobrist);
    CHECK(memcmp(lhs->board, rhs->board, sizeof(lhs->board)) == 0);
    CHECK(lhs->ply_counter == rhs->ply_counter);
    CHECK(lhs->friendly_colour == rhs->friendly_colour);
    CHECK(lhs->check_square == rhs->check_square);
    CHECK(lhs->n_checks == rhs->n_checks);
    CHECK(lhs->discovered_check == rhs->discovered_check);
    CHECK(lhs->black_to_move == rhs->black_to_move);
    CHECK(lhs->castle_rights == rhs->castle_rights);
    CHECK(lhs->enpassent_target == rhs->enpassent_target);
    CHECK(lhs->half_move_clock == rhs->half_move_clock);
    return 0;
}

void test(chess_state_t* cs) {
    chess_state_t clone = {};
    copy_position(&clone, cs);
    move_t moves[256];
    size_t move_count = generate_legal_moves(cs, moves, cs->friendly_colour);
    for (size_t i = 0; i < move_count; i++) {
      char buf[16];
        write_long_algebraic_notation(buf, sizeof(buf), moves[i]);
        puts(buf);
        make_move(cs, moves[i]);
        move_t move_in_stack = uncompress_move(cs->ply_stack[cs->ply_counter-1].move);
        printf("from %d:%d, to %d:%d, flags %d:%d\n", moves[i].from, move_in_stack.from, moves[i].to, move_in_stack.to,moves[i].priority_and_flags,move_in_stack.priority_and_flags);
        CHECK(compare_moves(move_in_stack, moves[i]));
        unmake_move(cs);
        compare(&clone, cs);
    }
    //release_position(&clone);
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
