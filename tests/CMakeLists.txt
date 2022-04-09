cmake_minimum_required(VERSION 3.14)

target_compile_definitions(testing PRIVATE TESTING)

target_include_directories(testing PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(testing PRIVATE ${CMAKE_SOURCE_DIR}/src/thirdparty/SQLiteCpp/include)

add_subdirectory(unit_tests)

target_link_libraries(
  testing
  SQLiteCpp
  sqlite3
  gtest
  gtest_main
)
