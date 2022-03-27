cmake_minimum_required(VERSION 3.14)
project(bank VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(bank)

add_subdirectory(src)
add_subdirectory(tests)

target_link_libraries(bank
  SQLiteCpp
  sqlite3
  pthread
  dl
)