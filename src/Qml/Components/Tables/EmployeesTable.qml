import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    
    HorizontalHeaderView {
        id: horizontalHeader
        syncView: tableView
        Layout.fillWidth: true
        model: ["Имя", "Отдел", "Должность", "Возраст", "Зарплата", "Опыт", "Email", "Телефон"]
    }

    TableView {
        id: tableView
        resizableColumns: true
        interactive: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [300, 230, 180, 50, 150, 80, 180, 120];
            return minWidths[column] || 100;
        }

        model: root.tableModel

        delegate: Rectangle {
            id: tile
            implicitHeight: 50
            border.width: 1

            Text {
                anchors.centerIn: parent

                text: {
                    switch (column) {
                        case 0:
                            return model.employeeName || ""
                        case 1:
                            return model.departmentName || ""
                        case 2:
                            return model.positionTitle || ""
                        case 3:
                            return model.age || ""
                        case 4:
                            return Number(model.salary || 0).toLocaleString(Qt.locale("ru_RU"), 'f', 2)
                        case 5:
                            return model.yearsOfExperience || ""
                        case 6:
                            return model.email || ""
                        case 7:
                            return model.phoneNumber || ""
                        default:
                            return ""
                    }
                }
            }
        }
    }
}
