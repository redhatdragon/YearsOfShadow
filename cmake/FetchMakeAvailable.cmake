macro(FetchMakeAvailable arg_name arg_type arg_url)
	include(FetchContent)
	
	if(NOT EXISTS ${FETCHCONTENT_BASE_DIR}/glew)
		FetchContent_Declare(
			${arg_name}
			${arg_type}		${arg_url}
			${ARGV3} ${ARGV4}
		)
	
		FetchContent_MakeAvailable(${arg_name})
	else()
		set(${arg_name}_SOURCE_DIR "${FETCHCONTENT_BASE_DIR}/${arg_name}")
	
		if(EXISTS ${FETCHCONTENT_BASE_DIR}/${arg_name}/CMakeLists.txt)
			add_subdirectory(${FETCHCONTENT_BASE_DIR}/${arg_name})
		endif()
	endif()

endmacro()



