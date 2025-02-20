#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../include/bot.h"

#define AUTHOR "Alex B"
#define BOT_NAME "PieceMeal"
#define BOT_VERSION "1.2.0"

#define UNIMPLEMENTED fprintf(stderr, "unimplemented\n")
#define INVALIDARG(CMD) fprintf(stderr, "\"%.*s\" is not a valid argument for command \"%s\"\n", (arglen), (arg), (CMD))
#define MISSINGARG(CMD, ARG) fprintf(stderr, "command \"%s\" is missing required argument \"%s\"\n", (CMD), (ARG))
#define LOGERROR(msg, ...) fprintf(stderr, msg "\n", ##__VA_ARGS__)
#define FATALERROR(msg, ...) {LOGERROR(msg, ##__VA_ARGS__); exit(-1);}
#define TELLGUI(msg, ...) { fprintf(stdout, msg "\n", ##__VA_ARGS__); fflush(stdout); }

#define BUFFER_SIZE 256

bot_t bot;
const char * startpos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
int pos, arglen;
char * arg;
char buffer[BUFFER_SIZE];

void quit() {
  bot_release(&bot);
  exit(0);
}

int uci_is_whitespace(char c) {
  return c == ' ' || c == '\t';
}

int uci_is_newline(char c) {
  return c == '\n';
}

int uci_is_delim(char c) {
  return c == 0 || c == ' ' || c == '\t' || c == '\n';
}

int uci_strcmp(const char* lhs, const char* rhs) {
  while (!uci_is_delim(*lhs) && !uci_is_delim(*rhs)) {
    if (*lhs != *rhs) return 0;
    lhs++;
    rhs++;
  }
  return uci_is_delim(*lhs) && uci_is_delim(*rhs);
}

void uci_read_from_stdin(int keep) {
  if (keep == 0 && buffer[0]) {
    LOGERROR("argument too big, discarding first %d bytes", BUFFER_SIZE);
    keep = pos;
  }
  pos = pos - keep;
  memmove(buffer, &buffer[keep], pos);
  if (fgets(&buffer[pos], BUFFER_SIZE - pos, stdin) == NULL) {
    if (feof(stdin)) {
      LOGERROR("bot reached eof");
      quit();
    }
    FATALERROR("bot failed to read from stdin: %s", strerror(errno));
  }
}

void uci_next_arg() {

  do {

    while (buffer[pos] && uci_is_whitespace(buffer[pos])) {
      pos++;
    }

    if (buffer[pos] == 0) {
      uci_read_from_stdin(pos);
      continue;
    }
    break;
  } while (1);

  if (uci_is_newline(buffer[pos])) {
    pos++;
    arg = NULL;
    return;
  }

  int arg_start = pos;

  do {
    while (buffer[pos] && !uci_is_whitespace(buffer[pos]) && !uci_is_newline(buffer[pos])) {
        pos++;
    }
    if (buffer[pos] == 0) {
      uci_read_from_stdin(arg_start);
      arg_start = 0;
      continue;
    }
    
    break;
  } while (1);

  arglen = pos - arg_start;

  // printf("token %.*s\n", arglen, &buffer[arg_start]);
  arg = &buffer[arg_start];
}

void uci_handler() {
  if (bot_init(&bot, NULL)) {
    FATALERROR("failed to initialise bot " BOT_NAME " " BOT_VERSION);
  }
  TELLGUI("id name " BOT_NAME " " BOT_VERSION);
  TELLGUI("id author " AUTHOR);
  TELLGUI("uciok");
  uci_next_arg();
}

void debug_handler() {
  uci_next_arg();
  if (arg && uci_strcmp(arg, "on")) {
    bot.settings.debug = 1;
  } else if (arg && uci_strcmp(arg, "off")) {
    bot.settings.debug = 0;
  } else {
    MISSINGARG("debug", "on/off");
  }
}

void isready_handler() {
  TELLGUI("readyok");
  uci_next_arg();
}

void ucinewgame_handler() {
  bot_load_fen(&bot, startpos);
  uci_next_arg();
}

void position_handler() {
  uci_next_arg();
  if (arg && uci_strcmp(arg, "startpos")) {
    bot_load_fen(&bot, startpos);
    uci_next_arg();
  } else if (arg && uci_strcmp(arg, "fen")) {
    char FEN[BUFFER_SIZE];
    int fpos = 0;
    uci_next_arg();
    while (arg && !uci_strcmp(arg, "moves")) {
      if (fpos + arglen + 2 >= BUFFER_SIZE) {
        LOGERROR("fen too long");
        break;
      }
      memcpy(&FEN[fpos], arg, arglen);
      fpos += arglen;
      FEN[fpos++] = ' ';
      uci_next_arg();
    }
    FEN[fpos] = 0;
    int out = bot_load_fen(&bot, FEN);
    if (out < 0) {
      LOGERROR("invalid fen \"%s\"", arg);
      return;
    }
  } else {
    MISSINGARG("position", "fen/startpos");
  }

  if (arg && uci_strcmp(arg, "moves")) {
    uci_next_arg();

    while (arg) {
      if (bot_load_move(&bot, arg) < 0) {
        LOGERROR("invalid move in movetext \"%s\"", arg);
        return;
      }
      uci_next_arg();
    }
  }
}

void go_handler() {
  bot.search_mode = SEARCHMODE_REGULAR;
  bot.stop_cond = (bot_term_cond_t){0, 0, 0, 0};
  uci_next_arg();
  while (arg) {
    if (uci_strcmp(arg, "searchmoves")) {
      UNIMPLEMENTED;
    } else if (uci_strcmp(arg, "ponder")) {
      if (bot.settings.Ponder != 1) {
        LOGERROR("cannot ponder, ponder mode is disabled");
        return; // do not ponder
      } else {
        bot.search_mode = SEARCHMODE_PONDER;
      }
    } else if (uci_strcmp(arg, "wtime")) {
      
      uci_next_arg();
      if (arg) {
      bot.match_info.wtime = atoll(arg);
      } else {
        MISSINGARG("wtime", "<time ms>");
      }
    } else if (uci_strcmp(arg, "btime")) {
      uci_next_arg();
      if (arg) {
        bot.match_info.btime = atoll(arg);
      } else {
        MISSINGARG("btime", "<time ms>");
      }
    } else if (uci_strcmp(arg, "winc")) {
      uci_next_arg();
      if (arg) {
        bot.match_info.winc = atoll(arg);
      } else {
        MISSINGARG("winc", "<inc ms>");
      }
    } else if (uci_strcmp(arg, "binc")) {
      uci_next_arg();
      if (arg) {
        bot.match_info.binc = atoll(arg);
      } else {
        MISSINGARG("binc", "<inc ms>");
      }
    } else if (uci_strcmp(arg, "movestogo")) {
      UNIMPLEMENTED;
      uci_next_arg();
    } else if (uci_strcmp(arg, "depth")) {
      uci_next_arg();
      if (arg) {
        bot.stop_cond.depth_limit_ply = atoll(arg);
      }
    } else if (uci_strcmp(arg, "nodes")) {
      uci_next_arg();
      if (arg) {
        bot.stop_cond.node_limit_nds = atoll(arg);
      }
    } else if (uci_strcmp(arg, "mate")) {
      uci_next_arg();
      if (arg) {
        bot.stop_cond.mate_in_ply = atoll(arg);
      }
    } else if (uci_strcmp(arg, "movetime")) {
      uci_next_arg();
      if (arg) {
        bot.stop_cond.time_limit_ms = atoll(arg);
      }
    } else if (uci_strcmp(arg, "infinite")) {
      bot.stop_cond.depth_limit_ply = 0;
      bot.stop_cond.node_limit_nds = 0;
      bot.stop_cond.time_limit_ms = 0;
      bot.stop_cond.mate_in_ply = 0;
    } else {
      INVALIDARG("go");
    }
    uci_next_arg();
  }

  bot_start(&bot);
}

void stop_handler() {
  bot_stop(&bot);
  bot_wait(&bot);
  uci_next_arg();
}

void ponderhit_handler() {
  bot_ponder_hit(&bot);
  uci_next_arg();
}

void handle_command() {
  const char * cmd;
  do {

    uci_next_arg();

    if (arg == NULL) {
      break;
    }

    if (uci_strcmp(arg, "uci")) {
      cmd = "uci";
      uci_handler();

    } else if (uci_strcmp(arg, "debug")) {
      cmd = "debug";
      debug_handler();

    } else if (uci_strcmp(arg, "isready")) {
      cmd = "isready";
      isready_handler();

    } else if (uci_strcmp(arg, "setoption")) {
      cmd = "setoption";
      UNIMPLEMENTED;

    } else if (uci_strcmp(arg, "register")) {
      cmd = "register";
      UNIMPLEMENTED;

    } else if (uci_strcmp(arg, "ucinewgame")) {
      cmd = "ucinewgame";
      ucinewgame_handler();

    } else if (uci_strcmp(arg, "position")) {
      cmd = "position";
      position_handler();

    } else if (uci_strcmp(arg, "go")) {
      cmd = "go";
      go_handler();
      
    } else if (uci_strcmp(arg, "stop")) {
      cmd = "stop";
      stop_handler();

    } else if (uci_strcmp(arg, "ponderhit")) {
      cmd = "ponderhit";
      ponderhit_handler();

    } else if (uci_strcmp(arg, "quit")) {
      quit();

    } else {
      LOGERROR("\"%.*s\" is not a valid command", arglen, arg);
      continue;
    }
    
    break;
  } while (1); // loop until we find a valid command

  while (arg) {
    INVALIDARG(cmd);
    uci_next_arg();
  }

}

int main(void) {
  for (;;) {
    handle_command(buffer);
  }
}