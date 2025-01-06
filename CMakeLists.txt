cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(PieceMeal VERSION 1.1.0)

add_library(chess SHARED src/game-over.c src/increment-vectors.c src/load_position.c src/make_move.c src/move_generation.c src/move_is_legal.c src/notation.c src/piece_list.c src/threats.c src/zobrist.c include/chess.h)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
add_compile_options(-O3)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    
    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX /O2)
endif()

enable_testing()

include(FetchContent)

set(CATCH2_DIR "${CMAKE_BINARY_DIR}/_deps/catch2")

FetchContent_Declare(
    catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.3.2.zip
    DOWNLOAD_DIR ${CATCH2_DIR}
)

FetchContent_MakeAvailable(catch2)

add_executable(test_perft tests/perft.cpp)
add_executable(test_is_check tests/is_check.cpp)
add_executable(test_is_pseudo_legal tests/pseudo_legal.cpp)
add_executable(test_is_legal tests/is_legal.cpp)
add_executable(test_inc_movegen tests/incremental_movegen.cpp)
add_executable(test_is_check_after_move tests/check_after_move.cpp)
add_executable(test_fuzz_notation tests/fuzz.cpp)
add_executable(alphaBeta tests/alpha_beta.cpp)

# Link the test executables with Catch2
target_link_libraries(test_perft PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_is_check PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_is_pseudo_legal PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_is_legal PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_inc_movegen PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_is_check_after_move PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_fuzz_notation PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(alphaBeta PRIVATE chess)

include(CTest)
include(Catch)
catch_discover_tests(test_perft)
catch_discover_tests(test_is_check)
catch_discover_tests(test_is_pseudo_legal)
catch_discover_tests(test_is_legal)
catch_discover_tests(test_inc_movegen)
catch_discover_tests(test_is_check_after_move)
catch_discover_tests(test_fuzz_notation)