#include "../../include/bot.h"


int rootSearch(worker_t* worker, centipawn_t alpha, centipawn_t beta, int depth) {
  (void)beta;
  // aliasing thread data
  chess_state_t* position = &worker->position;
  atomic_fetch_add(&worker->bot->nodes_searched, 1);

  move_t* moves = worker->moves;
  size_t move_count = worker->move_count;
  centipawn_t* scores = worker->scores;

  make_move(position, moves[0]);
  scores[0] = -abSearch(worker, MINSCORE, MAXSCORE, depth - 1);
  unmake_move(position);

  for (size_t i = 1; !stop(worker) && i < move_count; i++) {
    make_move(position, moves[i]);
    scores[i] = -abSearch(worker, -scores[0] - 1, -scores[0], depth - 1);
    if (scores[i] > alpha) {
      scores[i] = -abSearch(worker, MINSCORE, -scores[0], depth - 1);
    }
    unmake_move(position);

    if (stop(worker)) break;

    for (int j = i; j > 0; j--) {
      if (scores[j - 1] < scores[j]) {
        centipawn_t temp_score = scores[j - 1];
        scores[j - 1] = scores[j];
        scores[j] = temp_score;

        move_t temp_move = moves[j - 1];
        moves[j - 1] = moves[j];
        moves[j] = temp_move;
        continue;
      }
      break;
    }
  }
  return 0;
}

centipawn_t abSearch(worker_t* worker, centipawn_t alpha, centipawn_t beta,
                    int depth) {
  // aliasing thread data
  chess_state_t* position = &worker->position;
  table_t* table = &worker->bot->transpostion_table;
  int ply = position->ply_counter- worker->root_ply;


  if (depth <= 0 && !is_check(position)) {
    return qSearch(worker, alpha, beta, depth);
  }
  
  atomic_fetch_add(&worker->bot->nodes_searched, 1);

  if (is_draw_by_50_move_rule(position) || is_draw_by_insufficient_material(position) ||
      is_repetition(position, worker->root_ply)) {
    return DRAW_SCORE;
  }

  entry_t tt_entry = tt_get(table, position->zobrist);
  move_t hash_move = null_move;
  if (tt_entry) {
    int tt_entry_type = entry_type(tt_entry);
    centipawn_t tt_score = entry_score(tt_entry);
    if (entry_depth(tt_entry) >= depth) {
      if (tt_entry_type == TT_EXACT ||
          (tt_entry_type == TT_UPPER && tt_score <= alpha) ||
          (tt_entry_type == TT_LOWER && tt_score >= beta)) {
        return tt_score;
      }
    }
    hash_move = entry_best_move(tt_entry);
  }

  move_list_t move_list;
  init_move_list(position, &move_list, hash_move, worker->killer_moves[depth], worker->history_heuristic, worker->butterfly_heuristic);

  move_t best_move = next_move(position, &move_list);

  if (is_null_move(best_move)) {
    if (is_check(position)) {
      return CHECKMATE_SCORE - depth;
    } else {
      return DRAW_SCORE;
    }
  }

  make_move(position, best_move);
  centipawn_t best_score = -abSearch(worker, -beta, -alpha, depth - 1);
  unmake_move(position);

  if (!is_capture(best_move) && !is_promotion(best_move)) {
    inc_butteryfly_board(worker->butterfly_heuristic, best_move);
  }
  if (best_score > alpha) {
    if (best_score >= beta) {
      tt_store_depth_prefered(table, position->zobrist, TT_LOWER, best_move,
                              best_score, depth, 0);
      if (!is_capture(best_move) && !is_promotion(best_move)) {
        inc_butteryfly_board(worker->history_heuristic, best_move);
        add_killer_move(worker->killer_moves[ply], best_move);
      }
      return best_score;
    }
    alpha = best_score;
  }
  move_t move;
  while (!is_null_move(move = next_move(position, &move_list)) &&
         !stop(worker)) {
    make_move(position, move);
    centipawn_t score = -abSearch(worker, -alpha - 1, -alpha, depth - 1);
    if (score > alpha && score < beta) {
      score = -abSearch(worker, -beta, -alpha, depth - 1);
      if (score > alpha) {
        alpha = score;
      }
    }
    unmake_move(position);
    if (score > best_score) {
      if (score > alpha) {
        alpha = score;
      }
      if (!is_capture(best_move) && !is_promotion(best_move)) {
        inc_butteryfly_board(worker->butterfly_heuristic, best_move);
      }
      if (score >= beta) {
        tt_store_depth_prefered(table, position->zobrist, TT_LOWER, move, score,
                                depth, 0);
        if (!is_capture(move) && !is_promotion(move)) {
          inc_butteryfly_board(worker->history_heuristic, move);
          add_killer_move(worker->killer_moves[ply], move);
        }
        return score;
      }
      best_score = score;
      best_move = move;
    }
  }
  enum tt_entry_type entry_type = (best_score > alpha) ? TT_EXACT : TT_UPPER;
  tt_store_depth_prefered(table, position->zobrist, entry_type, best_move,
                          best_score, depth, 0);
  return best_score;
}


centipawn_t qSearch(worker_t* worker, centipawn_t alpha, centipawn_t beta,
                   int depth) {
  chess_state_t* position = &worker->position;

  if (is_check(position)) {
    return abSearch(worker, alpha, beta, depth);
  }

  atomic_fetch_add(&worker->bot->nodes_searched, 1);

  if (is_draw(position)) {
    return DRAW_SCORE;
  }

  move_list_t move_list;
  init_move_list(position, &move_list, null_move, worker->killer_moves[depth], worker->history_heuristic, worker->butterfly_heuristic);

  centipawn_t stand_pat = eval(position);

  if (stand_pat >= beta) {
    return stand_pat;
  }

  //if (stand_pat + 950 < alpha) {
  //  return stand_pat;
  //}

  if (stand_pat > alpha) {
    alpha = stand_pat;
  }


  centipawn_t best_score = stand_pat;
  move_t move;
  while (!is_null_move(move = next_capture(position, &move_list)) && !stop(worker)) {
    if (!is_promotion(move) && static_exchange_evaluation(position, move) < 0) {
      continue;
    }

    //if (stand_pat + piece_value(move.to, piece(position, move.to)) < alpha - 200) {
    //  continue;
    //}

    make_move(position, move);
    centipawn_t score = -qSearch(worker, -beta, -alpha, depth - 1);
    unmake_move(position);

    if (score >= beta) {
      return score;
    }

    if (score > best_score) {
      best_score = score;
      if (score > alpha) {
        alpha = score;
      }
    }
  }

  return best_score;
}