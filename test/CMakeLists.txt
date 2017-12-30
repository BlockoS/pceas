cmake_minimum_required (VERSION 3.2)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/externals/munit
)

add_executable( sha256_test sha256.c ${PROJECT_SOURCE_DIR}/src/sha256.c )
target_link_libraries( sha256_test munit)
add_test( sha256 ${EXECUTABLE_OUTPUT_PATH}/sha256_test )

add_executable( hashtable_test hashtable.c ${PROJECT_SOURCE_DIR}/src/hashtable.c ${PROJECT_SOURCE_DIR}/src/hash.c ${PROJECT_SOURCE_DIR}/src/swap.c )
target_link_libraries( hashtable_test munit)
add_test( hashtable ${EXECUTABLE_OUTPUT_PATH}/hashtable_test )

add_executable( readline_test readline.c ${PROJECT_SOURCE_DIR}/src/readline.c )
target_link_libraries( readline_test munit)
add_test( readline ${EXECUTABLE_OUTPUT_PATH}/readline_test )
