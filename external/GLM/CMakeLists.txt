cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(GLM INTERFACE
	${SOURCES}
)

target_include_directories(
	GLM
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set_target_properties(GLM PROPERTIES FOLDER "vendor")