cmake_minimum_required(VERSION 3.25)

include(FetchContent)

cmake_policy(SET CMP0135 NEW)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE sources 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
    )

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})

add_executable(
    unit_testing
    ${sources}
)

set_target_properties(unit_testing PROPERTIES FOLDER "YearsOfShadow")

target_link_libraries(
    unit_testing
    GTest::gtest_main
    GTest::gmock_main
    EchoEngine
)

include(GoogleTest)
gtest_discover_tests(unit_testing)

set_target_properties(gtest_main PROPERTIES FOLDER "vendor")
set_target_properties(gtest PROPERTIES FOLDER "vendor")
set_target_properties(gmock_main PROPERTIES FOLDER "vendor")
set_target_properties(gmock PROPERTIES FOLDER "vendor")