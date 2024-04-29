##
# build debug or release:
# cmake -DCMAKE_BUILD_TYPE=Debug ..
# cmake -DCMAKE_BUILD_TYPE=Release ..
##

##
# cmake -DCMAKE_GENERATOR_PLATFORM=x64 ..
# cmake -DCMAKE_GENERATOR_PLATFORM=x86 ..
##
#SET(CMAKE_GENERATOR_PLATFORM x86)
#SET(CMAKE_GENERATOR_PLATFORM x64)

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # no compiler extensions like gnu++11

# name
project(RCP++)
set(RABBIT_NAME RabbitControl)

# options

#set(COMPILE_AS_FRAMEWORK ON)

# to compile universal binary on macos
set(CMAKE_OSX_ARCHITECTURES x86_64;arm64)


if (MSVC)
    add_compile_options(-bigobj)
endif()

if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS true)
endif()

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


SET(CMAKE_DEBUG_POSTFIX "-d")


set_target_properties(${RABBIT_NAME} PROPERTIES
    # "current version" in semantic format in Mach-O binary file
    VERSION 0.2.0
    # "compatibility version" in semantic format in Mach-O binary file
    SOVERSION 0.2.0
    PUBLIC_HEADER "${HEADERS}"
    DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
)

if(APPLE AND COMPILE_AS_FRAMEWORK)
    set_target_properties(${RABBIT_NAME} PROPERTIES
        FRAMEWORK TRUE
        FRAMEWORK_VERSION A
        MACOSX_FRAMEWORK_IDENTIFIER cc.rabbitcontrol.libcpp
        # in case we want to ship our own Info.plist
        #MACOSX_FRAMEWORK_INFO_PLIST Info.plist
    )
endif()

install (TARGETS ${RABBIT_NAME}
    LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
    ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib
    FRAMEWORK DESTINATION ${CMAKE_BINARY_DIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_BINARY_DIR}/include
)

set (CMAKE_STATIC_LINKER_FLAGS "-v")
set (CMAKE_SHARED_LINKER_FLAGS "-v")
                                                                                
