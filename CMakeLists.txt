cmake_minimum_required(VERSION 3.1)

#add_compile_options(-std=c++11) # CMake 2.8.12 or newer
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

project(RCP)

file(GLOB SOURCES
    *.cpp
    *.h
    src/*.h
    src/*.cpp
)

#add_executable(${PROJECT_NAME} "main.cpp")
add_executable(${PROJECT_NAME} ${SOURCES}) #"rcptest.cpp"

#include_directories ("./model/")     # make sure your .h all inside.

# add exmaple data to output directory
FILE(COPY ../RCP/example_data DESTINATION "${CMAKE_BINARY_DIR}")

set (CMAKE_STATIC_LINKER_FLAGS "-v")
set (CMAKE_SHARED_LINKER_FLAGS "-v")
