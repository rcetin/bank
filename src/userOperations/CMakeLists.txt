target_include_directories(bank PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Customer)
add_subdirectory(Account)
add_subdirectory(Storage)
add_subdirectory(Credentials)
add_subdirectory(Transaction)

target_sources(bank PRIVATE 
    UserOps.cpp
)
