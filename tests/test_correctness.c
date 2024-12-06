#include <stdio.h>
#include <string.h>

#include "../include/chess-lib.h"

static chess_state_t CHESS_STATE;


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

perft_results_t default_board_results[10] = {
    {1, 0, 0, 0, 0, 0, 0, 0, 0},
    {20, 0, 0, 0, 0, 0, 0, 0, 0},
    {400, 0, 0, 0, 0, 0, 0, 0, 0},
    {8902, 34, 0, 0, 0, 12, 0, 0, 0},
    {197281, 1576, 0, 0, 0, 469, 0, 0, 8},
    {4865609, 82719, 258, 0, 0, 27351, 6, 0, 347},
    {119060324, 2812008, 5248, 0, 0, 809099, 329, 46, 10828},
    {3195901860, 108329926, 319617, 883453, 0, 33103848, 18026, 1628, 435767},
    {84998978956, 3523740106, 7187977, 23605205, 0, 968981593, 847039, 147215,
     9852036},
    {2439530234167, 125208536153, 319496827, 1784356000, 17334376, 36095901903,
     37101713, 5547231, 400191963},
};

perft_results_t Kiwipete_results[7] = {
    {48, 8, 0, 2, 0, 0, 0, 0, 0},
    {2039, 351, 1, 91, 0, 3, 0, 0, 0},
    {97862, 17102, 45, 3162, 0, 993, 0, 0, 1},
    {4085603, 757163, 1929, 128013, 15172, 25523, 42, 6, 43},
    {193690690, 35043416, 73365, 4993637, 8392, 3309887, 19883, 2645, 30171},
    {8031647685, 1558445089, 3577504, 184513607, 56627920, 92238050, 568417,
     54948, 360003},
};

perft_results_t position_3_results[9] = {
    {14, 1, 0, 0, 0, 2, 0, 0, 0},
    {191, 14, 0, 0, 0, 10, 0, 0, 0},
    {2812, 209, 2, 0, 0, 267, 3, 0, 0},
    {43238, 3348, 123, 0, 0, 1680, 106, 0, 17},
    {674624, 52051, 1165, 0, 0, 52950, 1292, 3, 0},
    {11030083, 940350, 33325, 0, 7552, 452473, 26067, 0, 2733},
    {178633661, 14519036, 294874, 0, 140024, 12797406, 370630, 3612, 87},
    {3009794393, 267586558, 8009239, 0, 6578076, 135626805, 7181487, 1630,
     450410},
};

perft_results_t position_4_results[9] = {
    {6, 0, 0, 0, 0, 0, 0, 0, 0},
    {264, 87, 0, 6, 48, 10, 0, 0, 0},
    {9467, 1021, 4, 0, 120, 38, 0, 0, 22},
    {422333, 131393, 0, 7795, 60032, 15492, 0, 0, 5},
    {15833292, 2046173, 6512, 0, 329464, 200568, 0, 0, 50562},
    {706045033, 210369132, 212, 10882006, 81102984, 26973664, 0, 0, 81076},
};

perft_results_t position_5_results[5] = {
    {44,0,0,0,0,0,0,0,0}, {1486,0,0,0,0,0,0,0,0}, {62379,0,0,0,0,0,0,0,0}, {2103487,0,0,0,0,0,0,0,0}, {89941194,0,0,0,0,0,0,0,0}};

const char* positions[] = {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"};

const perft_results_t* results_tables[] = {default_board_results,
                                           Kiwipete_results, position_3_results,position_4_results,
                                           position_5_results};

typedef struct {
  const char* const position;
  const perft_results_t* const results_table;
  int depth_of_first_row;
  int depth_of_largest_row;
  perft_active_columns flags;
} perft_test_t;

void test_perft(perft_test_t test_case) {
  printf("TESTING %s\n", test_case.position);
  load_position(&CHESS_STATE, test_case.position);
  for (int i = test_case.depth_of_first_row;
       i <= test_case.depth_of_largest_row; i++) {
    perft_results_t got = perft(&CHESS_STATE, i, test_case.flags);
    if (!compare_perft_results(
            got, test_case.results_table[i - test_case.depth_of_first_row],
            test_case.flags)) {
      printf("\nFAILED DEPTH %d expected:\n\n", i);
      print_perft_results(
          test_case.results_table[i - test_case.depth_of_first_row],
          test_case.flags);
      printf("\ngot:\n\n");
      print_perft_results(got, test_case.flags);
    } else {
      printf("\nPASSED %d\n", i);
    }
  }
}

int main() {
  const perft_test_t test_cases[] = {
      {positions[0], results_tables[0], 0, 5, PERFT_ALL},
      {positions[1], results_tables[1], 1, 5, PERFT_ALL},
      {positions[2], results_tables[2], 1, 5, PERFT_ALL},
      {positions[3], results_tables[3], 1, 5, PERFT_NODES|PERFT_CAPTURES|PERFT_CASTLES|PERFT_ENPASSENT|PERFT_CHECKS|PERFT_CHECKMATE|PERFT_PROMOTIONS},
      {positions[4], results_tables[4], 1, 5, PERFT_JUST_NODES},
  };

  test_perft(test_cases[0]);
  test_perft(test_cases[1]);
  test_perft(test_cases[2]);
  test_perft(test_cases[3]);
  test_perft(test_cases[4]);

  return 0;
}