cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(toplevel)

add_subdirectory(src)

if (${BUILD_EXAMPLE})
  MESSAGE(STATUS "Building example")
  add_subdirectory(example)
endif()

