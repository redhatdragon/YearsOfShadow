cmake_minimum_required(VERSION 3.6)

#NOTE: GAME_TYPE 0 = single, 2 = client, 3 = server







file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES})

add_executable(
	YearsOfShadow_Single
	${SOURCES}
	#"${CMAKE_CURRENT_SOURCE_DIR}/../.clang-format"  #looking for replacement...
)

target_include_directories(
	YearsOfShadow_Single
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

if(${IPO_SUPPORTED})
	#set_target_properties(YearsOfShadow_Single PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set_target_properties(YearsOfShadow_Single PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../bin-game/")

if(MSVC)
	target_compile_options(
		YearsOfShadow_Single
			PRIVATE /W3 # warning lvl-1
			PRIVATE /MP # multi-processor compilation
			PRIVATE /arch:AVX2
			PRIVATE /WX
	)

	set_property(TARGET YearsOfShadow_Single APPEND PROPERTY LINK_FLAGS /IGNORE:4098 )
	#set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT YearsOfShadow_Single )
	set_target_properties(YearsOfShadow_Single PROPERTIES LINK_FLAGS "/PROFILE")
endif()

set_target_properties(YearsOfShadow_Single PROPERTIES FOLDER "YearsOfShadow")

target_compile_definitions(YearsOfShadow_Single PRIVATE GAME_TYPE=0)

target_link_libraries(
	YearsOfShadow_Single
	PUBLIC
	EchoEngine
	OptickCore
)







add_executable(
	YearsOfShadow_Client
	${SOURCES}
	#"${CMAKE_CURRENT_SOURCE_DIR}/../.clang-format"  #looking for replacement...
)

target_include_directories(
	YearsOfShadow_Client
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(YearsOfShadow_Client PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../bin-game/")

if(MSVC)
	target_compile_options(
		YearsOfShadow_Client
			PRIVATE /W3 # warning lvl-1
			PRIVATE /MP # multi-processor compilation
			PRIVATE /arch:AVX2
			PRIVATE /WX
	)

	set_property(TARGET YearsOfShadow_Client APPEND PROPERTY LINK_FLAGS /IGNORE:4098 )
	#set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT YearsOfShadow_Client )
endif()

set_target_properties(YearsOfShadow_Client PROPERTIES FOLDER "YearsOfShadow")

target_compile_definitions(YearsOfShadow_Client PRIVATE GAME_TYPE=1)

target_link_libraries(
	YearsOfShadow_Client
	PUBLIC
	EchoEngine
	OptickCore
)







add_executable(
	YearsOfShadow_Server
	${SOURCES}
	#"${CMAKE_CURRENT_SOURCE_DIR}/../.clang-format"  #looking for replacement...
)

target_include_directories(
	YearsOfShadow_Server
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(YearsOfShadow_Server PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../bin-game/")

if(MSVC)
	target_compile_options(
		YearsOfShadow_Server
			PRIVATE /W3 # warning lvl-1
			PRIVATE /MP # multi-processor compilation
			PRIVATE /arch:AVX2
			PRIVATE /WX
	)

	set_property(TARGET YearsOfShadow_Server APPEND PROPERTY LINK_FLAGS /IGNORE:4098 )
	#set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT YearsOfShadow_Server )
endif()

set_target_properties(YearsOfShadow_Server PROPERTIES FOLDER "YearsOfShadow")

target_compile_definitions(YearsOfShadow_Server PRIVATE GAME_TYPE=2)

target_link_libraries(
	YearsOfShadow_Server
	PUBLIC
	EchoEngine
	OptickCore
)