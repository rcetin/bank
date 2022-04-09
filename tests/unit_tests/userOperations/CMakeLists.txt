target_include_directories(testing PRIVATE
    ${PROJECT_SOURCE_DIR}/src/userOperations/Credentials
)

target_sources(testing PRIVATE
    CredentialsTest.cpp
    StorageTest.cpp
    ${PROJECT_SOURCE_DIR}/src/userOperations/Credentials/Credentials.cpp
    ${PROJECT_SOURCE_DIR}/src/userOperations/Storage/Storage.cpp
)
