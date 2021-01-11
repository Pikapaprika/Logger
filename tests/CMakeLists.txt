cmake_minimum_required(VERSION 3.17)
find_package(PkgConfig REQUIRED)
pkg_check_modules(gtest REQUIRED gtest>=1.8.1)
include_directories(${gtest_INCLUDE_DIRS})
add_executable(LogTest log_test.cpp ../log_utils.h)
target_link_libraries(LogTest Log ${gtest_LIBRARIES} stdc++fs)
target_include_directories(LogTest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

