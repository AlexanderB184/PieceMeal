cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)

project(PieceMeal VERSION 1.0.0)

add_library(chess SHARED src/game-over.c src/increment-vectors.c src/load_position.c src/make_move.c src/move_generation.c src/move_is_legal.c src/move.c src/notation.c src/piece_list.c src/threats.c src/zobrist.c include/chess-lib.h)

enable_testing()

include(FetchContent)

set(CATCH2_DIR "${CMAKE_BINARY_DIR}/_deps/catch2")

FetchContent_Declare(
    catch2
    URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.3.2.zip
    DOWNLOAD_DIR ${CATCH2_DIR}
)

FetchContent_MakeAvailable(catch2)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O3)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/W4 /WX /O2)
endif()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds are not allowed. Please use a separate build directory.")
endif()

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug or Release)")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_TESTS "Enable building tests" ON)
option(ENABLE_COVERAGE "Enable code coverage reporting" OFF)

if (ENABLE_COVERAGE)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(--coverage)
        add_link_options(--coverage)
    else()
        message(WARNING "Code coverage is only supported with GCC and Clang")
    endif()
endif()

if (ENABLE_TESTS)
    enable_testing()
    add_test(NAME Perft COMMAND test_perft)
endif()

install(TARGETS chess
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/PieceMealConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

export(TARGETS chess FILE ${CMAKE_CURRENT_BINARY_DIR}/PieceMealTargets.cmake)
