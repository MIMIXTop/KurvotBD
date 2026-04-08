cmake_minimum_required(VERSION 3.16)
project(Kurvot)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Gui Widgets)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(Kurvot
    src/main.cpp
    src/Models/ButtonFilterModel.hpp
    src/Models/ButtonFilterModel.cpp
    src/Models/EmployeesTableModel.hpp
    src/Models/EmployeesTableModel.cpp
    src/Models/EmployeesWorkloadModel.hpp
    src/Models/EmployeesWorkloadModel.cpp
)

set_target_properties(Kurvot PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

qt_add_qml_module(Kurvot
    URI Kurvot_URI
    VERSION 1.0
    QML_FILES
        src/Qml/Main.qml
        src/Qml/MainWidget.qml
        src/Qml/FilterDelegate.qml
        src/Qml/Sidebar.qml
        src/Qml/Employees.qml
        src/Qml/Sales.qml
        src/Qml/Documents.qml
        src/Qml/TestingAndBug.qml
        src/Qml/ProjectsAndClients.qml
)

add_subdirectory(lib)

target_link_libraries(Kurvot PUBLIC Kurvot_lib Qt6::Core Qt6::Qml Qt6::Quick Qt6::Gui Qt6::Widgets)

enable_testing()

file(GLOB_RECURSE TEST_SRCS test/*.cpp)

add_executable(Kurvot_tests ${TEST_SRCS})

target_include_directories(Kurvot_tests PRIVATE lib/src)

target_link_libraries(Kurvot_tests PRIVATE Kurvot_lib gtest gtest_main)