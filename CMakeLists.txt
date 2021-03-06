cmake_minimum_required(VERSION 2.8)

find_package(Boost REQUIRED COMPONENTS)

set(Boost_USE_STATIC_LIBS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_BUILD_TYPE Debug)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_custom_target(build-time-make-directory ALL COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if (NOT DOXYGEN_FOUND)
	message(FATAL_ERROR
		"Doxygen is needed to build the documentation. Please install it correctly")
  endif()

  add_custom_target (doc
	                 ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile
					 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
					 )

endif()

add_subdirectory(src)
add_subdirectory(tests)
