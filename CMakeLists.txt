cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (ChessEngine)

file(GLOB SOURCES ChessEngine/*.cpp)

add_executable(ChessEngine ${SOURCES})
