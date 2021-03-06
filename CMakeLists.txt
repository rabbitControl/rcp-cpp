cmake_minimum_required(VERSION 3.1)

#add_compile_options(-std=c++11) # CMake 2.8.12 or newer
set(CMAKE_CXX_STANDARD 11) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

set(RABBIT_NAME RabbitControl)

if (MSVC)
    add_compile_options(-bigobj)
endif()

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
endif()

project(RCP)

file(GLOB SRC_APP
    *.cpp
    *.h
)

file(GLOB SOURCES
    src/*.cpp
)

file(GLOB HEADERS
    src/*.h
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${SRC_APP}) #"rcptest.cpp"

add_library(${RABBIT_NAME} SHARED ${HEADERS} ${SOURCES})

target_include_directories(${RABBIT_NAME} PUBLIC ${HEADERS})

set_target_properties(${RABBIT_NAME} PROPERTIES
#  FRAMEWORK TRUE
#  FRAMEWORK_VERSION A
#  MACOSX_FRAMEWORK_IDENTIFIER io.rcp.rcpcpp
  #MACOSX_FRAMEWORK_INFO_PLIST Info.plist
  # "current version" in semantic format in Mach-O binary file
  VERSION 0.1.0
  # "compatibility version" in semantic format in Mach-O binary file
  SOVERSION 0.1.0
  PUBLIC_HEADER "${HEADERS}"
)

install (TARGETS ${RABBIT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    FRAMEWORK DESTINATION .
    PUBLIC_HEADER DESTINATION include
)

set (CMAKE_STATIC_LINKER_FLAGS "-v")
set (CMAKE_SHARED_LINKER_FLAGS "-v")
