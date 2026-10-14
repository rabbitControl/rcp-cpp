cmake_minimum_required(VERSION 3.16)

project(rcp-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # no compiler extensions like gnu++11

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_SHARED "Build shared library" ON)
option(BUILD_STATIC "Build static library" OFF)

option(WITH_V010 "Build with RCP 0.1.0" OFF)


include_directories(.)

add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
