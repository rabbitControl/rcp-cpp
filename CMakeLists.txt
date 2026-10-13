cmake_minimum_required(VERSION 3.16)

project(rcp-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # no compiler extensions like gnu++11

include_directories(.)

add_subdirectory(src)
add_subdirectory(tests)
