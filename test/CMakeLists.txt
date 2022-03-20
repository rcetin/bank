cmake_minimum_required(VERSION 3.14)
project(my_project)

set(CMAKE_CXX_STANDARD 17)

add_executable(
  hello_test
  hello_test.cc
)

target_link_libraries(
  hello_test
  gtest
  gtest_main
)
