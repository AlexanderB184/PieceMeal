#include "../include/perft.h"

#include <stdio.h>
#include <string.h>

#include "../include/chess-lib.h"

void print_perft_results(perft_results_t results, perft_active_columns flags) {
  if (flags & PERFT_NODES) printf("nodes: %zu\n", results.nodes);
  if (flags & PERFT_CAPTURES) printf("captures: %zu\n", results.captures);
  if (flags & PERFT_ENPASSENT) printf("enpassents: %zu\n", results.enpassent);
  if (flags & PERFT_CASTLES) printf("castles: %zu\n", results.castles);
  if (flags & PERFT_PROMOTIONS) printf("promotions: %zu\n", results.promotions);
  if (flags & PERFT_CHECKS) printf("checks: %zu\n", results.checks);
  if (flags & PERFT_DISCOVER_CHECKS)
    printf("discovered checks: %zu \n", results.discovered_checks);
  if (flags & PERFT_DOUBLE_CHECKS)
    printf("double checks: %zu\n", results.double_checks);
  if (flags & PERFT_CHECKMATE) printf("checkmates: %zu\n", results.checkmates);
}

int compare_perft_results(perft_results_t lhs, perft_results_t rhs,
                          perft_active_columns flags) {
  if (flags & PERFT_NODES && lhs.nodes != rhs.nodes) return 0;
  if (flags & PERFT_CAPTURES && lhs.captures != rhs.captures) return 0;
  if (flags & PERFT_ENPASSENT && lhs.enpassent != rhs.enpassent) return 0;
  if (flags & PERFT_CASTLES && lhs.castles != rhs.castles) return 0;
  if (flags & PERFT_PROMOTIONS && lhs.promotions != rhs.promotions) return 0;
  if (flags & PERFT_CHECKS && lhs.checks != rhs.checks) return 0;
  if (flags & PERFT_DISCOVER_CHECKS &&
      lhs.discovered_checks != rhs.discovered_checks)
    return 0;
  if (flags & PERFT_DOUBLE_CHECKS && lhs.double_checks != rhs.double_checks)
    return 0;
  if (flags & PERFT_CHECKMATE && lhs.checkmates != rhs.checkmates) return 0;
  return 1;
}

perft_results_t combine_perft_results(perft_results_t lhs,
                                      perft_results_t rhs) {
  perft_results_t combined = {0};
  combined.nodes = lhs.nodes + rhs.nodes;
  combined.captures = lhs.captures + rhs.captures;
  combined.enpassent = lhs.enpassent + rhs.enpassent;
  combined.castles = lhs.castles + rhs.castles;
  combined.promotions = lhs.promotions + rhs.promotions;
  combined.checks = lhs.checks + rhs.checks;
  combined.discovered_checks = lhs.discovered_checks + rhs.discovered_checks;
  combined.double_checks = lhs.double_checks + rhs.double_checks;
  combined.checkmates = lhs.checkmates + rhs.checkmates;
  return combined;
}

perft_results_t perft(chess_state_t* chess_state, int max_depth,
                      perft_active_columns flags) {
  perft_results_t results;
  memset(&results, 0, sizeof(results));
  if (max_depth == 0) {
    results.nodes = 1;
    if (is_check(chess_state)) {
      results.checks = 1;
    } else {
      return results;
    }
    if (is_double_check(chess_state)) {
      results.double_checks = 1;
    } else if (is_discover_check(chess_state)) {
      results.discovered_checks = 1;
    }
  }

  size_t move_count;
  move_t moves[256];
  move_count = generate_legal_moves(chess_state, moves);
  if (max_depth == 1 && flags == PERFT_JUST_NODES) {
    results.nodes = move_count;
    return results;
  }
  if (max_depth > 0) {
    for (size_t i = 0; i < move_count; i++) {
      if (max_depth == 2) {
      }
      if (max_depth == 1) {
        if (is_capture(moves[i])) {
          results.captures++;
        }

        if (is_enpassent(moves[i])) {
          results.enpassent++;
        }
        if (is_promotion(moves[i])) {
          results.promotions++;
        }
        if (is_king_castle(moves[i]) || is_queen_castle(moves[i])) {
          results.castles++;
        }
      }

      make_move(chess_state, moves[i]);
      results = combine_perft_results(results,
                                      perft(chess_state, max_depth - 1, flags));
      unmake_move(chess_state);
    }
  }

  if (max_depth == 0 && is_check(chess_state) && move_count == 0) {
    results.checkmates++;
  }
  return results;
}
