cmake_minimum_required(VERSION 3.14)

add_executable(testing)

add_subdirectory(unit_tests)

target_link_libraries(
  testing
  gtest
  gtest_main
)
