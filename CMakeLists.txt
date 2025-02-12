cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(PieceMeal VERSION 1.1.0)

add_library(chess SHARED PieceMeal.c include/chess.h)

add_compile_options(-Wall -Wextra -Wpedantic -Werror)
add_compile_options(-O3)

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
add_executable(test_gameover tests/gameover.cpp)
add_executable(alphaBeta tests/alpha_beta.cpp)
add_executable(profTest tests/profile_test.cpp)

# Link the test executables with Catch2
target_link_libraries(test_perft PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_is_check PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_is_pseudo_legal PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_is_legal PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_inc_movegen PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_is_check_after_move PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_fuzz_notation PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(test_gameover PRIVATE Catch2::Catch2WithMain chess)
target_link_libraries(alphaBeta PRIVATE chess)
target_link_libraries(profTest PRIVATE chess)

include(CTest)
include(Catch)
catch_discover_tests(test_perft)
catch_discover_tests(test_is_check)
catch_discover_tests(test_is_pseudo_legal)
catch_discover_tests(test_is_legal)
catch_discover_tests(test_inc_movegen)
catch_discover_tests(test_is_check_after_move)
catch_discover_tests(test_fuzz_notation)
catch_discover_tests(test_gameover)