import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    signal itemDoubleClicked(string bugInfo)
    
    HorizontalHeaderView {
        id: bugsHeaderView
        syncView: bugsTableView
        Layout.fillWidth: true
        model: ["Проект", "Название", "Описание", "Статус", "Серьезность", "Найден кем", "Исправлен кем", "Создан", "Дата обнаружения", "Дата исправления"]
    }

    TableView {
        id: bugsTableView
        resizableColumns: true
        interactive: false
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [150, 200, 250, 120, 120, 200, 200, 150, 120, 120];
            return minWidths[column] || 100;
        }

        model: root.tableModel

        delegate: Rectangle {
            implicitHeight: 50
            border.width: 1
            color: row % 2 === 0 ? "white" : "#f9f9f9"

            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    let info = "Проект: " + (model.projectName || "") +
                        "\nНазвание: " + (model.title || "") + 
                        "\nОписание: " + (model.description || "") + 
                        "\nСтатус: " + (model.status || "") + 
                        "\nСерьезность: " + (model.severity || "") + 
                        "\nНайден кем: " + (model.foundBy || "") + 
                        "\nИсправлен кем: " + (model.fixedBy || "") + 
                        "\nСоздан: " + (model.createdAt || "") + 
                        "\nДата обнаружения: " + (model.foundDate || "") + 
                        "\nДата исправления: " + (model.fixedDate || "")
                    root.itemDoubleClicked(info)
                }
            }

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: {
                    switch (column) {
                        case 0: return model.projectName || ""
                        case 1: return model.title || ""
                        case 2: return model.description || ""
                        case 3: return model.status || ""
                        case 4: return model.severity || ""
                        case 5: return model.foundBy || ""
                        case 6: return model.fixedBy || ""
                        case 7: return model.createdAt || ""
                        case 8: return model.foundDate || ""
                        case 9: return model.fixedDate || ""
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
