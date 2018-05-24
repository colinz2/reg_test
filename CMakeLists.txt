cmake_minimum_required(VERSION 3.10)
project(reg_test)

set(CMAKE_CXX_STANDARD 11)

link_directories(local/lib)
link_directories(local/lib64)

include_directories(local/include)

set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -g"
        )


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(reg_test reg_test.cc reg_pcre.cc reg_hyperscan.cc)
target_link_libraries(reg_test pcre hs hs_runtime)

