cmake_minimum_required(VERSION 3.15)
project(compiler CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_executable(compiler ${SOURCES})
target_include_directories(compiler PRIVATE src)
