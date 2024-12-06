#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../include/chess-lib.h"

static chess_state_t chess_state;

void test_recursive(int N) {
  if (N == 0) return;
  move_t moves[256];
  int move_count = generate_legal_moves(&chess_state, moves);
  for (int i = 0; i < move_count; i++) {
    int ncheckers = chess_state.n_checks;
    int checker = chess_state.check_square;
    int discover = chess_state.discovered_check;
    int halfmoves = chess_state.half_move_clock;
    int lastirr = chess_state.ply_of_last_irreversible_move;
    int ply = chess_state.ply_counter;
    int ep = chess_state.enpassent_target;
    castle_rights_t rights = chess_state.castle_rights;
    uint64_t zobrist = chess_state.zobrist;
    int whiteKing = chess_state.white_pieces.king_square;
    int blackKing = chess_state.black_pieces.king_square;
    int who2move = chess_state.black_to_move;
    piece_t pieces[16 * 16];
    memcpy(pieces, chess_state.board, 256);
    make_move(&chess_state, moves[i]);
    test_recursive(N - 1);
    unmake_move(&chess_state);
    assert(ncheckers == chess_state.n_checks);
    assert(checker == chess_state.check_square);
    assert(discover == chess_state.discovered_check);
    assert(halfmoves == chess_state.half_move_clock);
    assert(lastirr == chess_state.ply_of_last_irreversible_move);
    assert(ply == chess_state.ply_counter);
    assert(ep == chess_state.enpassent_target);
    assert(memcmp(pieces, chess_state.board, 256) == 0);
    assert(whiteKing == chess_state.white_pieces.king_square);
    assert(blackKing == chess_state.black_pieces.king_square);
    assert(rights == chess_state.castle_rights);
    assert(who2move == chess_state.black_to_move);
    assert(zobrist == chess_state.zobrist);
  }
}

void test(const char* fen_string) {
  static int testno = 0;
  testno++;
  load_position(&chess_state, fen_string);
  test_recursive(6);
  printf("completed test %d!!!\n", testno);
}

int main() {
  test("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
  test("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  test("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
  test("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
  return 0;
}
