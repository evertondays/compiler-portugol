cmake_minimum_required(VERSION 3.10)
project(compiler_portugol)

set(CMAKE_CXX_STANDARD 20)

include_directories(core)

add_executable(main main.cpp core/parser.cpp
        core/lexical.cpp
        core/lexical.h)
