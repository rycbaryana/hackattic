cmake_minimum_required(VERSION 3.5.0)
project(hackattic VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined -march=native")

add_subdirectory(help_me_unpack)
add_subdirectory(mini_miner)
add_subdirectory(tools)
add_subdirectory(tales_of_ssl)
include_directories(tools)


