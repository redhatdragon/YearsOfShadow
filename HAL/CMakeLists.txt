cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_library(
	HAL
	STATIC
	${SOURCES}
	#"${CMAKE_CURRENT_SOURCE_DIR}/../.clang-format"
)

message($<CONFIG:RelWithDebInfo>)
#if($<CONFIG> STREQUAL "RelWithDebInfo")
if($<CONFIG:RelWithDebInfo>)
  target_compile_definitions(HAL PUBLIC -DALLOCATION_TRACKING)
endif()

target_include_directories(
	HAL
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

if(${IPO_SUPPORTED})
	#set_target_properties(HAL PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(MSVC)
	target_compile_options(
		HAL
		PRIVATE /W3 # warning lvl-1
		PRIVATE /MP # multi-processor compilation
		PRIVATE /arch:AVX2
		PRIVATE /WX
	)

	set_property(TARGET HAL APPEND PROPERTY LINK_FLAGS /IGNORE:4098 )
endif()

include(FetchContent) 

FetchContent_Declare(
	stacktree
	GIT_REPOSITORY https://github.com/RedSkittleFox/stacktree.git
	GIT_TAG main  
)

FetchContent_MakeAvailable(stacktree)
set_target_properties(stacktree PROPERTIES FOLDER "vendor")

set_target_properties(HAL PROPERTIES FOLDER "YearsOfShadow")

target_link_libraries(
	HAL
	GLEW
	GLFW
	GLM
	OBJ_LOADER
	imgui
	OptickCore
	fox::stacktree
)