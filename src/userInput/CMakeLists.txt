target_include_directories(bank PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(bank PRIVATE ${CMAKE_SOURCE_DIR})

target_sources(bank PRIVATE 
    UserSchema.cpp
)
