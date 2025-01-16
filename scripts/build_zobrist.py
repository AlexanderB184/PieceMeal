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

if __name__ == "__main__":    
    print(HEADER_GUARD_TOP)
    print(INCLUDES)
    print(ZOBRIST_BLACK_TO_MOVE_DECL + randomHex64() + ';\n')
    print(ZOBRIST_TABLE_DECL)
    for row in range(ROWS):
        line = ''
        for col in range(COLS):
           line += randomHex64()+', '
        print(f"\t{'{'}{line}{'}'},")
    print("};\n")
    print(HEADER_GUARD_BOTTOM)