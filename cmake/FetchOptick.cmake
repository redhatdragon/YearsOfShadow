include(cmake/FetchMakeAvailable.cmake)

FetchMakeAvailable(optick_core	GIT_REPOSITORY	"https://github.com/bombomby/optick.git" )
set_target_properties(OptickCore PROPERTIES FOLDER "vendor")