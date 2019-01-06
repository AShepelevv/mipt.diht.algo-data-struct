cmake_minimum_required(VERSION 3.6)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp ImplicitTreap.h Node.h)
add_executable(untitled ${SOURCE_FILES})