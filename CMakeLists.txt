
project(str_to_integer)

cmake_minimum_required(VERSION 2.8)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++1y" )

include_directories("include")

file(GLOB header_files "include/sti/*.hpp")
#file(GLOB source_files "src/*.cpp")
file(GLOB test_files "test/*.cpp")

add_executable(basic_test ${test_files} ${header_files})


