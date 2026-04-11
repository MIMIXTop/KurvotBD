import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    
    HorizontalHeaderView {
        id: releasesHeaderView
        syncView: releasesTableView
        Layout.fillWidth: true
        model: ["Проект", "Версия", "Дата релиза", "Changelog"]
    }

    TableView {
        id: releasesTableView
        resizableColumns: true
        interactive: false
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [200, 120, 150, 400];
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
                        case 0: return model.projectName || ""
                        case 1: return model.version || ""
                        case 2: return model.releaseDate || ""
                        case 3: return model.changelog || ""
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
