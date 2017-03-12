file(GLOB Sources "*.cpp")

add_executable(UnitTests ${Sources})

target_link_libraries (UnitTests gtest)
target_link_libraries (UnitTests gtest_main)
target_link_libraries (UnitTests CHESSLIBRARY)