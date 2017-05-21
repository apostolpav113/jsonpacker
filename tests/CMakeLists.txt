link_directories(../include/googletest/build/googlemock/gtest/)
include_directories(../include ../include/googletest/googletest/include/)

add_executable(json_packer_tests "main.cpp")
target_link_libraries(json_packer_tests gtest)
