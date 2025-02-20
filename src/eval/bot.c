#include "../../include/bot.h"

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BOT_THREAD_COUNT_DEFAULT 1
#define BOT_HASH_SIZE_MB_DEFAULT 4
#define BOT_CAN_PONDER_DEFAULT 0

int bot_init(bot_t* bot, bot_settings_t* settings) {
  *bot = (bot_t){
      .abort = 1,
      .running = 0,
      .search_mode = SEARCHMODE_INACTIVE,
      .n_threads = BOT_THREAD_COUNT_DEFAULT,
  };
  if (settings) {
    bot->settings = *settings;
  } else {
    bot->settings = (bot_settings_t){
        .debug = 0,
        .Hash = BOT_HASH_SIZE_MB_DEFAULT,
        .Ponder = BOT_CAN_PONDER_DEFAULT,
    };
  }
  tt_init(&bot->transpostion_table, bot->settings.Hash * (1024 * 1024));
  bot->workers = calloc(bot->n_threads, sizeof(worker_t*));
  for (int id = 0; id < bot->n_threads; id++) {
    bot->workers[id] = calloc(1, sizeof(worker_t));
    *bot->workers[id] = (worker_t){.id = id, .bot = bot, .terminated = 1};
  }
  return 0;
}

int bot_load_fen(bot_t* bot, const char* pos_text) {
  chess_state_t* root_position = &bot->root_position;
  return load_position(root_position, pos_text);
}

int bot_load_move(bot_t* bot, const char* movetext) {
  chess_state_t* root_position = &bot->root_position;
  long bytes_to_read = strlen(movetext);
  move_t move;
  long out = read_long_algebraic_notation(movetext, bytes_to_read, root_position, &move);

  if (out == -1) {
    release_position(root_position);
    return -1;
  }
  make_move(root_position, move);
  
  return out;
}

int bot_start(bot_t* bot) {
  clock_gettime(CLOCK_MONOTONIC, &bot->start_time);

  atomic_store(&bot->nodes_searched, 0);
  atomic_store(&bot->depth_searched, 0);
  atomic_store(&bot->duration_ms, 0);
  atomic_store(&bot->abort, 0);
  atomic_store(&bot->running, 1);

  for (int i = 0; i < bot->n_threads; i++) {
    if (pthread_create(&bot->workers[i]->handle, NULL, worker_start,
                       bot->workers[i])) {
      bot_stop(bot);
      return -1;
    }
  }

  return 0;
}

int bot_ponder_hit(bot_t* bot) {
  clock_gettime(CLOCK_MONOTONIC, &bot->start_time);

  bot->search_mode = SEARCHMODE_REGULAR;
  
  atomic_store(&bot->nodes_searched, 0);
  atomic_store(&bot->depth_searched, 0);
  atomic_store(&bot->duration_ms, 0);
  
  return 0;
}

int bot_stop(bot_t* bot) {
  
  atomic_store(&bot->abort, 1);
  return 0;
}

int bot_wait(bot_t* bot) {
  if (!bot_is_running(bot)) return 0;
  for (int i = 0; i < bot->n_threads; i++) {
    pthread_join(bot->workers[i]->handle, NULL);
  }
  return 0;
}

int bot_is_running(bot_t* bot) {
  if (atomic_load(&bot->running)) return 1;
  return 0;
}

int bot_release(bot_t* bot) {
  bot_stop(bot);
  bot_wait(bot);
  for (int i = 0; i < bot->n_threads; i++) {
    free(bot->workers[i]);
  }
  tt_free(&bot->transpostion_table);
  free(bot->workers);
  return 0;
}
