target_include_directories(bank PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(bank PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/userInput)
# target_include_directories(bank PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/utils)

add_subdirectory(thirdparty/SQLiteCpp)
add_subdirectory(userOperations)
add_subdirectory(userInput)

target_sources(bank PRIVATE 
    main.cpp
)
