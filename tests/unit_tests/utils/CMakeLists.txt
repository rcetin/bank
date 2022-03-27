target_include_directories(testing PRIVATE
    ${PROJECT_SOURCE_DIR}/src/utils
)

target_sources(testing PRIVATE
    Rand.cpp
    Tree.cpp
)
