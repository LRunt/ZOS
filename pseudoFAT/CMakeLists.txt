cmake_minimum_required(VERSION 3.23)
project(pseudoFAT)

set(CMAKE_CXX_STANDARD 20)

add_executable(pseudoFAT main.cpp Parser.cpp Parser.h Commands.cpp Commands.h)
