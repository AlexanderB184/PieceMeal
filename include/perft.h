#ifndef PERFT_H
#define PERFT_H

#include "../include/chess-lib.h"

typedef struct {
  size_t nodes, captures, enpassent, castles, promotions, checks,
      discovered_checks, double_checks, checkmates;
} perft_results_t;

typedef enum {
  PERFT_NODES = 0x01,
  PERFT_CAPTURES = 0x02,
  PERFT_ENPASSENT = 0x04,
  PERFT_CASTLES = 0x08,
  PERFT_PROMOTIONS = 0x10,
  PERFT_CHECKS = 0x20,
  PERFT_DISCOVER_CHECKS = 0x40,
  PERFT_DOUBLE_CHECKS = 0x80,
  PERFT_CHECKMATE = 0x100,
  PERFT_ALL = 0x1FF,
  PERFT_JUST_NODES = 0x01,
} perft_active_columns;

void print_perft_results(perft_results_t results, perft_active_columns flags);
int compare_perft_results(perft_results_t lhs, perft_results_t rhs,
                          perft_active_columns flags);
perft_results_t combine_perft_results(perft_results_t lhs, perft_results_t rhs);
perft_results_t perft(chess_state_t* chess_state, int max_depth,
                      perft_active_columns flags);

#endif