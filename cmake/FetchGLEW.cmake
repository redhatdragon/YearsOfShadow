include(cmake/FetchMakeAvailable.cmake)

FetchMakeAvailable(glew URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip)

add_library(GLEW INTERFACE
	${glew_SOURCE_DIR}/include/GL/eglew.h
	${glew_SOURCE_DIR}/include/GL/glew.h
	${glew_SOURCE_DIR}/include/GL/glxew.h
	${glew_SOURCE_DIR}/include/GL/wglew.h
)

target_include_directories(
	GLEW
	INTERFACE
	"${glew_SOURCE_DIR}/include"
)

target_link_libraries(
	GLEW
	INTERFACE
	"${glew_SOURCE_DIR}/lib/Release/x64/glew32s.lib"
)

set_target_properties(GLEW PROPERTIES FOLDER "vendor")