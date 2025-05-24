cmake_minimum_required(VERSION 3.14)
project(FMIndex LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC_FILES CMAKE_CONFIGURE_DEPENDS *.cpp)

add_executable(fm_index ${SRC_FILES})