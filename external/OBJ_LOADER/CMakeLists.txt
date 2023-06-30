cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(OBJ_LOADER INTERFACE
	${SOURCES}
)

target_include_directories(
	OBJ_LOADER
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set_target_properties(OBJ_LOADER PROPERTIES FOLDER "vendor")