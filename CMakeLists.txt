cmake_minimum_required(VERSION 3.10)
project(reg)

set(CMAKE_CXX_STANDARD 11)

add_executable(reg main.cpp)

target_link_libraries(reg pcre)