import QtQuick
import QtQuick.Window
import QtQuick.Layouts

Window {
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visible: true
    title: "Kurvot"

    // Создаем свойство на уровне окна для хранения индекса.
    // -1 означает, что ничего не выбрано
    property int currentSelectedIndex: -1
    property int currentFilterType: -1

    // Слушаем сигнал из вашего C++ класса ButtonFilterModel
    Connections {
        target: filterModel // Убедитесь, что объект в main.cpp зарегистрирован именно под этим именем

        // Название функции формируется как "on" + название сигнала с большой буквы
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
            Layout.preferredWidth: 300
            Layout.fillHeight: true

            model: filterModel

            onFilterSelected: function(selectedIndex) {
                // 1. Сохраняем индекс строки для QML-интерфейса
                currentSelectedIndex = selectedIndex

                // 2. Отправляем индекс в C++ (вызов вашего Q_INVOKABLE)
                filterModel.selectFilter(selectedIndex)
            }
        }

        Rectangle {
            id: mainContent
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f0f0f0"

            Column {
                anchors.centerIn: parent
                spacing: 20

                Text {
                    text: currentSelectedIndex === -1
                        ? "Выберите фильтр в меню слева"
                        : "Нажат индекс (строка): " + currentSelectedIndex

                    font.pixelSize: 28
                    font.bold: true
                    color: currentSelectedIndex === -1 ? "gray" : "#333333"
                }

                Text {
                    visible: currentSelectedIndex !== -1
                    text: "ID фильтра (из C++ enum): " + currentFilterType
                    font.pixelSize: 20
                    color: "indianred"
                }
            }
        }
    }
}