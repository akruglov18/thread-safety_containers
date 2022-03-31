set(ProjectId containers_tests)
project(${ProjectId})

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h *.hpp)

add_executable(${ProjectId} ${ALL_SOURCE_FILES})
target_link_libraries(${ProjectId} containers matrix_op gtest_main)
