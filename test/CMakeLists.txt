cmake_minimum_required(VERSION 3.1.3)

set(CMAKE_CXX_STANDARD 17)

project(canlibtests C CXX)


set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage --coverage")
set(GCC_COVERAGE_LINK_FLAGS "-lgcov --coverage")
set(GCC_DEBUG_FLAGS "-g -ggdb -Og")
set(GCC_WARNING_FLAGS "-Wall -Wextra -fdiagnostics-color=always -Werror")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} ${GCC_WARNING_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

include_directories(../src)
include_directories(./tests/inc)
include_directories(../src/drivers/inc)
include_directories(../src/computers/inc)

file(GLOB_RECURSE CANLIB_SOURCES "../src/*.cpp" "../src/*.c")
add_library(canlib ${CANLIB_SOURCES})
target_compile_features(canlib INTERFACE cxx_strong_enums)

file(GLOB_RECURSE SOURCES "./tests/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} stdc++fs canlib)
target_compile_definitions(canlibtests PUBLIC CANLIB_ARCH_TESTFAMILY)

enable_testing()
add_test(Tests ${PROJECT_NAME})
