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
    src/Models/ProjectsTableModel.hpp
    src/Models/ProjectsTableModel.cpp
    src/Models/ClientsTableModel.hpp
    src/Models/ClientsTableModel.cpp
    src/Models/BugsTableModel.hpp
    src/Models/BugsTableModel.cpp
    src/Models/ReleasesTableModel.hpp
    src/Models/ReleasesTableModel.cpp
    src/Models/TestingEfficiencyTableModel.hpp
    src/Models/TestingEfficiencyTableModel.cpp
    src/Models/ProfitabilityTableModel.hpp
    src/Models/ProfitabilityTableModel.cpp
    src/Models/InfrastructureTableModel.hpp
    src/Models/InfrastructureTableModel.cpp
    src/Models/MonthlyFinancialTableModel.hpp
    src/Models/MonthlyFinancialTableModel.cpp
    src/Models/ProjectTeamTableModel.hpp
    src/Models/ProjectTeamTableModel.cpp
    src/Models/ProjectStatusModel.hpp
    src/Models/ProjectStatusModel.cpp
    src/Models/ProjectTechnologiesTableModel.hpp
    src/Models/ProjectTechnologiesTableModel.cpp
    src/Models/DocumentationTableModel.hpp
    src/Models/DocumentationTableModel.cpp
    src/Models/SpecificationTableModel.hpp
    src/Models/SpecificationTableModel.cpp
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
        src/Qml/FinancesAndInfrastructure.qml
        src/Qml/ReportsAndDocuments.qml
        src/Qml/TeamReports.qml
        src/Qml/StatusReports.qml
        src/Qml/Technologies.qml
        src/Qml/Documentation.qml
)

add_subdirectory(lib)

target_link_libraries(Kurvot PUBLIC Kurvot_lib Qt6::Core Qt6::Qml Qt6::Quick Qt6::Gui Qt6::Widgets)

enable_testing()

file(GLOB_RECURSE TEST_SRCS test/*.cpp)

add_executable(Kurvot_tests ${TEST_SRCS})

target_include_directories(Kurvot_tests PRIVATE lib/src)

target_link_libraries(Kurvot_tests PRIVATE Kurvot_lib gtest gtest_main)