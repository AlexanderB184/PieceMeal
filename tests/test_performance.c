#include "../include/chess-lib.h"

// functions to test for performance
// piece
// is_legal
// is_psuedo_legal
// make_move
// unmake_move
// generate_moves
// + incremental move gen once implemented
// is_gameover
// load_position
// save_position

// test of typical search usecase (brute force fixed depth from random starting position) to get relative usage


const char * test_positions[] = {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
};

chess_state_t chess_state;


struct timer_t {
    size_t total_elapsed_time_ns;
    size_t time_started;
}

#define PAUSE_TIMER(timer) do { timer.total_elapsed_time_ns += } while(0);


piece_perf() {
    for (int i = 0; i < 5; i++) {
        const char * position = test_positions[i];
        (void)load_position(&chess_state, position);
        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {
            (void)piece(&chess_state, rankfile_to_sq0x88(rank, file));
            }
        }
    }
}
