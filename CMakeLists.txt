cmake_minimum_required(VERSION 3.26)
project(Tesoro)

set(CMAKE_CXX_STANDARD 17)

add_executable(Tesoro main.cpp
        Board.cpp
        Board.h
)
