import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    signal itemDoubleClicked(string techInfo)
    
    HorizontalHeaderView {
        id: techHeaderView
        syncView: techTableView
        Layout.fillWidth: true
        model: ["Технология", "Количество проектов"]
    }

    TableView {
        id: techTableView
        resizableColumns: true
        interactive: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [300, 200];
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
                    let info = "Технология: " + (model.techName || "") +
                        "\nИспользуется в проектах: " + (model.projectCount || 0)
                    root.itemDoubleClicked(info)
                }
            }

            Text {
                anchors.centerIn: parent
                text: {
                    switch (column) {
                        case 0: return model.techName || ""
                        case 1: return (model.projectCount || 0).toString()
                        default: return ""
                    }
                }
            }
        }
    }
}
