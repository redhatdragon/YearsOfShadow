include(cmake/FetchMakeAvailable.cmake)

FetchMakeAvailable(glm GIT_REPOSITORY https://github.com/g-truc/glm.git)
set_target_properties(glm PROPERTIES FOLDER "vendor")