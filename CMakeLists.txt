cmake_minimum_required(VERSION 3.15)
project(Kurvot)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME} src/main.cpp)

add_subdirectory(lib)

target_link_libraries(Kurvot PUBLIC Kurvot_lib)