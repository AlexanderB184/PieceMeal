#ifndef CHESS_LIB_H
#define CHESS_LIB_H

#ifdef __cplusplus
extern "C" {
//namespace chess {
#endif

#include "checks.h"
#include "chess-lib-constants.h"
#include "chess-lib-inlines.h"
#include "chess-lib-types.h"
#include "chess-state.h"
#include "game-over.h"
#include "move-generation.h"
#include "move-is-legal.h"
#include "move.h"
#include "zobrist.h"
#include "notation.h"

#ifdef __cplusplus
//} // namespace chess
}
#endif

#endif