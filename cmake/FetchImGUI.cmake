include(cmake/FetchMakeAvailable.cmake)

FetchMakeAvailable(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG docking)

FetchContent_MakeAvailable(imgui)

file(COPY 
		${imgui_SOURCE_DIR}/imconfig.h
		${imgui_SOURCE_DIR}/imgui.cpp
		${imgui_SOURCE_DIR}/imgui.h
		${imgui_SOURCE_DIR}/imgui_demo.cpp
		${imgui_SOURCE_DIR}/imgui_draw.cpp
		${imgui_SOURCE_DIR}/imgui_internal.h
		${imgui_SOURCE_DIR}/imgui_tables.cpp
		${imgui_SOURCE_DIR}/imgui_widgets.cpp
		${imgui_SOURCE_DIR}/imstb_rectpack.h
		${imgui_SOURCE_DIR}/imstb_textedit.h
		${imgui_SOURCE_DIR}/imstb_truetype.h
		${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
		${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
		${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
		${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
		${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3_loader.h
	DESTINATION 
		${imgui_SOURCE_DIR}/include/imgui/
)

file(GLOB_RECURSE SOURCES
	"${imgui_SOURCE_DIR}/include/*.cpp"
	"${imgui_SOURCE_DIR}/include/*.h"
	"${imgui_SOURCE_DIR}/include/*.hpp"
)

add_library(imgui STATIC
	${SOURCES}
)

target_include_directories(
	imgui
	PUBLIC
	"${imgui_SOURCE_DIR}/include"
)

set_target_properties(imgui PROPERTIES FOLDER "vendor")

target_link_libraries(
	imgui
	GLFW
)