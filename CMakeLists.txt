cmake_minimum_required(VERSION 3.10)

# PROJECT NAME
project(GASC C)

# STANDARD
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

file(GLOB_RECURSE SOURCES "src/*.c")

# ADD FILES
add_executable(gasc ${SOURCES})

set(CMAKE_C_FLAGS_DEBUG "-g")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
