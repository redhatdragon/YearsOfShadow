cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_library(
	EchoEngine
	STATIC
	${SOURCES}
	#"${CMAKE_CURRENT_SOURCE_DIR}/../.clang-format"
)

target_include_directories(
	EchoEngine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

if(${IPO_SUPPORTED})
	#set_target_properties(EchoEngine PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

if(MSVC)
	target_compile_options(
		EchoEngine
		PRIVATE /W3 # warning lvl-1
		PRIVATE /MP # multi-processor compilation
		PRIVATE /arch:AVX2
		PRIVATE /WX
	)

	set_property(TARGET EchoEngine APPEND PROPERTY LINK_FLAGS /IGNORE:4098 )
endif()

set_target_properties(EchoEngine PROPERTIES FOLDER "YearsOfShadow")

target_link_libraries(
	EchoEngine
	PUBLIC
	HAL
)