include(cmake/FetchMakeAvailable.cmake)

FetchMakeAvailable(obj_loader GIT_REPOSITORY https://github.com/Bly7/OBJ-Loader.git)

add_library(OBJ_LOADER INTERFACE
	"${obj_loader_SOURCE_DIR}/Source/OBJ_Loader.h"
)

target_include_directories(
	OBJ_LOADER
	INTERFACE
	"${obj_loader_SOURCE_DIR}/Source"
)

set_target_properties(OBJ_LOADER PROPERTIES FOLDER "vendor")