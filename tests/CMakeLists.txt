cmake_minimum_required(VERSION 3.23)

include_directories(../src/include)
include_directories(libs)

enable_testing()

set(V010_FILES
  0.1.0/Helper.h

  # NOTE: needs RCP test-data to be in place
  v010ParsingTest.cpp

  v010ParameterTest.cpp
  v010ParameterGroup.cpp
  ClientTest.cpp
)

add_executable(RcpTests
  Reporter.cpp
  Utils.h
  Utils.cpp
  1.0/Helper.h  

  v10BasicTypesTest.cpp
  v10TestFiles.cpp
  v10Benchmarks.cpp
  v10ParameterTest.cpp
  v10EnumTest.cpp
  v10ParameterGroup.cpp
)

if(WITH_V010)
  target_sources(RcpTests PRIVATE ${V010_FILES})
endif()

target_compile_definitions(RcpTests PUBLIC TEST_DATA_V1=\"${TEST_DATA_V1}\")
target_compile_definitions(RcpTests PUBLIC TEST_DATA_V010=\"${TEST_DATA_V010}\")

target_link_libraries(RcpTests PRIVATE RabbitControl)
target_link_libraries(RcpTests PRIVATE Catch2::Catch2WithMain)


Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.16.0
)

FetchContent_MakeAvailable(Catch2)


list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(RcpTests)
