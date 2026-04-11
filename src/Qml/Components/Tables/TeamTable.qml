import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    signal itemDoubleClicked(string teamInfo)
    
    HorizontalHeaderView {
        id: teamHeaderView
        syncView: teamTableView
        Layout.fillWidth: true
        model: ["Проект", "ФИО", "Должность", "Роль", "Фаза", "Часы"]
    }

    TableView {
        id: teamTableView
        resizableColumns: true
        interactive: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [200, 200, 150, 150, 150, 80];
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
                        "\nФИО: " + (model.fullName || "") +
                        "\nДолжность: " + (model.positionTitle || "") +
                        "\nРоль: " + (model.projectRole || "") +
                        "\nФаза: " + (model.phaseName || "") +
                        "\nДата начала: " + (model.startDate || "") +
                        "\nДата окончания: " + (model.endDate || "") +
                        "\nЧасы выделено: " + (model.hoursAllocated || 0)
                    root.itemDoubleClicked(info)
                }
            }

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: {
                    switch (column) {
                        case 0: return model.projectName || ""
                        case 1: return model.fullName || ""
                        case 2: return model.positionTitle || ""
                        case 3: return model.projectRole || ""
                        case 4: return model.phaseName || ""
                        case 5: return (model.hoursAllocated || 0).toString()
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
