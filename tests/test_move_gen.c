#include <stdio.h>

#include "../include/chess-lib.h"

static chess_state_t chess_state;

void test(const char* fen_string, int expected_move_count) {
  static int testno = 0;
  testno++;
  load_position(&chess_state, fen_string);
  //printf("ischeck: %d ", is_check(&chess_state));
  move_t moves[256];
  int move_count = generate_legal_moves(&chess_state, moves);
  if (move_count == expected_move_count) {
    printf("passed %d\n", testno);
  } else {
    printf("failed %d: expected: %d got: %d\n", testno, expected_move_count,
           move_count);
    for (int i = 0; i < move_count; i++) {
      printf("from: %d to: %d flags: %d\n", get_from(moves[i]),
             get_to(moves[i]), get_flags(moves[i]));
    }
  }
}

int main() {
  test("8/8/8/8/8/8/8/R3K2k w Q - 0 1", 16); // rook and king castle rights
  test("8/8/8/8/8/8/8/R3K2k b Q - 0 1", 3);  // king movement
  test("4r3/8/8/8/8/8/8/R3K2k w Q - 0 1", 4); // king in check
  test("8/8/8/8/8/8/8/R3K1k1 b Q - 0 1", 3); // king blocking king movement
  test("8/8/8/3pP3/4K3/8/8/6k1 w - d6 0 1", 8); // king threatened by enpassent capturable piece
  test("4r3/8/8/3pP3/4K3/8/8/6k1 w - d6 0 1", 7); // king threatened by enpassent capturable piece but pawn is pinned
  test("K3r3/3P4/8/8/8/8/8/7k w - - 0 1", 10); // capture/block checker with promoting pawn
  test("4r3/3P4/8/8/8/8/4K3/7k w - - 0 1", 10); // capture checker with promoting pawn
  test("4r3/3P4/8/8/8/8/r7/1k1K4 w - - 0 1", 8); // pinned king, only move is promotion
  test("7k/8/8/1K1Pp2r/8/8/8/8 w - e6 0 1", 9); //enpassent blocked by rook pin
  test("7k/8/8/1K1Pp3/7r/8/8/8 w - e6 0 1", 7); //enpassent not blocked by pin
  test("1K5k/8/8/3Pp3/5b1r/8/8/8 w - e6 0 1", 6); //enpassent blocked by bishop pin
  test("r1bqk1nr/pppp1ppp/2n5/2b1p3/5P2/5K2/PPPPP1PP/RNB1QBNR b kq - 3 4", 14+10+5+1+4+2+3); // position causing crashes in bot
  return 0;
}
