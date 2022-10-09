cmake_minimum_required(VERSION 3.10)

# create demo project
project(main VERSION 1.0)

# add executable to project
add_executable(main main.cpp)

# add header files from library directory
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/src)

# link the library
target_link_libraries(main PUBLIC gcaplus)