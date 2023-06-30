cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

add_library(GLFW INTERFACE
	${SOURCES}
)

target_include_directories(
	GLFW
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(
	GLFW
	INTERFACE
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3_mt.lib"
	"${CMAKE_CURRENT_SOURCE_DIR}/lib/glfw3.lib"
)

set_target_properties(GLFW PROPERTIES FOLDER "vendor")