cmake_minimum_required(VERSION 3.12)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Configure.cmake")

set(ProjectId main)
project(${ProjectId})

add_subdirectory(containers)

add_executable(${ProjectId} main.cpp)
target_link_libraries(${ProjectId} Containers)
