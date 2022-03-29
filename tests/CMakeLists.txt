cmake_minimum_required(VERSION 3.14)

add_executable(testing)

target_include_directories(testing PRIVATE ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(unit_tests)

target_link_libraries(
  testing
  gtest
  gtest_main
)
