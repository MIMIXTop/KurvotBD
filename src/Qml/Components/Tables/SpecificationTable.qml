import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    signal itemDoubleClicked(string specInfo, string specContent)
    
    HorizontalHeaderView {
        id: specHeaderView
        syncView: specTableView
        Layout.fillWidth: true
        model: ["Проект", "Версия", "Автор", "Дата создания", "Последнее обновление"]
    }

    TableView {
        id: specTableView
        resizableColumns: true
        interactive: false
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [250, 120, 200, 150, 150];
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
                    let info = "ID: " + (model.specId || 0) +
                        "\nПроект: " + (model.projectName || "") +
                        "\nВерсия: " + (model.version || "") +
                        "\nАвтор: " + (model.authorName || "") +
                        "\nДата создания: " + (model.creationDate || "") +
                        "\nПоследнее обновление: " + (model.lastUpdate || "")
                    let content = model.documentText || ""
                    root.itemDoubleClicked(info, content)
                }
            }

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: {
                    switch (column) {
                        case 0: return model.projectName || ""
                        case 1: return model.version || ""
                        case 2: return model.authorName || ""
                        case 3: return model.creationDate || ""
                        case 4: return model.lastUpdate || ""
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
