cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(GLEW INTERFACE
	${SOURCES}
)

target_include_directories(
	GLEW
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
	GLEW
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/glew32s.lib"
)

set_target_properties(GLEW PROPERTIES FOLDER "vendor")