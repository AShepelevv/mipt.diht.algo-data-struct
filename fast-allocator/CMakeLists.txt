cmake_minimum_required(VERSION 3.7)
project(F_problem)

set(CMAKE_CXX_STANDARD 14)

set(CXXFLAGS -Wall -Wextra -Werror -O2)

set(SOURCE_FILES main.cpp)
add_executable(F_problem ${SOURCE_FILES})