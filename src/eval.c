#include "chess.h"
#include "piece_square_tables.h"

centipawn_t base_material_score(const chess_state_t* position) {
  centipawn_t friendly_material =
      position->friendly_pieces->pawn_count * PAWN_SCORE +
      position->friendly_pieces->knight_count * KNIGHT_SCORE +
      position->friendly_pieces->dark_bishop_count * BISHOP_SCORE +
      position->friendly_pieces->light_bishop_count * BISHOP_SCORE +
      position->friendly_pieces->rook_count * ROOK_SCORE +
      position->friendly_pieces->queen_count * QUEEN_SCORE;
  centipawn_t enemy_material =
      position->enemy_pieces->pawn_count * PAWN_SCORE +
      position->enemy_pieces->knight_count * KNIGHT_SCORE +
      position->enemy_pieces->dark_bishop_count * BISHOP_SCORE +
      position->enemy_pieces->light_bishop_count * BISHOP_SCORE +
      position->enemy_pieces->rook_count * ROOK_SCORE +
      position->enemy_pieces->queen_count * QUEEN_SCORE;
  return friendly_material - enemy_material;
}

centipawn_t material_score(const chess_state_t* position) {
  centipawn_t score = 0;
  FOR_EACH_PIECE(&position->white_pieces, pawn, square) {
    score += pawn_score_table[sq0x88_to_sq8x8(square)];
  }
  FOR_EACH_PIECE(&position->black_pieces, pawn, square) {
    score -= pawn_score_table[sq0x88_to_sq8x8(square) ^ 56];
  }
  FOR_EACH_PIECE(&position->white_pieces, knight, square) {
    score += knight_score_table[sq0x88_to_sq8x8(square)];
  }
  FOR_EACH_PIECE(&position->black_pieces, knight, square) {
    score -= knight_score_table[sq0x88_to_sq8x8(square) ^ 56];
  }
  FOR_EACH_PIECE(&position->white_pieces, light_bishop, square) {
    score += bishop_score_table[sq0x88_to_sq8x8(square)];
  }
  FOR_EACH_PIECE(&position->black_pieces, light_bishop, square) {
    score -= bishop_score_table[sq0x88_to_sq8x8(square) ^ 56];
  }
  FOR_EACH_PIECE(&position->white_pieces, dark_bishop, square) {
    score += bishop_score_table[sq0x88_to_sq8x8(square)];
  }
  FOR_EACH_PIECE(&position->black_pieces, dark_bishop, square) {
    score -= bishop_score_table[sq0x88_to_sq8x8(square) ^ 56];
  }
  FOR_EACH_PIECE(&position->white_pieces, rook, square) {
    score += rook_score_table[sq0x88_to_sq8x8(square)];
  }
  FOR_EACH_PIECE(&position->black_pieces, rook, square) {
    score -= rook_score_table[sq0x88_to_sq8x8(square) ^ 56];
  }
  FOR_EACH_PIECE(&position->white_pieces, queen, square) {
    score += queen_score_table[sq0x88_to_sq8x8(square)];
  }
  FOR_EACH_PIECE(&position->black_pieces, queen, square) {
    score -= queen_score_table[sq0x88_to_sq8x8(square) ^ 56];
  }
  score += king_score_table[sq0x88_to_sq8x8(position->white_pieces.king_square)];
  score -= king_score_table[sq0x88_to_sq8x8(position->black_pieces.king_square) ^ 56];
  score = position->black_to_move ? -score : score;
  return score + base_material_score(position);
}

#define MOBILITY_SCORE_MULTIPLIER 1 / 10

centipawn_t mobility_score(const chess_state_t* position) {
  move_t buffer[256];
  size_t friendly_move_count =
      generate_legal_moves(position, buffer, position->friendly_colour);
  size_t enemy_move_count =
      generate_legal_moves(position, buffer, position->enemy_colour);
  return (enemy_move_count - friendly_move_count) * MOBILITY_SCORE_MULTIPLIER;
}

centipawn_t pseudo_mobility_score(const chess_state_t* position) {
  move_t buffer[256];
  size_t friendly_move_count =
      generate_moves(position, buffer, position->friendly_colour);
  size_t enemy_move_count =
      generate_moves(position, buffer, position->enemy_colour);
  return (enemy_move_count - friendly_move_count) * MOBILITY_SCORE_MULTIPLIER;
}