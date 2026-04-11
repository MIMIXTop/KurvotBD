import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    
    HorizontalHeaderView {
        id: clientsHeaderView
        syncView: clientsTableView
        Layout.fillWidth: true
        model: ["Клиент", "Активные проекты", "Общие затраты", "Первый проект", "Последний проект"]
    }

    TableView {
        id: clientsTableView
        resizableColumns: true
        interactive: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [300, 150, 200, 150, 150];
            return minWidths[column] || 100;
        }

        model: root.tableModel

        delegate: Rectangle {
            implicitHeight: 50
            border.width: 1
            color: row % 2 === 0 ? "white" : "#f9f9f9"

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: {
                    switch (column) {
                        case 0: return model.clientName || ""
                        case 1: return model.activeProjects || 0
                        case 2: return Number(model.totalSpend || 0).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                        case 3: return model.earliestProjectDate || ""
                        case 4: return model.latestProjectDate || ""
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
