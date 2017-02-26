cmake_minimum_required (VERSION 2.6)

project (ChessEngine)

file(GLOB SOURCES ChessEngine/*.cpp)

add_executable(ChessEngine ${SOURCES})

target_compile_features(ChessEngine PRIVATE cxx_strong_enums)