link_directories(../include/googletest/build/googlemock/gtest/)
include_directories(../include ../include/googletest/googletest/include/)

add_executable(json_packer_tests main.cpp utils_tests.cpp jsoncoder_tests_defines.h jsoncoder_tests.cpp ../src/utils.cpp ../src/apperror.cpp ../src/coder.cpp ../src/packerstream.cpp)
target_link_libraries(json_packer_tests gtest)
target_link_libraries(json_packer_tests pthread)
