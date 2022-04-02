target_include_directories(testing PRIVATE
    ${PROJECT_SOURCE_DIR}/src/userOperations/Credentials
)

target_sources(testing PRIVATE
    Credentials.cpp
)
