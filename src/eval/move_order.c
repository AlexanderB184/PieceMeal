#include "../../include/bot.h"
#include <string.h>

#define FALLTHROUGH __attribute__((fallthrough))

void reset_butterfly_board(int16_t* board) {
  memset(board, 0, sizeof(int16_t) * BUTTERFLY_BOARD_SIZE);
}

void add_killer_move(compact_move_t* killer_moves, move_t move) {
  compact_move_t compressed_move = compress_move(move);
  if (compressed_move == killer_moves[0]) return;
  for (int i = MAX_KILLER_MOVES - 1; i > 0; i--) {
    killer_moves[i] = killer_moves[i - 1];
  }
  killer_moves[0] = compressed_move;
}


void generate_capture_priorities(const chess_state_t* position,
                                 move_list_t* move_list) {
  for (size_t i = 0; i < move_list->move_count; i++) {
    uint16_t prio = PRIORITY_QUIET_MOVE;
    move_t move = move_list->moves[i];
    if (is_promotion(move)) {
      prio = PRIORITY_WINNING_CAPTURE + value_of(piece(position, move.to)) +
             value_of(get_promotes_to(move));
    } else if (is_capture(move)) {
      // SEE reduces the number of nodes search substantially but still
      // increases run time potentially it will be worth re-enabling if
      // additional features cause the eval time to increase and therefor the
      // better branch efficiency will make up for the overhead
      /*int see = static_exchange_evaluation(position, move);
      if (see > 0) {
        prio = PRIORITY_WINNING_CAPTURE;
      } else if (see == 0) {
        prio = PRIORITY_NEUTRAL_CAPTURE;
      } else {
        prio = PRIORITY_LOSING_CAPTURE;
      }*/
      prio = PRIORITY_NEUTRAL_CAPTURE;
      prio += value_of(piece(position, move.to)) -
              value_of(piece(position, move.from));
      move_list->moves[i] = set_priority(move, prio);
    }
  }
}

void generate_quiet_priorities(const chess_state_t* position,
                               move_list_t* move_list) {
  (void)position;
  for (size_t i = 0; i < move_list->move_count; i++) {
    uint16_t prio = PRIORITY_QUIET_MOVE;
    move_t move = move_list->moves[i];
    if (get_butteryfly_board(move_list->bh, move) == 0) {
      prio = 0;
    } else {
      prio = get_butteryfly_board(move_list->hh, move) * 256 /
              get_butteryfly_board(move_list->bh, move);
    }
    move_list->moves[i] = set_priority(move, prio);
  }
}

void init_move_list(const chess_state_t* position, move_list_t* move_list,
                    move_t hash_move, compact_move_t* killer_moves, int16_t* hh,
                    int16_t* bh) {
  (void)position;
  *move_list = (move_list_t){
      .move_count = 0,
      .hash_move = hash_move,
      .killer_moves = killer_moves,
      .hh = hh,
      .bh = bh,
      .state = PRIORITY_PV_MOVE,
  };
}

move_t select_best_move(move_list_t* move_list) {
  size_t selected_idx = 0;
  move_t selected_move;
  int max_priority = get_priority(move_list->moves[0]);
  for (size_t i = 1; i < move_list->move_count; i++) {
    int prio = get_priority(move_list->moves[i]);
    if (prio >= max_priority) {
      max_priority = prio;
      selected_idx = i;
    }
  }
  selected_move = move_list->moves[selected_idx];
  move_list->moves[selected_idx] =
      move_list->moves[--move_list->move_count];
  return selected_move;
}

move_t next_capture(const chess_state_t* position, move_list_t* move_list) {
  move_t selected_move;
  do {
    switch (move_list->state) {
      case PRIORITY_PV_MOVE:
        if (is_pseudo_legal(position, move_list->hash_move)) {
          selected_move = move_list->hash_move;
          move_list->state = PRIORITY_WINNING_CAPTURE;
          break;
        }
        move_list->state = PRIORITY_WINNING_CAPTURE;
        FALLTHROUGH;
      case PRIORITY_WINNING_CAPTURE:
        move_list->move_count = generate_promotions(position, move_list->moves,
                                                    position->friendly_colour);
        generate_capture_priorities(position, move_list);
        move_list->state = PRIORITY_NEUTRAL_CAPTURE;
        FALLTHROUGH;
      case PRIORITY_NEUTRAL_CAPTURE:

        if (move_list->move_count > 0) {
          selected_move = select_best_move(move_list);
          break;
        }
        move_list->state = PRIORITY_LOSING_CAPTURE;
        move_list->move_count = generate_captures(position, move_list->moves,
                                                  position->friendly_colour);
        generate_capture_priorities(position, move_list);
        FALLTHROUGH;
      case PRIORITY_LOSING_CAPTURE:
        if (move_list->move_count > 0) {
          selected_move = select_best_move(move_list);
          break;
        }
        move_list->state = PRIORITY_KILLER_MOVES;
        for (int i = 0; i < MAX_KILLER_MOVES; i++) {
          move_t killer = uncompress_move(move_list->killer_moves[i]);
          if (is_pseudo_legal(position, killer)) {
            move_list->moves[move_list->move_count++] = killer;
          }
        }
        FALLTHROUGH;
        default:
        return null_move;
    }
  } while (!is_legal(position, selected_move));
  return selected_move;
}

move_t next_move(const chess_state_t* position, move_list_t* move_list) {
  move_t selected_move;
  do {
    switch (move_list->state) {
      case PRIORITY_PV_MOVE:
        if (is_pseudo_legal(position, move_list->hash_move)) {
          selected_move = move_list->hash_move;
          move_list->state = PRIORITY_WINNING_CAPTURE;
          break;
        }
        move_list->state = PRIORITY_WINNING_CAPTURE;
        FALLTHROUGH;
      case PRIORITY_WINNING_CAPTURE:
        move_list->move_count = generate_promotions(position, move_list->moves, position->friendly_colour);
        generate_capture_priorities(position, move_list);
        move_list->state = PRIORITY_NEUTRAL_CAPTURE;
        FALLTHROUGH;
      case PRIORITY_NEUTRAL_CAPTURE:
        if (move_list->move_count > 0) {
          selected_move = select_best_move(move_list);
          break;
        }
        move_list->state = PRIORITY_LOSING_CAPTURE;
        move_list->move_count = generate_captures(position, move_list->moves, position->friendly_colour);
        generate_capture_priorities(position, move_list);
        FALLTHROUGH;
      case PRIORITY_LOSING_CAPTURE:
        if (move_list->move_count > 0) {
          selected_move = select_best_move(move_list);
          break;
        }
        move_list->state = PRIORITY_KILLER_MOVES;
        for (int i = 0; i < MAX_KILLER_MOVES; i++) {
          move_t killer = uncompress_move(move_list->killer_moves[i]);
          if (is_pseudo_legal(position, killer)) {
            move_list->moves[move_list->move_count++] = killer;
          }
        }
        FALLTHROUGH;
        case PRIORITY_KILLER_MOVES:
        if (move_list->move_count > 0) {
            selected_move = select_best_move(move_list);
          break;
        }
        move_list->move_count = generate_quiets(position, move_list->moves, position->friendly_colour);
        generate_quiet_priorities(position, move_list);
        move_list->state = PRIORITY_QUIET_MOVE;
        FALLTHROUGH;
      default: {
        if (move_list->move_count == 0) return null_move;
        selected_move = select_best_move(move_list);
      } break;
    }

  } while (!is_legal(position, selected_move));
  return selected_move;
}