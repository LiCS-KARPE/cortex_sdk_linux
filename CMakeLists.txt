project(CORTEX_LINUX)
cmake_minimum_required(VERSION 2.6)

# COMPILER FLAGS (default: Release)
# use "-DCMAKE_BUILD_TYPE=Debug" in cmake for a Debug-build
IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
message(STATUS "Build type is " ${CMAKE_BUILD_TYPE})

# COMPILER FLAGS
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb")

add_library(cortex_sdk m3x3.cpp cortex_socket.cpp cortex_unpack.cpp cortex.cpp)
target_link_libraries(cortex_sdk pthread m)

target_include_directories(cortex_sdk
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

add_executable(clienttest clienttest.cpp)
target_link_libraries(clienttest cortex_sdk)
