cmake_minimum_required(VERSION 3.10)
project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NCURSESW_LIB /usr/lib/x86_64-linux-gnu/libncursesw.so)
set(NCURSESW_INC /usr/include)

include_directories(${NCURSESW_INC})
include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(game ${SOURCES})
target_link_libraries(game ${NCURSESW_LIB})