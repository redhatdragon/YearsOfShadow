cmake_minimum_required(VERSION 3.21)

file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable(
	YearsOfShadow
	${SOURCES}
	#"${CMAKE_CURRENT_SOURCE_DIR}/../.clang-format"  #looking for replacement...
)

target_include_directories(
	YearsOfShadow
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

if(${IPO_SUPPORTED})
	#set_target_properties(YearsOfShadow PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set_target_properties(YearsOfShadow PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../bin-game/")

if(MSVC)
	target_compile_options(
		YearsOfShadow
			PRIVATE /W3 # warning lvl-1
			PRIVATE /MP # multi-processor compilation
			PRIVATE /arch:AVX2
			PRIVATE /WX
	)

	set_property(TARGET YearsOfShadow APPEND PROPERTY LINK_FLAGS /IGNORE:4098 )
endif()

set_target_properties(YearsOfShadow PROPERTIES FOLDER "YearsOfShadow")

target_link_libraries(
	YearsOfShadow
	PUBLIC
	EchoEngine
	OptickCore
)