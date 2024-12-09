#include "../include/chess-lib.h"

// example showing use of chess api in search function

#define PAWN_VALUE 100
#define KNIGHT_VALUE 320
#define BISHOP_VALUE 330
#define ROOK_VALUE 500
#define QUEEN_VALUE 900


short eval(const chess_state_t* chess_state) {
    short score = 0;
    score += chess_state->white_pieces.pawn_count * PAWN_VALUE;
    score += chess_state->white_pieces.knight_count * KNIGHT_VALUE;
    score += chess_state->white_pieces.light_bishop_count * BISHOP_VALUE;
    score += chess_state->white_pieces.dark_bishop_count * BISHOP_VALUE;
    score += chess_state->white_pieces.rook_count * ROOK_VALUE;
    score += chess_state->white_pieces.queen_count * QUEEN_VALUE;
    score -= chess_state->black_pieces.pawn_count * PAWN_VALUE;
    score -= chess_state->black_pieces.knight_count * KNIGHT_VALUE;
    score -= chess_state->black_pieces.light_bishop_count * BISHOP_VALUE;
    score -= chess_state->black_pieces.dark_bishop_count * BISHOP_VALUE;
    score -= chess_state->black_pieces.rook_count * ROOK_VALUE;
    score -= chess_state->black_pieces.queen_count * QUEEN_VALUE;
    return score;
}

short min(chess_state_t* chess_state, int depth) {
    // base condition
    if (depth == 0) {
        return eval(chess_state);
    }
    // generate pseudo legal moves
    move_t moves[256];
    int move_count = generate_moves(chess_state, moves);

    // evaluate all legal moves
    short min_so_far = INT16_MAX;
    for (int i = 0; i < move_count; i++) {
        if (!is_legal(chess_state, moves[i])) {
            continue;
        }
        make_move(chess_state, moves[i]);
        short score = max(chess_state, depth - 1);
        unmake_move(chess_state);

        if (score < min_so_far) {
            min_so_far = score;
        }
    }
    return min_so_far;
}

short max(chess_state_t* chess_state, int depth) {
    // base condition
    if (depth == 0) {
        return eval(chess_state);
    }
    // generate pseudo legal moves
    move_t moves[256];
    int move_count = generate_moves(chess_state, moves);

    // evaluate all legal moves
    short max_so_far = INT16_MIN;
    for (int i = 0; i < move_count; i++) {
        if (!is_legal(chess_state, moves[i])) {
            continue;
        }
        make_move(chess_state, moves[i]);
        short score = min(chess_state, depth - 1);
        unmake_move(chess_state);

        if (score > max_so_far) {
            max_so_far = score;
        }
    }

    return max_so_far;
}

move_t best_move_white(chess_state_t* chess_state, int depth) {
    move_t moves[256];
    int move_count = generate_moves(chess_state, moves);

    // evaluate all legal moves
    short max_so_far = INT16_MIN;
    move_t best_so_far;
    for (int i = 0; i < move_count; i++) {
        if (!is_legal(chess_state, moves[i])) {
            continue;
        }
        make_move(chess_state, moves[i]);
        short score = min(chess_state, depth - 1);
        unmake_move(chess_state);

        if (score > max_so_far) {
            max_so_far = score;
            best_so_far = moves[i];
        }
    }
    return best_so_far;
}


