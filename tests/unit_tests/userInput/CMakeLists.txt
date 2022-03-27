target_include_directories(testing PRIVATE
    ${PROJECT_SOURCE_DIR}/src/userInput
)

target_sources(testing PRIVATE
    StringInputTest.cpp
    PasswordInputTest.cpp
    IntegerInputTest.cpp
    UserSchemaTest.cpp
    CharInputTest.cpp
    ${PROJECT_SOURCE_DIR}/src/userInput/UserSchema.cpp
)
