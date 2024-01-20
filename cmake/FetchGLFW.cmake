include(cmake/FetchMakeAvailable.cmake)

FetchMakeAvailable(glfw URL https://github.com/glfw/glfw/releases/download/3.3.9/glfw-3.3.9.bin.WIN64.zip)

add_library(GLFW INTERFACE
	"${glfw_SOURCE_DIR}/include/GLFW/glfw3.h"
	"${glfw_SOURCE_DIR}/include/GLFW/glfw3native.h"
)

target_include_directories(
	GLFW
	INTERFACE
	"${glfw_SOURCE_DIR}/include"
)

target_link_libraries(
	GLFW
	INTERFACE
	"${glfw_SOURCE_DIR}/lib-vc2022/glfw3.lib"
	"${glfw_SOURCE_DIR}/lib-vc2022/glfw3_mt.lib"
)

set_target_properties(GLFW PROPERTIES FOLDER "vendor")