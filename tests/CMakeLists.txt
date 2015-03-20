cmake_minimum_required(VERSION 2.8)

include_directories(. ..)

add_executable(test_array test_array.c ../array.c)
target_link_libraries(test_array check)

add_test(test_array ${EXECUTABLE_OUTPUT_PATH}/test_array)

# evil trick to prevent collision with check function
add_definitions(-Dlist_front=pceas_list_front)
add_executable(test_list test_list.c ../list.c)
target_link_libraries(test_list check)

add_test(test_list ${EXECUTABLE_OUTPUT_PATH}/test_list)
