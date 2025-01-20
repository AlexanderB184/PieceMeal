#include <stdlib.h>

#include "../include/chess.h"
#include "../include/increments.h"

sq0x88_t queen_increment(sq0x88_t from, sq0x88_t to) {
  return queen_increment_vector[diff_0x88(to, from)];
}
sq0x88_t rook_increment(sq0x88_t from, sq0x88_t to) {
  return rook_increment_vector[diff_0x88(to, from)];
}
sq0x88_t bishop_increment(sq0x88_t from, sq0x88_t to) {
  return bishop_increment_vector[diff_0x88(to, from)];
}
sq0x88_t knight_increment(sq0x88_t from, sq0x88_t to) {
  return knight_increment_vector[diff_0x88(to, from)];
}
sq0x88_t king_increment(sq0x88_t from, sq0x88_t to) {
  return king_increment_vector[diff_0x88(to, from)];
}

sq0x88_t backwards_ray_cast(const chess_state_t* chess_state, sq0x88_t from,
                            sq0x88_t inc) {
  sq0x88_t next;
  for (next = from - inc; piece(chess_state, next) == EMPTY; next -= inc);
  return next;
}

sq0x88_t forwards_ray_cast(const chess_state_t* chess_state, sq0x88_t from,
                           sq0x88_t inc) {
  sq0x88_t next;
  for (next = from + inc; piece(chess_state, next) == EMPTY; next += inc);
  return next;
}