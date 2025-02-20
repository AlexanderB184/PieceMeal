#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/chess.h"

#ifdef DEBUG
#define PRINT_READ_ERRORS
#define PRINT_WRITE_ERRORS
#endif

#ifdef PRINT_READ_ERRORS
#define READ_ERROR(msg, ...)                                \
  {                                                         \
    fprintf(stderr, "READ ERROR: reading \"%s\" ", buffer); \
    fprintf(stderr, msg, ##__VA_ARGS__);                    \
    return -1;                                              \
  }
#else
#define READ_ERROR(msg, ...) \
  { return -1; }
#endif

#ifdef PRINT_WRITE_ERRORS
#define WRITE_ERROR(msg, ...)         \
  {                                   \
    fprintf(stderr, "WRITE ERROR: "); \
    printf(msg, ##__VA_ARGS__);       \
    buffer[0] = 0;                    \
    return -1;                        \
  }
#else
#define WRITE_ERROR(msg, ...) \
  {                           \
    buffer[0] = 0;            \
    return -1;                \
  }
#endif

int is_file(char c) { return c >= 'a' && c <= 'h'; }

int is_rank(char c) { return c >= '1' && c <= '8'; }

long write_square(char* buffer, long buffer_size, sq0x88_t square) {
  if (buffer_size < 3) return -1;
  if (off_the_board(square)) {
    WRITE_ERROR("square is off the board, square HEX value: \'%02x\'.\n",
                square);
  }
  buffer[0] = sq0x88_to_file07(square) + 'a';
  buffer[1] = sq0x88_to_rank07(square) + '1';
  buffer[2] = '\0';
  return 2;
}
long read_square(const char* buffer, long buffer_size, sq0x88_t* square) {
  if (buffer_size < 2) return -1;
  int file = buffer[0];
  int rank = buffer[1];
  if (!is_file(file)) {
    READ_ERROR("letter \'%c\' is not a valid file\n", file);
  }
  if (!is_rank(rank)) {
    READ_ERROR("letter \'%c\' is not a valid rank\n", rank);
  }
  *square = rankfile_to_sq0x88(rank - '1', file - 'a');
  return 2;
}

long write_piece(char* buffer, long buffer_size, piece_t piece) {
  if (buffer_size < 2) return -1;
  char piece_char;
  switch (piece & PIECE_MASK) {
    case KING:
      piece_char = 'K';
      break;
    case QUEEN:
      piece_char = 'Q';
      break;
    case ROOK:
      piece_char = 'R';
      break;
    case BISHOP:
      piece_char = 'B';
      break;
    case KNIGHT:
      piece_char = 'N';
      break;
    case PAWN:
      piece_char = 'P';
      break;
    default:
      WRITE_ERROR("invalid piece type %02x\n.", piece);
  }
  buffer[0] = piece_char;
  buffer[1] = '\0';
  return 1;
}

long read_piece(const char* buffer, long buffer_size, piece_t* piece) {
  *piece = EMPTY;
  if (buffer_size < 1) return -1;
  char piece_char = buffer[0];
  switch (piece_char) {
    case 'P':
      *piece = PAWN;
      break;
    case 'N':
      *piece = KNIGHT;
      break;
    case 'B':
      *piece = BISHOP;
      break;
    case 'R':
      *piece = ROOK;
      break;
    case 'Q':
      *piece = QUEEN;
      break;
    default:
      READ_ERROR("letter \'%c\' is not a valid piece.\n", piece_char)
  }
  return 1;
}

void disambiguate_piece(chess_state_t* chess_state, sq0x88_t from, sq0x88_t to,
                        sq0x88_t* piece_list, int piece_count,
                        sq0x88_t (*increment_func)(sq0x88_t, sq0x88_t),
                        int* disambiguate_rank, int* disambiguate_file) {
  for (int i = 0; i < piece_count; i++) {
    sq0x88_t sq = piece_list[i];
    if (sq == from) continue;
    sq0x88_t inc = increment_func(sq, to);
    if (inc && backwards_ray_cast(chess_state, to, inc) == sq &&
        is_legal(chess_state, move(sq, to, QUIET_MOVE))) {
      if (sq0x88_to_file07(from) == sq0x88_to_file07(sq))
        *disambiguate_rank = 1;
      else if (sq0x88_to_rank07(from) == sq0x88_to_rank07(sq))
        *disambiguate_file = 1;
      else if (*disambiguate_file != 0)
        *disambiguate_rank = 1;
      else
        *disambiguate_file = 1;
    }
  }
}

void disambiguate_knight(chess_state_t* chess_state, sq0x88_t from, sq0x88_t to,
                         int* disambiguate_rank, int* disambiguate_file) {
  for (int i = 0; i < chess_state->friendly_pieces->knight_count; i++) {
    sq0x88_t knight_square = chess_state->friendly_pieces->knight_list[i];
    if (knight_square == from) continue;
    if (knight_increment(knight_square, to) != 0 &&
        is_legal(chess_state, move(knight_square, to, QUIET_MOVE))) {
      if (sq0x88_to_file07(from) == sq0x88_to_file07(knight_square))
        *disambiguate_rank = 1;
      else if (sq0x88_to_rank07(from) == sq0x88_to_rank07(knight_square))
        *disambiguate_file = 1;
      else if (*disambiguate_file != 0)
        *disambiguate_rank = 1;
      else
        *disambiguate_file = 1;
    }
  }
}

long write_algebraic_notation(char* buffer, long buffer_size,
                              chess_state_t* chess_state,
                              move_t move_to_write) {
  // castling
  if (is_king_castle(move_to_write)) {
    if (buffer_size < 4) return -1;
    buffer[0] = 'O';
    buffer[1] = '-';
    buffer[2] = 'O';
    buffer[3] = '\0';
    return 3;
  }
  if (is_queen_castle(move_to_write)) {
    if (buffer_size < 6) return -1;
    buffer[0] = 'O';
    buffer[1] = '-';
    buffer[2] = 'O';
    buffer[3] = '-';
    buffer[4] = 'O';
    buffer[5] = '\0';
    return 5;
  }
  long out, bytes_written = 0;

  // piece character
  piece_t moved_piece = piece(chess_state, get_from(move_to_write));

  if ((moved_piece & PIECE_MASK) != PAWN) {
    out = write_piece(buffer + bytes_written, buffer_size - bytes_written,
                      moved_piece);
    if (out == -1) {
      buffer[0] = '\0';
      return -1;
    }
    bytes_written += out;
  }
  // disambiguation

  sq0x88_t from = get_from(move_to_write);
  sq0x88_t to = get_to(move_to_write);
  int disambiguate_rank = 0, disambiguate_file = 0;

  switch (moved_piece & PIECE_MASK) {
    case PAWN: {
      if (!is_capture(move_to_write))
        break;  // non capture pawn moves don't need disambiguation
      disambiguate_file = 1;  // always disambiguate pawn captures
    } break;
    case KNIGHT: {
      disambiguate_knight(chess_state, from, to, &disambiguate_rank,
                          &disambiguate_file);
    } break;
    case BISHOP: {
      if (is_light_square(to)) {
        disambiguate_piece(chess_state, from, to,
                           chess_state->friendly_pieces->light_bishop_list,
                           chess_state->friendly_pieces->light_bishop_count,
                           bishop_increment, &disambiguate_rank,
                           &disambiguate_file);
      } else {
        disambiguate_piece(chess_state, from, to,
                           chess_state->friendly_pieces->dark_bishop_list,
                           chess_state->friendly_pieces->dark_bishop_count,
                           bishop_increment, &disambiguate_rank,
                           &disambiguate_file);
      }
    } break;
    case ROOK: {
      disambiguate_piece(
          chess_state, from, to, chess_state->friendly_pieces->rook_list,
          chess_state->friendly_pieces->rook_count, rook_increment,
          &disambiguate_rank, &disambiguate_file);
    } break;
    case QUEEN: {
      disambiguate_piece(
          chess_state, from, to, chess_state->friendly_pieces->queen_list,
          chess_state->friendly_pieces->queen_count, queen_increment,
          &disambiguate_rank, &disambiguate_file);
    } break;
    case KING:
      break;  // no need to disambiguate king moves
    default: {
      buffer[0] = '\0';
      return -1;
    }  // invalid piece (probably already caught in previous step)
  }
  if (disambiguate_file) {
    if (buffer_size - bytes_written < 2)
      WRITE_ERROR("insufficient space in buffer.\n");
    buffer[bytes_written++] = sq0x88_to_file07(from) + 'a';
    buffer[bytes_written] = '\0';
  }
  if (disambiguate_rank) {
    if (buffer_size - bytes_written < 2)
      WRITE_ERROR("insufficient space in buffer.\n");
    buffer[bytes_written++] = sq0x88_to_rank07(from) + '1';
    buffer[bytes_written] = '\0';
  }

  // capture
  if (is_capture(move_to_write)) {
    if (buffer_size - bytes_written < 2)
      WRITE_ERROR("insufficient space in buffer.\n");
    buffer[bytes_written++] = 'x';
    buffer[bytes_written] = '\0';
  }

  // to square
  out = write_square(buffer + bytes_written, buffer_size - bytes_written,
                     get_to(move_to_write));
  if (out == -1) WRITE_ERROR("insufficient space in buffer.\n");
  bytes_written += out;

  // promotion
  if (is_promotion(move_to_write)) {
    if (buffer_size - bytes_written < 2)
      WRITE_ERROR("insufficient space in buffer.\n");
    buffer[bytes_written++] = '=';
    buffer[bytes_written] = '\0';
    out = write_piece(buffer + bytes_written, buffer_size - bytes_written,
                      get_promotes_to(move_to_write));
    if (out == -1) {
      buffer[0] = '\0';
      return -1;
    }
    bytes_written += out;
  }

  // check and checkmate
  int check = 0, checkmate = 0;
  if (!is_legal(chess_state, move_to_write))
    WRITE_ERROR("move to write is not legal.\n");
  make_move(chess_state, move_to_write);
  check = is_check(chess_state);
  checkmate = is_checkmate(chess_state);
  unmake_move(chess_state);

  if (checkmate) {
    if (buffer_size - bytes_written < 2)
      WRITE_ERROR("insufficient space in buffer.\n");
    buffer[bytes_written++] = '#';
  } else if (check) {
    if (buffer_size - bytes_written < 2)
      WRITE_ERROR("insufficient space in buffer.\n");
    buffer[bytes_written++] = '+';
  }

  // termination
  buffer[bytes_written] = '\0';
  return bytes_written;
}

int matches_disambiguation(sq0x88_t square, int rank, int file) {
  return (rank == -1 || sq0x88_to_rank07(square) == rank) &&
         (file == -1 || sq0x88_to_file07(square) == file);
}

long read_algebraic_notation(const char* buffer, long buffer_size,
                             const chess_state_t* chess_state,
                             move_t* read_move) {
  if (buffer_size == 0) {
    READ_ERROR("cannot parse empty buffer.\n");
  }

  long bytes_read = 0;

  *read_move = null_move;

  if (buffer_size >= 5 && strncmp(buffer, "O-O-O", 5) == 0) {
    if (!can_castle_queen_side(chess_state, chess_state->friendly_colour) ||
        !is_legal(chess_state,
                  move(chess_state->friendly_pieces->king_square,
                       chess_state->friendly_pieces->king_square - 2,
                       QUEEN_CASTLE))) {
      READ_ERROR("cannot legally castle queen side.\n");
    }
    *read_move =
        move(chess_state->friendly_pieces->king_square,
             chess_state->friendly_pieces->king_square - 2, QUEEN_CASTLE);
    return 5;
  }
  if (buffer_size >= 3 && strncmp(buffer, "O-O", 3) == 0) {
    if (!can_castle_king_side(chess_state, chess_state->friendly_colour) ||
        !is_legal(
            chess_state,
            move(chess_state->friendly_pieces->king_square,
                 chess_state->friendly_pieces->king_square + 2, KING_CASTLE))) {
      READ_ERROR("cannot legally castle king side.\n");
    }
    *read_move =
        move(chess_state->friendly_pieces->king_square,
             chess_state->friendly_pieces->king_square + 2, KING_CASTLE);
    return 3;
  }

  sq0x88_t from = -1, to = -1;
  int flags = 0;

  piece_t moved_piece = PAWN;
  char piece_char = buffer[bytes_read];
  if (isupper(piece_char)) {
    switch (piece_char) {
      case 'K':
        moved_piece = KING;
        break;
      case 'Q':
        moved_piece = QUEEN;
        break;
      case 'R':
        moved_piece = ROOK;
        break;
      case 'B':
        moved_piece = BISHOP;
        break;
      case 'N':
        moved_piece = KNIGHT;
        break;
      default:
        READ_ERROR("the letter \'%c\' is not a valid piece.\n", piece_char);
    }
    bytes_read++;
  }

  int disambiguation_rank = -1, disambiguation_file = -1;
  int to_square_rank = -1, to_square_file = -1;
  if (bytes_read < buffer_size && is_file(buffer[bytes_read])) {
    disambiguation_file = buffer[bytes_read] - 'a';
    bytes_read++;
  }
  if (bytes_read < buffer_size && is_rank(buffer[bytes_read])) {
    disambiguation_rank = buffer[bytes_read] - '1';
    bytes_read++;
  }
  if (bytes_read < buffer_size && buffer[bytes_read] == 'x') {
    flags |= CAPTURE;
    bytes_read++;
  }
  if (bytes_read < buffer_size && is_file(buffer[bytes_read])) {
    to_square_file = buffer[bytes_read] - 'a';
    bytes_read++;
  }
  if (bytes_read < buffer_size && is_rank(buffer[bytes_read])) {
    to_square_rank = buffer[bytes_read] - '1';
    bytes_read++;
  }

  int from_rank = -1;
  int from_file = -1;

  if (disambiguation_file != -1 && disambiguation_rank != -1 &&
      to_square_file == -1 && to_square_rank == -1) {
    to = rankfile_to_sq0x88(disambiguation_rank, disambiguation_file);
  } else if (to_square_file != -1 && to_square_rank != -1) {
    to = rankfile_to_sq0x88(to_square_rank, to_square_file);
    from_rank = disambiguation_rank;
    from_file = disambiguation_file;
  } else {
    READ_ERROR("move missing valid destination square.\n");
  }

  if (piece_is_friendly(chess_state, to)) {
    READ_ERROR("move is illegal (attempting to capture own piece).\n");
  }

  int found_valid_pieces = 0;

  switch (moved_piece & PIECE_MASK) {
    case PAWN: {
      for (int i = 0; i < chess_state->friendly_pieces->pawn_count; i++) {
        sq0x88_t pawn_square = chess_state->friendly_pieces->pawn_list[i];
        // doesn't match disambiguations from move
        if (!matches_disambiguation(pawn_square, from_rank, from_file)) {
          continue;
        }
        if ((sq0x88_t)(pawn_square + chess_state->up_increment) == to &&
            piece(chess_state, to) == EMPTY &&
            is_legal(chess_state, move(pawn_square, to, QUIET_MOVE))) {
          found_valid_pieces++;
          from = pawn_square;
        }
        if ((sq0x88_t)(pawn_square + chess_state->up_increment * 2) == to &&
            piece(chess_state, to) == EMPTY &&
            piece(chess_state, pawn_square + chess_state->up_increment) ==
                EMPTY &&
            is_legal(chess_state, move(pawn_square, to, DOUBLE_PAWN_PUSH))) {
          found_valid_pieces++;
          from = pawn_square;
          flags = DOUBLE_PAWN_PUSH;
        }
        int test_move_flag =
            (to == enpassent_target(chess_state)) ? ENPASSENT : CAPTURE;
        if ((sq0x88_t)(pawn_square + 1 + chess_state->up_increment) == to &&
            (piece_is_enemy(chess_state, to) || test_move_flag == ENPASSENT) &&
            is_legal(chess_state, move(pawn_square, to, test_move_flag))) {
          found_valid_pieces++;
          from = pawn_square;
          flags = test_move_flag;
        }
        if ((sq0x88_t)(pawn_square - 1 + chess_state->up_increment) == to &&
            (piece_is_enemy(chess_state, to) || test_move_flag == ENPASSENT) &&
            is_legal(chess_state, move(pawn_square, to, test_move_flag))) {
          found_valid_pieces++;
          from = pawn_square;
          flags = test_move_flag;
        }
      }
    } break;
    case KNIGHT: {
      for (int i = 0; i < chess_state->friendly_pieces->knight_count; i++) {
        sq0x88_t knight_square = chess_state->friendly_pieces->knight_list[i];
        // doesn't match disambiguations from move
        if (!matches_disambiguation(knight_square, from_rank, from_file)) {
          continue;
        }
        // knight can legally reach to square
        if (knight_increment(knight_square, to) != 0 &&
            is_legal(chess_state, move(knight_square, to, QUIET_MOVE))) {
          found_valid_pieces++;
          from = knight_square;
        }
      }
    } break;
    case BISHOP: {
      sq0x88_t* piece_list;
      int piece_count;
      if (is_light_square(to)) {
        piece_list = chess_state->friendly_pieces->light_bishop_list;
        piece_count = chess_state->friendly_pieces->light_bishop_count;
      } else {
        piece_list = chess_state->friendly_pieces->dark_bishop_list;
        piece_count = chess_state->friendly_pieces->dark_bishop_count;
      }

      for (int i = 0; i < piece_count; i++) {
        sq0x88_t bishop_square = piece_list[i];
        // doesn't match disambiguations from move
        if (!matches_disambiguation(bishop_square, from_rank, from_file)) {
          continue;
        }
        // bishop can legally reach to square
        int inc = bishop_increment(bishop_square, to);
        if (inc && backwards_ray_cast(chess_state, to, inc) == bishop_square &&
            is_legal(chess_state, move(bishop_square, to, QUIET_MOVE))) {
          found_valid_pieces++;
          from = bishop_square;
        }
      }

    } break;
    case ROOK: {
      for (int i = 0; i < chess_state->friendly_pieces->rook_count; i++) {
        sq0x88_t rook_square = chess_state->friendly_pieces->rook_list[i];
        // doesn't match disambiguations from move
        if (!matches_disambiguation(rook_square, from_rank, from_file)) {
          continue;
        }
        // rook can legally reach to square
        int inc = rook_increment(rook_square, to);
        if (inc && backwards_ray_cast(chess_state, to, inc) == rook_square &&
            is_legal(chess_state, move(rook_square, to, QUIET_MOVE))) {
          found_valid_pieces++;
          from = rook_square;
        }
      }
    } break;
    case QUEEN: {
      for (int i = 0; i < chess_state->friendly_pieces->queen_count; i++) {
        sq0x88_t queen_square = chess_state->friendly_pieces->queen_list[i];
        // doesn't match disambiguations from move
        if (!matches_disambiguation(queen_square, from_rank, from_file)) {
          continue;
        }
        // queen can legally reach to square
        int inc = queen_increment(queen_square, to);
        if (inc && backwards_ray_cast(chess_state, to, inc) == queen_square &&
            is_legal(chess_state, move(queen_square, to, QUIET_MOVE))) {
          found_valid_pieces++;
          from = queen_square;
        }
      }
    } break;
    case KING: {
      from = chess_state->friendly_pieces->king_square;
      found_valid_pieces = 1;
      if (king_increment(from, to) == 0 ||
          !is_legal(chess_state, move(from, to, QUIET_MOVE)))
        return -1;
    } break;
    default:
      return -1;
  }

  if (found_valid_pieces == 0) {
    READ_ERROR("no piece which can legally reach this square.\n");
  }
  if (found_valid_pieces > 1) {
    READ_ERROR("multiple pieces can legally reach this square.\n");
  }

  // promotion
  if (bytes_read < buffer_size && buffer[bytes_read] == '=') {
    if (moved_piece != PAWN && sq0x88_to_rank07(to) != 0 &&
        sq0x88_to_rank07(to) != 7) {
      READ_ERROR("illegal promotion.\n");
    }
    flags |= PROMOTION;
    bytes_read++;
    if (bytes_read >= buffer_size) return -1;
    switch (buffer[bytes_read]) {
      case 'Q':
        flags |= QUEEN_PROMOTE_TO;
        break;
      case 'R':
        flags |= ROOK_PROMOTE_TO;
        break;
      case 'B':
        flags |= BISHOP_PROMOTE_TO;
        break;
      case 'N':
        flags |= KNIGHT_PROMOTE_TO;
        break;
      default:
        READ_ERROR("the letter \'%c\' is not a valid promotion.\n",
                   buffer[bytes_read]);
    }
    bytes_read++;
  }
  // skip check and checkmate
  if (bytes_read < buffer_size && buffer[bytes_read] == '+') {
    bytes_read++;
  } else if (bytes_read < buffer_size && buffer[bytes_read] == '#') {
    bytes_read++;
  }

  // unexpected characters at end of move
  if (bytes_read < buffer_size && buffer[bytes_read] != ' ' &&
      buffer[bytes_read] != '\t' && buffer[bytes_read] != '\n' &&
      buffer[bytes_read] != '\r' && buffer[bytes_read] != '\0') {
    READ_ERROR("unexpected letters after end of move.\n");
  }

  // set move
  move_t move_to_test = move(from, to, flags);
  if (!is_legal(chess_state, move_to_test))
    READ_ERROR("read move is not legal.\n");
  *read_move = move_to_test;
  return bytes_read;
}

long write_long_algebraic_notation(char* buffer, long buffer_size,
                                   move_t move) {
  long bytes_read = 0, out;

  if (buffer_size < 5) WRITE_ERROR("insufficient space in buffer.\n");

  if (is_null_move(move)) {
    buffer[0] = '0';
    buffer[1] = '0';
    buffer[2] = '0';
    buffer[3] = '0';
    buffer[4] = '\0';
    return 4;
  }

  // write from square
  out = write_square(buffer + bytes_read, buffer_size - bytes_read, move.from);
  if (out == -1) {
    buffer[0] = '\0';
    return -1;
  }
  bytes_read += out;

  // write to square
  out = write_square(buffer + bytes_read, buffer_size - bytes_read, move.to);
  if (out == -1) {
    buffer[0] = '\0';
    return -1;
  }
  bytes_read += out;

  // write promotion
  if (is_promotion(move)) {
    out = write_piece(buffer + bytes_read, buffer_size - bytes_read,
                      get_promotes_to(move));
    if (out == -1) {
      buffer[0] = '\0';
      return -1;
    }
    bytes_read += out;
  }
  return bytes_read;
}

// @todo add validation
long read_long_algebraic_notation(const char* buffer, long buffer_size,
                                  const chess_state_t* chess_state,
                                  move_t* read_move) {
  long bytes_read = 0, out;
  sq0x88_t from, to;
  piece_t promotion;
  int move_flags = QUIET_MOVE;

  *read_move = null_move;  // default to null_move

  if (buffer_size >= 4 && strncmp(buffer, "0000", 4) == 0) {
    return 4;
  }

  // read from square
  out = read_square(buffer + bytes_read, buffer_size - bytes_read, &from);
  if (out == -1) return -1;
  bytes_read += out;

  // read to square
  out = read_square(buffer + bytes_read, buffer_size - bytes_read, &to);
  if (out == -1) return -1;
  bytes_read += out;
  if (piece(chess_state, to) != EMPTY) {
    move_flags |= CAPTURE;
  }
  // calculate move flags
  if (piece(chess_state, from) & PAWN) {
    if (to == enpassent_target(chess_state)) {
      move_flags = ENPASSENT;
    }
    if ((sq0x88_t)(to - from) ==
        (sq0x88_t)(pawn_push_increment(chess_state->friendly_colour) * 2)) {
      move_flags = DOUBLE_PAWN_PUSH;
    }
    if (sq0x88_to_rank07(to) == 7 || sq0x88_to_rank07(to) == 0) {
      move_flags |= PROMOTION;
      // read promotion
      out =
          read_piece(buffer + bytes_read, buffer_size - bytes_read, &promotion);
      if (out == -1) return -1;
      bytes_read += out;

      switch (promotion) {
        case QUEEN:
          move_flags |= QUEEN_PROMOTE_TO;
          break;
        case ROOK:
          move_flags |= ROOK_PROMOTE_TO;
          break;
        case BISHOP:
          move_flags |= BISHOP_PROMOTE_TO;
          break;
        case KNIGHT:
          move_flags |= KNIGHT_PROMOTE_TO;
          break;
        default:
          READ_ERROR("invalid promotion target\n")
      }
    }
  } else if (piece(chess_state, from) & KING) {
    if ((sq0x88_t)(from - to) == (sq0x88_t)(-2)) {
      move_flags = KING_CASTLE;
    }
    if ((sq0x88_t)(from - to) == (sq0x88_t)(2)) {
      move_flags = QUEEN_CASTLE;
    }
  }
  *read_move = move(from, to, move_flags);
  return bytes_read;
}

long write_movetext_debug(char* buffer, long buffer_size,
                          const chess_state_t* chess_state) {
  long bytes_written = 0, out;
  for (int i = 0; i < chess_state->ply_counter; i++) {
    move_t move = chess_state->ply_stack[i].move;
    out = write_long_algebraic_notation(buffer + bytes_written,
                                        buffer_size - bytes_written, move);
    if (out == -1) break;
    bytes_written += out;
    if (bytes_written < buffer_size) {
      buffer[bytes_written++] = ' ';
    }
  }
  return bytes_written;
}

long write_movetext(char* buffer, long buffer_size,
                    const chess_state_t* chess_state,
                    const char* start_position) {
  long bytes_written = 0, out;
  chess_state_t state = {0};
  if (start_position == NULL) {
    load_start_position(&state);
  } else {
    load_position(&state, start_position);
  }
  for (int i = 0; i < chess_state->ply_counter; i++) {
    move_t move = chess_state->ply_stack[i].move;
    out = write_algebraic_notation(buffer + bytes_written,
                                   buffer_size - bytes_written, &state, move);
    make_move(&state, move);
    if (out == -1) break;
    bytes_written += out;

    if (bytes_written < buffer_size) {
      buffer[bytes_written++] = ' ';
    }
  }
  free(state.ply_stack);
  return bytes_written;
}

long write_pgn(char* buffer, long buffer_size,
               const chess_state_t* chess_state, const char* event,
               const char* site, const char* date, const char* round,
               const char* white_name, const char* black_name,
               const char* fen) {
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

  if (event == NULL) event = "?";
  if (site == NULL) site = "?";
  if (date == NULL) date = "????.??.??";
  if (round == NULL) round = "?";

  long bytes_written = 0;
  bytes_written +=
      snprintf(buffer + bytes_written, buffer_size - bytes_written,
               "[Event \"%s\"]\n"
               "[Site \"%s\"]\n"
               "[Date \"%s\"]\n"
               "[Round \"%s\"]\n"
               "[White \"%s\"]\n"
               "[Black \"%s\"]\n"
               "[Result \"%s\"]\n",
               event, site, date, round, white_name, black_name, result);
  if (fen != NULL) {
    bytes_written +=
        snprintf(buffer + bytes_written, buffer_size - bytes_written,
                 "[FEN \"%s\"]\n", fen);
  }
  if (bytes_written != buffer_size)
    buffer[bytes_written++] = '\n';  // extra newline before movetext
  bytes_written += write_movetext(
      buffer + bytes_written, buffer_size - bytes_written, chess_state, fen);
  return bytes_written;
}