#include "../include/chess-lib.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
char printable_chars[] = {
    'a', 'b', 'c', 'd', 'e',  'f', 'g',  'h',  'i', 'j', 'k',  'l',  'm', 'n',
    'o', 'p', 'q', 'r', 's',  't', 'u',  'v',  'w', 'x', 'y',  'z',  'A', 'B',
    'C', 'D', 'E', 'F', 'G',  'H', 'I',  'J',  'K', 'L', 'M',  'N',  'O', 'P',
    'Q', 'R', 'S', 'T', 'U',  'V', 'W',  'X',  'Y', 'Z', '0',  '1',  '2', '3',
    '4', '5', '6', '7', '8',  '9', '-',  '+',  '=', '#', ':',  '?',  ',', '.',
    '_', '<', '>', '/', '\\', ';', '\'', '\"', '{', '}', '[',  ']',  '(', ')',
    '*', '&', '^', '%', '$',  '@', '!',  '`',  '~', ' ', '\t', '\n', '\r'};
char printable_aln_chars[] = {
    'a', 'b', 'c', 'd', 'e',  'f', 'g',  'h', 'i',   '1',  '2', '3',
    '4', '5', '6', '7', '8',  'Q', 'K', 'P', 'B', 'N', 'Z'};
void make_random_str(char* buffer, int max_size) {
    int len = rand() % max_size;
    for (int i = 0; i < len; i ++) {
        buffer[i] = printable_aln_chars[rand() % sizeof(printable_aln_chars)];
    }
    buffer[len] = '\0';
}

int test1() {
    char buffer[1028];
    chess_state_t chess_state = {0};
    load_position(&chess_state, "8/3N1N2/8/8/6k1/3N4/8/K7 w - - 0 1");
    for (int i = 0; i < 1; i++) {
        make_random_str(buffer, 8);
        move_t Move;
        int outv = read_algebraic_notation(buffer, sizeof(buffer), &chess_state, &Move);
        if (outv == -1) continue;
        char out[1028];
        outv = write_algebraic_notation(out, sizeof(out), &chess_state, Move);
        if (outv == -1) {
            printf("error, couldn't write what it could read: \'%s\'\n", buffer);
        } else {
            printf("read: \'%s\'\nwrote: \'%s\'\n\n", buffer, out);
        }
    }
    return 0;
}

int test2() {
    char buffer[1028];
    chess_state_t chess_state = {0};
    load_position(&chess_state, "8/3N1N2/8/8/6k1/3N1N2/8/K7 w - - 0 1");
    move_t moves[256];
    int move_count = generate_legal_moves(&chess_state, moves);
    for (int i = 0; i < move_count; i++) {
        char out[1028];
        int outv = write_algebraic_notation(out, sizeof(out), &chess_state, moves[i]);
        if (outv == -1) {
            outv = write_long_algebraic_notation(out, sizeof(out), moves[i]);
            printf("couldn't write \'%s\' in algebraic notation\n", out);
            continue;
        }
        printf("%s\n", out);
        move_t Move;
        outv = read_algebraic_notation(out, sizeof(buffer), &chess_state, &Move);
        if (outv == -1) {
            printf("error, couldn't read what it could write: \'%s\'\n", out);
        } else {
            if (!compare_moves(moves[i], Move)) {
                printf("read move different from initial move\n");
            }
        }
    }
    return 0;
}

void test3(chess_state_t* chess_state, int depth) {
    if (depth <= 0) return;
    move_t moves[256];
    int move_count = generate_legal_moves(chess_state, moves);
    for (int i = 0; i < move_count; i++) {
        char out[1028];
        int outv = write_long_algebraic_notation(out, sizeof(out), moves[i]);
        if (outv == -1) {
            outv = write_long_algebraic_notation(out, sizeof(out), moves[i]);
            printf("couldn't write \'%s\' in algebraic notation\n", out);
            exit(1);
        }
        //printf("%s\n", out);
        move_t Move;
        outv = read_long_algebraic_notation(out, sizeof(out), chess_state, &Move);
        if (outv == -1) {
            char m[1028];
            write_movetext_debug(m, sizeof(m), chess_state);
            printf("%s\n",m);
            printf("error, couldn't read what it could write: \'%s\'\n", out);
            exit(1);
        } else {
            if (!compare_moves(moves[i], Move)) {
                printf("read move different from initial move\n");
                exit(1);
            }
        }
        make_move(chess_state, Move);
        test3(chess_state, depth - 1);
        unmake_move(chess_state);
    }

}

int main() {
    chess_state_t chess_state = {0};
    load_position(&chess_state, "K7/5R2/2R5/7R/p1p3R1/4k1p1/1P1ppp1P/1R2B3 w - - 0 1");
    test3(&chess_state, 6);
}