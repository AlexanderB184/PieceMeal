#include "../include/notation.h"

#include <stdio.h>
#include <string.h>

#include "../include/chess-lib.h"
/*
int write_square(char* buffer, size_t buffer_size, sq0x88_t square) {
  if (buffer_size < 2) {
    return 0;
  }
  if (off_the_board(square)) {
    buffer[0] = '0';
    buffer[1] = '0';
    return 2;
  }
  buffer[0] = sq0x88_to_file07(square) + 'a';
  buffer[1] = sq0x88_to_rank07(square) + '1';
  return 2;
}

sq0x88_t read_square(const char* buffer, size_t buffer_size) {
  if (buffer_size < 2) {
    return 0x88;
  }
  if (buffer[0] == '0' && buffer[1] == '0') {
    return 0x88;
  }
  return rankfile_to_sq0x88(buffer[1] - '1', buffer[0] - 'a');
}

int write_piece(char* buffer, size_t buffer_size, piece_t piece) {
  if (buffer_size < 1) {
    return 0;
  }
  switch (piece & PIECE_MASK) {
    default:
      return 0;
    case PAWN:
      buffer[0] = 'P';
      break;
    case KNIGHT:
      buffer[0] = 'N';
      break;
    case BISHOP:
      buffer[0] = 'B';
      break;
    case ROOK:
      buffer[0] = 'R';
      break;
    case QUEEN:
      buffer[0] = 'Q';
      break;
    case KING:
      buffer[0] = 'K';
      break;
  }
  return 1;
}

piece_t read_piece(const char* buffer, size_t buffer_size) {
  if (buffer_size < 1) {
    return EMPTY;
  }
  switch (buffer[0]) {
    case 'P':
      return PAWN;
    case 'N':
      return KNIGHT;
    case 'B':
      return BISHOP;
    case 'R':
      return ROOK;
    case 'Q':
      return QUEEN;
    case 'K':
      return KING;
    default:
      return EMPTY;
  }
}

int write_algebraic_notation(char* buffer, size_t buffer_size,
                             const chess_state_t* chess_state, move_t move) {
  int index = 0;
  if (buffer_size < 16) return 0;
  move_t moves[256];
  size_t move_count = generate_legal_moves(chess_state, moves);
  if (is_king_castle(move)) {
    strncpy(buffer, "O-O", buffer_size);
    index += 3;
  } else if (is_queen_castle(move)) {
    strncpy(buffer, "O-O-O", buffer_size);
    index += 5;
  } else {
    piece_t moving_piece = piece(chess_state, move.from);
    if (!(moving_piece & PAWN)) {
      index = write_piece(buffer + index, buffer_size - index, moving_piece);
      int disambiguate_file = 0;
      int disambiguate_rank = 0;
      for (size_t i = 0; i < move_count; i++) {
        if (moves[i].to == move.to && moves[i].from != move.from &&
            piece(chess_state, moves[i].from) ==
                piece(chess_state, move.from)) {
          if (sq0x88_to_file07(moves[i].from) != sq0x88_to_file07(move.from)) {
            disambiguate_file = 1;
          }
          if (sq0x88_to_file07(moves[i].from) == sq0x88_to_file07(move.from) &&
              sq0x88_to_rank07(moves[i].from) != sq0x88_to_rank07(move.from)) {
            disambiguate_rank = 1;
          }
        }
      }
      if (disambiguate_file) {
        buffer[index++] = sq0x88_to_file07(move.from) + 'a';
      }
      if (disambiguate_rank) {
        buffer[index++] = sq0x88_to_rank07(move.from) + '1';
      }
    }
    if (is_capture(move)) {
      if (moving_piece & PAWN) {
        buffer[index++] = sq0x88_to_file07(move.from) + 'a';
      }
      buffer[index++] = 'x';
    }
    index += write_square(buffer + index, buffer_size - index, move.to);
  }
  if (is_promotion(move)) {
    buffer[index++] = '=';
    index +=
        write_piece(buffer + index, buffer_size - index, get_promotes_to(move));
  }
  if (is_check(chess_state)) {
    if (is_checkmate(chess_state)) {
      buffer[index++] = '#';
    } else {
      buffer[index++] = '+';
    }
  }

  return index;
}

move_t read_algebraic_notation(const char* buffer, size_t buffer_size,
                               const chess_state_t* chess_state) {
  int index = 0;
  move_t moves[256];
  size_t move_count = generate_legal_moves(chess_state, moves);
  piece_t moving_piece = read_piece(buffer + index, buffer_size - index);
  if (moving_piece == EMPTY) {
    moving_piece = PAWN;
  } else {
    index++;
  }
  file07_t file = -1;
  rank07_t rank = -1;
  sq0x88_t from = 0x88;
  sq0x88_t to = 0x88;
  int capture = 0;
  if (buffer[index] >= 'a' && buffer[index] <= 'h') {
    file = buffer[index++] - 'a';
  }
  if (buffer[index] >= '1' && buffer[index] <= '8') {
    rank = buffer[index++] - '1';
  }
  if (buffer[index] == 'x') {
    capture = 1;
    index++;
  }
  if (buffer[index] >= 'a' && buffer[index] <= 'h') {
    to = read_square(buffer + index, buffer_size);
    index += 2;
  } else {
    to = rankfile_to_sq0x88(rank, file);
  }

  if (file != -1 && rank != -1) {
    from = rankfile_to_sq0x88(rank, file);
  } else {
    for (size_t i = 0; i < move_count; i++) {
      if (moves[i].to == to &&
          piece(chess_state, moves[i].from) == moving_piece) {
        if (file != -1 && file == sq0x88_to_file07(moves[i].from)) {
          from = moves[i].from;
          break;
        }
        if (rank != -1 && rank == sq0x88_to_file07(moves[i].from)) {
          from = moves[i].from;
          break;
        }
        if (rank == -1 && file == -1) {
          from = moves[i].from;
          break;
        }
      }
    }
  }
  piece_t promote_to = EMPTY;
  if (buffer[index++] == '=') {
    promote_to = read_piece(buffer + index, buffer_size - index);
    index++;
  }
  uint16_t flags = QUIET_MOVE;
  if (capture) {
    flags |= CAPTURE;
    if (enpassent_target(chess_state) == to - chess_state->up_increment) {
      flags = ENPASSENT;
    }
  }
  if (is_promoting(chess_state, from)) {
    flags |= PROMOTION;
    // parse promote to
    switch (promote_to & PIECE_MASK) {
      case KNIGHT:
        flags |= KNIGHT_PROMOTE_TO;
        break;
      case BISHOP:
        flags |= BISHOP_PROMOTE_TO;
        break;
      case ROOK:
        flags |= ROOK_PROMOTE_TO;
        break;
      case QUEEN:
        flags |= QUEEN_PROMOTE_TO;
        break;
      default:
        break;
    }
  }
  if (piece(chess_state, from) & KING) {
    if (from == to + 2) {
      flags = KING_CASTLE;
    }
    if (from == to - 2) {
      flags = QUEEN_CASTLE;
    }
  }
  if (to - from == 2 * chess_state->up_increment &&
      piece(chess_state, from) & PAWN) {
    flags = DOUBLE_PAWN_PUSH;
  }
  return move(from, to, flags);
}

int write_long_algebraic_notation(char* buffer, size_t buffer_size,
                                  move_t move) {
  if (buffer_size < 4) {
    return 0;
  }
  if (is_null_move(move)) {
    buffer[0] = '0';
    buffer[1] = '0';
    buffer[2] = '0';
    buffer[3] = '0';
    return 4;
  }
  int index = 0;

  index += write_square(buffer + index, buffer_size - index, get_from(move));
  index += write_square(buffer + index, buffer_size - index, get_to(move));

  if (is_promotion(move) && buffer_size >= 5) {
    index +=
        write_piece(buffer + index, buffer_size - index, get_promotes_to(move));
  }

  return index;
}

move_t read_long_algebraic_notation(const char* buffer, size_t buffer_size,
                                    const chess_state_t* chess_state) {
  if (buffer_size < 4) {
    return null_move;
  }
  sq0x88_t from = read_square(buffer, buffer_size);
  sq0x88_t to = read_square(buffer + 2, buffer_size - 2);
  piece_t promote_to = EMPTY;
  if (buffer_size >= 5 && buffer[4] >= 'A' && buffer[4] <= 'Z') {
    promote_to = read_piece(buffer - 4, buffer_size - 4);
  }
  uint16_t flags = QUIET_MOVE;
  if (piece(chess_state, to) != EMPTY) {
    flags |= CAPTURE;
    if (enpassent_target(chess_state) == to - chess_state->up_increment) {
      flags = ENPASSENT;
    }
  }
  if (is_promoting(chess_state, from)) {
    flags |= PROMOTION;
    // parse promote to
    switch (promote_to & PIECE_MASK) {
      case KNIGHT:
        flags |= KNIGHT_PROMOTE_TO;
        break;
      case BISHOP:
        flags |= BISHOP_PROMOTE_TO;
        break;
      case ROOK:
        flags |= ROOK_PROMOTE_TO;
        break;
      case QUEEN:
        flags |= QUEEN_PROMOTE_TO;
        break;
      default:
        break;
    }
  }
  if (piece(chess_state, from) & KING) {
    if (from == to + 2) {
      flags = KING_CASTLE;
    }
    if (from == to - 2) {
      flags = QUEEN_CASTLE;
    }
  }
  if (to - from == 2 * chess_state->up_increment &&
      piece(chess_state, from) & PAWN) {
    flags = DOUBLE_PAWN_PUSH;
  }
  return move(from, to, flags);
}

size_t write_movetext(char* buffer, size_t buffer_size,
                      const chess_state_t* chess_state) {
  chess_state_t cloned_state = {};
  
  load_start_position(&cloned_state);

  int index = 0;

  for (int i = 0; i < chess_state->ply_counter; i++) {
    size_t capacity = buffer_size - 1 - index;
    if (i % 2 == 0) {
      index += snprintf(buffer + index, capacity, "%d. ",
                        1 + (cloned_state.ply_counter / 2));
      capacity = buffer_size - 1 - index;
    }
    index += write_algebraic_notation(buffer + index, capacity, &cloned_state,
                                      chess_state->ply_stack[i].move);
    make_move(&cloned_state, chess_state->ply_stack[i].move);
    capacity = buffer_size - 1 - index;
    if (capacity >= 1) {
      buffer[index++] = ' ';
    }
  }
  if (buffer_size - 1 - index >= 3) {
    if (is_draw(chess_state) || is_stalemate(chess_state)) {
      index += snprintf(buffer + index, buffer_size - index, "1/2-1/2");
    } else if (is_checkmate(chess_state)) {
      if (chess_state->black_to_move) {
        index += snprintf(buffer + index, buffer_size - index, "1-0");
      } else {
        index += snprintf(buffer + index, buffer_size - index, "0-1");
      }
    }
  }

  buffer[index++] = '\0';
  clear_position(&cloned_state);

  return index;
}

int write_pgn(char* buffer, size_t buffer_size,
              const chess_state_t* chess_state, const char* white_name,
              const char* black_name, const char* fen_initial_pos) {
  const char* result = "*";
  if (is_draw(chess_state) || is_stalemate(chess_state)) {
    result = "1/2-1/2";
  } else if (is_checkmate(chess_state)) {
    if (chess_state->black_to_move) {
      result = "1-0";
    } else {
      result = "0-1";
    }
  }

  int index = 0;
  index += snprintf(buffer + index, buffer_size - index,
                    "[Event \"?\"]\n"
                    "[Site \"?\"]\n"
                    "[Date \"????.??.??\"]\n"
                    "[Round \"?\"]\n"
                    "[White \"%s\"]\n"
                    "[Black \"%s\"]\n"
                    "[Result \"%s\"]\n",
                    white_name, black_name, result);

  if (fen_initial_pos != NULL) {
    index += snprintf(buffer + index, buffer_size - index, "[FEN \"%s\"]\n",
                      fen_initial_pos);
  }
  buffer[index++] = '\n';  // extra newline before movetext
  index += write_movetext(buffer + index, buffer_size - index, chess_state);
  return index;
}
*/