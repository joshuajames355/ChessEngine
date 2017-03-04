add_executable(UnitTests test.cpp)

target_link_libraries (UnitTests gtest)
target_link_libraries (UnitTests gtest_main)
target_link_libraries (UnitTests CHESSLIBRARY)