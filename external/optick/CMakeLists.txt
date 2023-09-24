cmake_minimum_required(VERSION 3.21)

include(FetchContent) 

FetchContent_Declare(
	OptickCore
	GIT_REPOSITORY https://github.com/bombomby/optick.git
)

FetchContent_MakeAvailable(OptickCore)

set_target_properties(OptickCore PROPERTIES FOLDER "vendor")