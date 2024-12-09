#include "../include/chess-lib.h"

#include <stdio.h>

// example demonstrating use of chess api in game flow

int main(void) {
    chess_state_t chess_state = {};
    load_start_position(&chess_state);
    char buffer[16];
    while (!is_gameover(&chess_state)) {
        
        if (fgets(buffer, 16, stdin) == NULL) {
            return 1;
        }
        move_t read_move;
        if (read_algebraic_notation(buffer, sizeof(buffer), &chess_state, &read_move) < 0) {
            printf("%s is not a valid move", buffer);
            continue;
        }
        make_move(&chess_state, read_move);
    }
    colour_t winner = get_winner(&chess_state);
    if (winner == WHITE) {
        printf("White Wins");
    } else if (winner = BLACK) {
        printf("Black Wins");
    } else {
        printf("Draw");
    }
    return 0;
}