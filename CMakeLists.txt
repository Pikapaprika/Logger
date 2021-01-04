cmake_minimum_required(VERSION 3.17)
project(Logger)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -pthread")

find_package(PkgConfig REQUIRED)
pkg_check_modules(gtest REQUIRED gtest>=1.8.1)
include_directories(${gtest_INCLUDE_DIRS})
add_executable(Logger main.cpp log.h log.cpp log_test.cpp)
target_link_libraries(Logger ${gtest_LIBRARIES} stdc++fs)