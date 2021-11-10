cmake_minimum_required(VERSION 3.19)
project(network)
link_libraries(ws2_32 wsock32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread" )
set(CMAKE_CXX_STANDARD 14)

add_executable(network main.cpp http_parser.h connection_handler.h file_handler.h)
