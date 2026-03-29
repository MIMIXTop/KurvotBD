cmake_minimum_required(VERSION 3.15)
project(Kurvot)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(lib)

target_link_libraries(Kurvot PUBLIC Kurvot_lib)

# Tests
enable_testing()

file(GLOB_RECURSE TEST_SRCS test/*.cpp)

add_executable(Kurvot_tests ${TEST_SRCS})

target_include_directories(Kurvot_tests PRIVATE lib/src)

target_link_libraries(Kurvot_tests PRIVATE Kurvot_lib gtest gtest_main)