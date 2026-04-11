import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    
    HorizontalHeaderView {
        id: efficiencyHeaderView
        syncView: efficiencyTableView
        Layout.fillWidth: true
        model: ["Проект", "Баги в тесте", "Баги после релиза", "Коэффициент эффективности"]
    }

    TableView {
        id: efficiencyTableView
        resizableColumns: true
        interactive: false
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [250, 150, 180, 220];
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
                        case 0: return model.projectName || ""
                        case 1: return model.bugsInTest || 0
                        case 2: return model.bugsAfterRelease || 0
                        case 3: return Number(model.efficiencyRatio || 0).toFixed(2)
                        default: return ""
                    }
                }
            }
        }
    }
}
