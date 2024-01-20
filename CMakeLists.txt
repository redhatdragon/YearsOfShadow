cmake_minimum_required(VERSION 3.25)

cmake_policy(PUSH)


cmake_policy(SET CMP0135 NEW)
project(YearsOfShadow VERSION 0.1.0 LANGUAGES C CXX)

if (PROJECT_IS_TOP_LEVEL)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    	
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin-etc")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
    
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_STANDARD_REQUIRED)

    if(CMAKE_CONFIGURATION_TYPES AND MSVC)
        list(APPEND CMAKE_CONFIGURATION_TYPES ReleaseProfile)
        list(REMOVE_DUPLICATES CMAKE_CONFIGURATION_TYPES)

        set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
            "Add the configurations that we need"
        FORCE)

        set(CMAKE_CXX_FLAGS_RELEASEPROFILE "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}" CACHE STRING
            "Flags used by the C++ compiler during ReleaseProfile builds."
        FORCE)

        set(CMAKE_C_FLAGS_RELEASEPROFILE "${CMAKE_C_FLAGS_RELWITHDEBINFO}" CACHE STRING
            "Flags used by the C compiler during ReleaseProfile builds."
        FORCE)

        set(CMAKE_EXE_LINKER_FLAGS_RELEASEPROFILE "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /PROFILE" CACHE STRING
            "Flags used for linking binaries during ReleaseProfile builds."
        FORCE)

        set(CMAKE_SHARED_LINKER_FLAGS_RELEASEPROFILE "${CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO} /PROFILE" CACHE STRING
            "Flags used by the shared libraries linker during ReleaseProfile builds."
        FORCE)

        mark_as_advanced(
            CMAKE_CXX_FLAGS_RELEASEPROFILE
            CMAKE_C_FLAGS_RELEASEPROFILE
            CMAKE_EXE_LINKER_FLAGS_RELEASEPROFILE
            CMAKE_SHARED_LINKER_FLAGS_RELEASEPROFILE
        )
        
        # Update the documentation string of CMAKE_BUILD_TYPE for GUIs
        set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ReleaseProfile."
        FORCE)

    endif()

endif()
    
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
enable_testing()

include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_NOT_SUPPORTED_ERROR LANGUAGES CXX)

include(cmake/FetchGLEW.cmake)
include(cmake/FetchGLFW.cmake)
include(cmake/FetchGLM.cmake)
include(cmake/FetchOBJ_Loader.cmake)
include(cmake/FetchOptick.cmake)
include(cmake/FetchImGUI.cmake)

add_subdirectory(HAL)
add_subdirectory(EchoEngine)
add_subdirectory(YearsOfShadow)
add_subdirectory("test")

cmake_policy(POP)