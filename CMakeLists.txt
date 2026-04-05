cmake_minimum_required(VERSION 3.15)
project(Kurvot)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Gui Widgets)

qt_add_executable(Kurvot src/main.cpp)

qt_add_qml_module(Kurvot
    URI Kurvot_URI
    QML_FILES
        src/Main.qml
)

add_subdirectory(lib)

target_link_libraries(Kurvot PUBLIC Kurvot_lib Qt6::Core Qt6::Qml Qt6::Quick Qt6::Gui Qt6::Widgets)

enable_testing()

file(GLOB_RECURSE TEST_SRCS test/*.cpp)

add_executable(Kurvot_tests ${TEST_SRCS})

target_include_directories(Kurvot_tests PRIVATE lib/src)

target_link_libraries(Kurvot_tests PRIVATE Kurvot_lib gtest gtest_main)