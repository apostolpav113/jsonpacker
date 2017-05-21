cmake_minimum_required(VERSION 2.8)

find_package(Boost REQUIRED COMPONENTS)

link_directories(/usr/local/lib)
include_directories(include)
set(Boost_USE_STATIC_LIBS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)

project(json_packer)
add_executable(${PROJECT_NAME} "main.cpp" "appoptions.cpp" "coder.cpp" "packerstream.cpp" "utils.cpp" "error.cpp")
#target_link_libraries(${PROJECT_NAME} boost_serialization)
target_link_libraries(${PROJECT_NAME} boost_system)
target_link_libraries(${PROJECT_NAME} boost_filesystem)
target_link_libraries(${PROJECT_NAME} boost_program_options)
