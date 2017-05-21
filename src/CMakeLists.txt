link_directories(/usr/local/lib)
include_directories(../include)

project(json_packer)

SET(SOURCES
	"main.cpp"
	"appoptions.cpp"
	"coder.cpp"
	"packerstream.cpp"
	"utils.cpp"
	"apperror.cpp")

SET(HEADERS
  "../include/appoptions.h"
  "../include/coder.h"
  "../include/packerstream.h"
  "../include/utils.h"
  "../include/apperror.h"
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} boost_system)
target_link_libraries(${PROJECT_NAME} boost_filesystem)
target_link_libraries(${PROJECT_NAME} boost_program_options)
