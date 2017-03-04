file(GLOB Sources "*.cpp")

add_library(CHESSLIBRARY ${Sources})

target_include_directories (CHESSLIBRARY PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
