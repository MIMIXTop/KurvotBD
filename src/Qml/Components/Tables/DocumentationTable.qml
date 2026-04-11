import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    signal itemDoubleClicked(string docInfo, string docContent)
    
    HorizontalHeaderView {
        id: docHeaderView
        syncView: docTableView
        Layout.fillWidth: true
        model: ["Проект", "Тип", "Версия/Дата", "Автор", "Последнее обновление", "Путь"]
    }

    TableView {
        id: docTableView
        resizableColumns: true
        interactive: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [200, 150, 120, 150, 120, 200];
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
                        "\nТип: " + (model.docType || "") +
                        "\nАвтор: " + (model.authorName || "") +
                        "\nДата создания: " + (model.creationDate || "") +
                        "\nПоследнее обновление: " + (model.lastUpdate || "") +
                        "\nПуть: " + (model.storagePath || "")
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
                        case 1: return model.docType || ""
                        case 2: return model.creationDate || ""
                        case 3: return model.authorName || ""
                        case 4: return model.lastUpdate || ""
                        case 5: return model.storagePath || ""
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
