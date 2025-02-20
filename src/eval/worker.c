#include "../../include/bot.h"

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdatomic.h>

void bot_on_stop(bot_t* bot) {
    for (int id = 1; id < bot->n_threads; id++) {
        pthread_join(bot->workers[id]->handle, NULL);
    }
    log_info(bot);
    bestmove(bot->workers[0]->moves[0], null_move);
    atomic_store(&bot->running, 0);
}

void* worker_start(void* arg) {
  worker_t* worker = (worker_t*)arg;
  bot_t* bot = worker->bot;


  copy_position(&worker->position, &bot->root_position);
  worker->move_count = generate_legal_moves(&worker->position, worker->moves, bot->root_position.friendly_colour);
  worker->root_ply = worker->position.ply_counter;
  reset_butterfly_board(worker->history_heuristic);
  reset_butterfly_board(worker->butterfly_heuristic);

  while (!stop(worker)) {
    rootSearch(worker, MINSCORE, MAXSCORE, bot->depth_searched);
    bot->depth_searched++;
  }

  release_position(&worker->position);
  worker->terminated = 1;

  if (is_main_thread(worker)) {
    bot_on_stop(bot);
  }
  
  return NULL;
}

long time_passed(const struct timespec* start_time) {
  struct timespec curr_time;
  clock_gettime(CLOCK_MONOTONIC, &curr_time);
  long delta_s = (curr_time.tv_sec - start_time->tv_sec);
  long delta_ns = (curr_time.tv_nsec - start_time->tv_nsec);
  long duration_ms = delta_s * 1000 + delta_ns / (1000 * 1000);
  return duration_ms;
}

int stop(worker_t* worker) {
  bot_term_cond_t* stop_cond = &worker->bot->stop_cond;
  bot_t* bot = worker->bot;
  if (atomic_load(&bot->abort)) {
    return 1;
  }
  if (stop_cond->time_limit_ms > 0 &&
      time_passed(&bot->start_time) > stop_cond->time_limit_ms) {
    atomic_store(&bot->abort, 1);
    return 1;
  }
  if (stop_cond->node_limit_nds > 0 &&
      atomic_load(&bot->nodes_searched) > stop_cond->node_limit_nds) {
    atomic_store(&bot->abort, 1);
    return 1;
  }
  if (stop_cond->depth_limit_ply > 0 &&
      atomic_load(&bot->depth_searched) > stop_cond->depth_limit_ply) {
    atomic_store(&bot->abort, 1);
    return 1;
  }
  return 0;
}

int is_main_thread(const worker_t* worker) {
  return (uintptr_t)worker == (uintptr_t)(worker->bot->workers[0]);
}

void log_info(const bot_t* bot) {
  fprintf(stdout, "info depth %ld nodes %ld time %ld score %d\n",
          atomic_load(&bot->depth_searched), atomic_load(&bot->nodes_searched),
          time_passed(&bot->start_time), bot->workers[0]->scores[0]);
  fflush(stdout);
}

void bestmove(move_t bestmove, move_t ponder) {
  char buffer[8];
  write_long_algebraic_notation(buffer, 8, bestmove);
  fprintf(stdout, "bestmove %s", buffer);
  if (!is_null_move(ponder)) {
    write_long_algebraic_notation(buffer, 8, ponder);
    fprintf(stdout, " ponder %s", buffer);
  }
  fprintf(stdout, "\n");
  fflush(stdout);
}