cmake_minimum_required(VERSION 2.8)

enable_testing()

include_directories(. ..)

add_executable(test_array test_array.c ../array.c)
target_link_libraries(test_array check)

add_test(test_array ${EXECUTABLE_OUTPUT_PATH}/test_array)
