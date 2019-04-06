cmake_minimum_required(VERSION 3.13)
project(example)

set(CMAKE_CXX_STANDARD 14)
add_executable(example main.cpp)
target_link_libraries(example /usr/lib/tobii/libtobii_stream_engine.so)
target_link_libraries(example pthread)

