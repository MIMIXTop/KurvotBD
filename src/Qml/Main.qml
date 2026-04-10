import QtQuick
import QtQuick.Window
import QtQuick.Layouts

Window {
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visible: true
    title: "Kurvot"
    visibility: Window.Maximized


    property int currentSelectedIndex: -1
    property int currentFilterType: -1

    Connections {
        target: filterModel

        function onFilterSelected(filterType) {
            currentFilterType = filterType
            console.log("C++ принял нажатие! Тип фильтра (enum):", filterType)
        }
    }

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 5

        Sidebar {
            id: sidebar
            model: filterModel
            Layout.preferredWidth: 300
            Layout.fillHeight: true

            onSectionSelected: function(selectedIndex) {
                currentSelectedIndex = selectedIndex

                filterModel.selectFilter(selectedIndex)
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            currentIndex: currentSelectedIndex

            Employees {
                id : employee
            }

            ProjectsAndClients {
                id: projectsAndClients
            }

            TestingAndBug {
                id: testingAndBugs
            }

            FinancesAndInfrastructure {
                id: financesAndInfrastructure
            }

            ReportsAndDocuments {
                id: reportsAndDocuments
            }
        }
    }
}