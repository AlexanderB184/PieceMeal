cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(PieceMeal VERSION 1.0.0)

add_library(chess SHARED src/game-over.c src/increment-vectors.c src/load_position.c src/make_move.c src/move_generation.c src/move_is_legal.c src/move.c src/notation.c src/piece_list.c src/threats.c src/zobrist.c include/chess-lib.h)

enable_testing()

include(FetchContent)

FetchContent_Declare(
    catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.3.2.zip
)

FetchContent_MakeAvailable(catch2)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O3)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX /O2)
endif()

# Add the test executable
add_executable(test_perft tests/perft.cpp)

# Link the test executable with Catch2
target_link_libraries(test_perft PRIVATE Catch2::Catch2WithMain chess)

# Add the test to CTest
add_test(NAME Perft COMMAND test_perft)