import random, sys

UINT64_MAX = 0xffff_ffff_ffff_ffff

PADDING = len(hex(UINT64_MAX))

def randomHex64() -> str:
    value = random.randint(0, UINT64_MAX)
    return f"{value:#0{PADDING}x}"

ROWS = 12
COLS = 64

HEADER_GUARD_TOP = "#ifndef CHESS_ZOBRIST_TABLES_H\n#define CHESS_ZOBRIST_TABLES_H\n"
HEADER_GUARD_BOTTOM = "#endif // CHESS_ZOBRIST_TABLES_H"

INCLUDES = "#include \"chess.h\"\n"

ZOBRIST_TABLE_DECL = f"static const zobrist_t zobrist_table[{ROWS}][{COLS}] = {'{'}"
ZOBRIST_BLACK_TO_MOVE_DECL = "static const zobrist_t zobrist_black_to_move = "

PAWN = 0x1
KNIGHT = 0x2
BISHOP = 0x4
ROOK = 0x8
QUEEN = BISHOP + ROOK
KING = 0x10

INDEX_MAP_SIZE = KING + 1

ZOBRIST_PIECE_INDEX_MAP_DECL = f"static const int8_t piece_to_zobrist_index_map[{INDEX_MAP_SIZE}] = {{"

ZOBRIST_PIECE_INDEX_MAP = [-1] * INDEX_MAP_SIZE
ZOBRIST_PIECE_INDEX_MAP[PAWN] = 0
ZOBRIST_PIECE_INDEX_MAP[KNIGHT] = 1
ZOBRIST_PIECE_INDEX_MAP[BISHOP] = 2
ZOBRIST_PIECE_INDEX_MAP[ROOK] = 3
ZOBRIST_PIECE_INDEX_MAP[QUEEN] = 4
ZOBRIST_PIECE_INDEX_MAP[KING] = 5

if __name__ == "__main__":    
    print(HEADER_GUARD_TOP)
    print(INCLUDES)
    #print(ZOBRIST_PIECE_INDEX_MAP_DECL)
    #for index in ZOBRIST_PIECE_INDEX_MAP:
    #    print(f"\t{index},")
    #print(f"}};\n")
    
    print(ZOBRIST_BLACK_TO_MOVE_DECL + randomHex64() + ';\n')
    print(ZOBRIST_TABLE_DECL)
    for row in range(ROWS):
        line = ''
        for col in range(COLS):
           line += randomHex64()+', '
        print(f"\t{'{'}{line}{'}'},")
    print("};\n")
    print(HEADER_GUARD_BOTTOM)