import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    
    HorizontalHeaderView {
        id: workloadHeaderView
        syncView: workloadTableView
        Layout.fillWidth: true
        model: ["Сотрудник", "Проект", "Часов", "Задач"]
    }

    TableView {
        id: workloadTableView
        resizableColumns: true
        interactive: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [250, 300, 100, 80];
            return minWidths[column] || 100;
        }

        model: root.tableModel

        delegate: Rectangle {
            implicitHeight: 50
            border.width: 1
            color: row % 2 === 0 ? "white" : "#f9f9f9"

            Text {
                anchors.centerIn: parent
                text: {
                    switch (column) {
                        case 0: return model.employeeName || ""
                        case 1: return model.projectName || ""
                        case 2: return Number(model.totalHours || 0).toLocaleString(Qt.locale("ru_RU"), 'f', 2)
                        case 3: return model.taskCount || 0
                        default: return ""
                    }
                }
            }
        }
    }
}
