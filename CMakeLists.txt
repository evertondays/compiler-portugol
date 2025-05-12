cmake_minimum_required(VERSION 3.10)
project(compiler_portugol)

set(CMAKE_CXX_STANDARD 20)


set(SOURCE_FILES
        tokens.cpp
        main.cpp
)

add_executable(compiler_portugol ${SOURCE_FILES})
