cmake_minimum_required(VERSION 3.20)
project(TeoriaGrafos)

set(CMAKE_CXX_STANDARD 17)

add_executable(TeoriaGrafos main.cpp Graph.h Node.h Edge.h Graph.cpp Node.cpp Edge.cpp)
