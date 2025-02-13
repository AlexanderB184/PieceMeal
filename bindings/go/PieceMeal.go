package PieceMeal

// #cgo CFLAGS: -Wall -Wextra -Werror -O3 -DDEBUG -I../../include
// #include "../../include/chess.h"
// #include "../../PieceMeal.c"
import "C"

// The key functionality from PieceMeal ported into golang!

import (
	"fmt"
	"io"
	"runtime"
	"unsafe"
)

type ChessState C.chess_state_t
type Move C.move_t
type Piece C.piece_t
type PieceKind C.piece_t
type Colour C.colour_t
type Square C.sq0x88_t

type GameState int8

const (
	ONGOING                       = GameState(0)
	CHECKMATE                     = GameState(1)
	STALEMATE                     = GameState(-1)
	DRAW_BY_REPETITION            = GameState(-2)
	DRAW_BY_50_MOVE_RULE          = GameState(-3)
	DRAW_BY_INSUFFICIENT_MATERIAL = GameState(-4)
)

func (piece *Piece) Kind() PieceKind {
	return PieceKind(*piece) & (PAWN | KNIGHT | BISHOP | ROOK | QUEEN | KING)
}

func (piece *Piece) Colour() Colour {
	return Colour(*piece) & (WHITE | BLACK)
}

const (
	EMPTY  = PieceKind(0x00)
	PAWN   = PieceKind(0x01)
	KNIGHT = PieceKind(0x02)
	BISHOP = PieceKind(0x04)
	ROOK   = PieceKind(0x08)
	KING   = PieceKind(0x10)
	WHITE  = Colour(0x20)
	BLACK  = Colour(0x40)
	BORDER = Piece(0x80)

	QUEEN = BISHOP | ROOK

	WHITE_PAWN   = Piece(WHITE) | Piece(PAWN)
	WHITE_KNIGHT = Piece(WHITE) | Piece(KNIGHT)
	WHITE_BISHOP = Piece(WHITE) | Piece(BISHOP)
	WHITE_ROOK   = Piece(WHITE) | Piece(ROOK)
	WHITE_QUEEN  = Piece(WHITE) | Piece(QUEEN)
	WHITE_KING   = Piece(WHITE) | Piece(KING)

	BLACK_PAWN   = Piece(BLACK) | Piece(PAWN)
	BLACK_KNIGHT = Piece(BLACK) | Piece(KNIGHT)
	BLACK_BISHOP = Piece(BLACK) | Piece(BISHOP)
	BLACK_ROOK   = Piece(BLACK) | Piece(ROOK)
	BLACK_QUEEN  = Piece(BLACK) | Piece(QUEEN)
	BLACK_KING   = Piece(BLACK) | Piece(KING)
)

const (
	WHITE_QUEEN_SIDE = 0x8
	WHITE_KING_SIDE  = 0x4
	BLACK_QUEEN_SIDE = 0x2
	BLACK_KING_SIDE  = 0x1
	WHITE_BOTH_SIDES = WHITE_QUEEN_SIDE | WHITE_KING_SIDE
	BLACK_BOTH_SIDES = BLACK_QUEEN_SIDE | BLACK_KING_SIDE
	ALL_RIGHTS       = WHITE_BOTH_SIDES | BLACK_BOTH_SIDES
	NO_RIGHTS        = 0
)

// move flags
const (
	PROMOTION  = 0x8
	CAPTURE    = 0x4
	SPECIAL1   = 0x2
	SPECIAL0   = 0x1
	QUIET_MOVE = 0x0

	DOUBLE_PAWN_PUSH         = SPECIAL0
	KING_CASTLE              = SPECIAL1
	QUEEN_CASTLE             = SPECIAL0 | SPECIAL1
	ENPASSENT                = CAPTURE | SPECIAL0
	KNIGHT_PROMOTE_TO        = 0
	BISHOP_PROMOTE_TO        = SPECIAL0
	ROOK_PROMOTE_TO          = SPECIAL1
	QUEEN_PROMOTE_TO         = SPECIAL0 | SPECIAL1
	KNIGHT_PROMOTION         = PROMOTION | KNIGHT_PROMOTE_TO
	BISHOP_PROMOTION         = PROMOTION | BISHOP_PROMOTE_TO
	ROOK_PROMOTION           = PROMOTION | ROOK_PROMOTE_TO
	QUEEN_PROMOTION          = PROMOTION | QUEEN_PROMOTE_TO
	KNIGHT_CAPTURE_PROMOTION = PROMOTION | CAPTURE | KNIGHT_PROMOTE_TO
	BISHOP_CAPTURE_PROMOTION = PROMOTION | CAPTURE | BISHOP_PROMOTE_TO
	ROOK_CAPTURE_PROMOTION   = PROMOTION | CAPTURE | ROOK_PROMOTE_TO
	QUEEN_CAPTURE_PROMOTION  = PROMOTION | CAPTURE | QUEEN_PROMOTE_TO
)

func (chessState *ChessState) MakeMove(move Move) {
	C.make_move((*C.chess_state_t)(chessState), C.move_t(move))
}

func (chessState *ChessState) UnmakeMove() {
	C.unmake_move((*C.chess_state_t)(chessState))
}

func (chessState *ChessState) IsLegalMove(move Move) bool {
	if C.is_pseudo_legal((*C.chess_state_t)(chessState), C.move_t(move)) == 0 {
		return false
	}
	return C.is_legal((*C.chess_state_t)(chessState), C.move_t(move)) != 0
}

func (square *Square) String() string {
	buffer := [3]C.char{0, 0, 0}
	out := C.write_square((*C.char)(unsafe.Pointer(&buffer[0])), 3, C.sq0x88_t(*square))
	if out == -1 {
		return ""
	}
	return C.GoString(&buffer[0])
}

func (piece *Piece) String() string {
	buffer := [8]C.char{}
	out := C.write_piece((*C.char)(unsafe.Pointer(&buffer[0])), 8, C.piece_t(*piece))
	if out == -1 {
		return ""
	}
	return C.GoString(&buffer[0])
}

func (move *Move) String() string {
	buffer := [8]C.char{}
	out := C.write_long_algebraic_notation((*C.char)(unsafe.Pointer(&buffer[0])), 8, C.move_t(*move))
	if out == -1 {
		return ""
	}
	return C.GoString(&buffer[0])
}

func (cs *ChessState) String() string {
	buffer := [1024]C.char{}
	out := C.save_position((*C.chess_state_t)(cs), (*C.char)(unsafe.Pointer(&buffer[0])), 1024)
	if out == -1 {
		return ""
	}
	return C.GoString(&buffer[0])
}

func NewGame() *ChessState {
	cs := &ChessState{}
	C.load_start_position((*C.chess_state_t)(cs))
	return cs
}

func LoadGame(fen string) (*ChessState, error) {
	cs := &ChessState{}

	cfen := C.CString(fen)
	defer C.free(unsafe.Pointer(cfen))

	out := C.load_position((*C.chess_state_t)(cs), cfen)
	if out == -1 {
		return nil, fmt.Errorf("invalid fen")
	}
	return cs, nil
}

func (chessState *ChessState) Clone() *ChessState {
	clonedState := &ChessState{}
	C.copy_position((*C.chess_state_t)(clonedState),
		(*C.chess_state_t)(chessState))
	return clonedState
}

func (cs *ChessState) free() {
	if cs != nil {
		C.release_position((*C.chess_state_t)(cs))
	}
}

func (cs *ChessState) Close() {
	runtime.SetFinalizer(cs, nil)
	cs.free()
}

func (cs *ChessState) LegalMoves() []Move {
	moves := [256]Move{}
	count := C.generate_legal_moves((*C.chess_state_t)(cs), (*C.move_t)(unsafe.Pointer(&moves[0])),
		cs.friendly_colour)
	return moves[:count]
}

func (cs *ChessState) LegalCaptures() []Move {
	moves := [256]Move{}
	count := C.generate_legal_captures((*C.chess_state_t)(cs), (*C.move_t)(unsafe.Pointer(&moves[0])),
		cs.friendly_colour)
	return moves[:count]
}

func (cs *ChessState) LegalQuiets() []Move {
	moves := [256]Move{}
	count := C.generate_legal_quiets((*C.chess_state_t)(cs), (*C.move_t)(unsafe.Pointer(&moves[0])),
		cs.friendly_colour)
	return moves[:count]
}

func (cs *ChessState) LegalPromotions() []Move {
	moves := [256]Move{}
	count := C.generate_legal_promotions((*C.chess_state_t)(cs), (*C.move_t)(unsafe.Pointer(&moves[0])),
		cs.friendly_colour)
	return moves[:count]
}

func (cs *ChessState) IsDrawByRepetition() bool {
	return C.is_draw_by_repetition((*C.chess_state_t)(cs)) != 0
}

func (cs *ChessState) IsDrawByInsufficientMaterial() bool {
	return C.is_draw_by_insufficient_material((*C.chess_state_t)(cs)) != 0
}

func (cs *ChessState) IsDrawBy50MoveRule() bool {
	return C.is_draw_by_50_move_rule((*C.chess_state_t)(cs)) != 0
}

func (cs *ChessState) IsDraw() bool {
	return C.is_draw((*C.chess_state_t)(cs)) != 0
}

func (cs *ChessState) IsCheckmate() bool {
	return C.is_checkmate((*C.chess_state_t)(cs)) != 0
}

func (cs *ChessState) IsStalemate() bool {
	return C.is_stalemate((*C.chess_state_t)(cs)) != 0
}

func (cs *ChessState) IsGameover() GameState {
	return GameState(C.is_gameover((*C.chess_state_t)(cs)))
}

func (cs *ChessState) GetWinner() Colour {
	return Colour(C.get_winner((*C.chess_state_t)(cs)))
}

func isWhiteSpace(b byte) bool {
	return b == ' ' || b == '\r' || b == '\n' || b == '\t'
}

func (cs *ChessState) ReadMove(reader io.ByteReader) (Move, error) {
	// longest move is 5 bytes so 8 is sufficient
	buffer := [8]byte{}

	i := 0

	// skip leading whitespace
	for {
		b, err := reader.ReadByte()
		if err != nil {
			return Move{}, err
		}
		if !isWhiteSpace(b) {
			buffer[i] = b
			i++
			break
		}
	}

	// read at most 8 bytes
	for i < len(buffer) {
		b, err := reader.ReadByte()
		if err != nil {
			break // End of input
		}
		if isWhiteSpace(b) { // Stop at whitespace
			break
		}
		buffer[i] = b
		i++
	}

	if i == 0 {
		return Move{}, fmt.Errorf("no move found")
	}

	cbuffer := (*C.char)(unsafe.Pointer(&buffer[0]))

	move := Move{}
	out := C.read_long_algebraic_notation(cbuffer, C.long(i), (*C.chess_state_t)(cs), (*C.move_t)(&move))

	if out == -1 {
		return move, fmt.Errorf("invalid move notation")
	}

	return move, nil
}

func (move *Move) IsCapture() bool {
	return (int(C.get_flags(*(*C.move_t)(move))) & CAPTURE) != 0
}

func (move *Move) IsPromotion() bool {
	return (int(C.get_flags(*(*C.move_t)(move))) & PROMOTION) != 0
}

func (move *Move) IsEnpassent() bool {
	return int(C.get_flags(*(*C.move_t)(move))) == ENPASSENT
}

func (move *Move) IsDoublePawnPush() bool {
	return int(C.get_flags(*(*C.move_t)(move))) == DOUBLE_PAWN_PUSH
}

func (move *Move) IsQueenCastle() bool {
	return int(C.get_flags(*(*C.move_t)(move))) == QUEEN_CASTLE
}

func (move *Move) IsKingCastle() bool {
	return int(C.get_flags(*(*C.move_t)(move))) == KING_CASTLE
}

func (move *Move) GetPromotesTo() PieceKind {
	switch int(C.get_flags(*(*C.move_t)(move))) & 3 {
	case KNIGHT_PROMOTE_TO:
		return KNIGHT
	case BISHOP_PROMOTE_TO:
		return BISHOP
	case ROOK_PROMOTE_TO:
		return ROOK
	case QUEEN_PROMOTE_TO:
		return QUEEN
	default:
		return EMPTY
	}
}

func (cs *ChessState) IsCheck() bool {
	return C.is_check((*C.chess_state_t)(cs)) != 0
}

func (cs *ChessState) IsDoubleCheck() bool {
	return C.is_double_check((*C.chess_state_t)(cs)) != 0
}

func (cs *ChessState) Turn() int {
	return int(cs.ply_counter)/2 + 1
}

func (cs *ChessState) BlackToMove() bool {
	return cs.black_to_move != 0
}

func (cs *ChessState) WhiteToMove() bool {
	return cs.black_to_move == 0
}

func (cs *ChessState) HalfMoveClock() int {
	return int(cs.half_move_clock)
}

func (cs *ChessState) EpTarget() Square {
	return Square(cs.enpassent_target)
}

func (cs *ChessState) WhiteCanCastleKingSide() bool {
	return (int(cs.castle_rights) & WHITE_KING_SIDE) != 0
}

func (cs *ChessState) BlackCanCastleKingSide() bool {
	return (int(cs.castle_rights) & BLACK_KING_SIDE) != 0
}

func (cs *ChessState) WhiteCanCastleQueenSide() bool {
	return (int(cs.castle_rights) & WHITE_QUEEN_SIDE) != 0
}

func (cs *ChessState) BlackCanCastleQueenSide() bool {
	return (int(cs.castle_rights) & BLACK_QUEEN_SIDE) != 0
}

func (cs *ChessState) Hash() uint64 {
	return uint64(cs.zobrist)
}

func (cs *ChessState) PrevMove() Move {
	if cs.ply_counter == 0 {
		return Move{}
	}
	plyStack := *(*[]C.ply_stack_item_t)(unsafe.Pointer(&cs.ply_stack))
	return Move(plyStack[cs.ply_counter-1].move)
}
