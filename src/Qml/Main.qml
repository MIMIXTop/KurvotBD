import QtQuick
import QtQuick.Window
import QtQuick.Layouts

Window {
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visible: true
    title: "Kurvot"


    property int currentSelectedIndex: -1
    property int currentFilterType: -1

    Connections {
        target: filterModel

        function onFilterSelected(filterType) {
            // Сохраняем filterType, который прислал C++
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
                // 1. Сохраняем индекс строки для QML-интерфейса
                currentSelectedIndex = selectedIndex

                // 2. Отправляем индекс в C++ (вызов вашего Q_INVOKABLE)
                filterModel.selectFilter(selectedIndex)
            }
        }

        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            currentIndex: currentSelectedIndex
            
            Employeers {
                id : employee
            }

            ProjectsAndClients {
                id: projectAndClient
            }

            TestingAndBug {
                id: testingAndBug
            }

            Sales {
                id: sales
            }

            Documents {
                id: documents
            }
        }

        // Rectangle {
        //     id: mainContent
        //     Layout.fillWidth: true
        //     Layout.fillHeight: true
        //     color: "#f0f0f0"
        //
        //     Column {
        //         anchors.centerIn: parent
        //         spacing: 20
        //
        //         Text {
        //             text: currentSelectedIndex === -1
        //                 ? "Выберите фильтр в меню слева"
        //                 : "Нажат индекс (строка): " + currentSelectedIndex
        //
        //             font.pixelSize: 28
        //             font.bold: true
        //             color: currentSelectedIndex === -1 ? "gray" : "#333333"
        //         }
        //
        //         Text {
        //             visible: currentSelectedIndex !== -1
        //             text: "ID фильтра (из C++ enum): " + currentFilterType
        //             font.pixelSize: 20
        //             color: "indianred"
        //         }
        //     }
        // }
    }
}