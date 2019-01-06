cmake_minimum_required(VERSION 3.7)
project(H_problem)

set(CMAKE_CXX_STANDARD 14)

set(CXXFLAGS -std=c++14 -O2 -Wall -Wextra -Werror)

set(SOURCE_FILES main.cpp)
add_executable(H_problem ${SOURCE_FILES})