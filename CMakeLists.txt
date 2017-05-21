cmake_minimum_required(VERSION 2.8)

find_package(Boost REQUIRED COMPONENTS)

set(Boost_USE_STATIC_LIBS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)
add_subdirectory(tests)
