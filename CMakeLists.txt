cmake_minimum_required(VERSION 3.12)

include("${CMAKE_SOURCE_DIR}/cmake/Configure.cmake")

SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
include_directories("${CMAKE_SOURCE_DIR}/3rdparty/gtest/include")
include_directories("${CMAKE_SOURCE_DIR}/3rdparty/gtest/googletest/include")
add_subdirectory("${CMAKE_SOURCE_DIR}/3rdparty/gtest")

include_directories("${CMAKE_SOURCE_DIR}/3rdparty/")

set(ProjectId main)
project(${ProjectId})

add_subdirectory(containers)
add_subdirectory(matrix_op)
add_subdirectory(tests)

add_executable(${ProjectId} main.cpp)
target_link_libraries(${ProjectId} containers matrix_op)
