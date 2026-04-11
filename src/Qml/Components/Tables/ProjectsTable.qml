import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    signal itemDoubleClicked(string projectInfo)
    
    HorizontalHeaderView {
        id: projectsHeaderView
        syncView: projectsTableView
        Layout.fillWidth: true
        model: ["Проект", "Клиент", "Тип клиента", "Статус", "Методология", "Бюджет", "Тип проекта", "Дата начала", "План. окончание", "Факт. окончание"]
    }

    TableView {
        id: projectsTableView
        resizableColumns: true
        interactive: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [250, 200, 150, 120, 120, 150, 150, 120, 150, 150];
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
                    let projectName = model.projectName || ""
                    let clientName = model.clientName || ""
                    let budget = model.budget || 0
                    
                    let info = "Проект: " + projectName + 
                        "\nКлиент: " + clientName + 
                        "\nБюджет: " + Number(budget).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                    root.itemDoubleClicked(info)
                }
            }

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: {
                    switch (column) {
                        case 0: return model.projectName || ""
                        case 1: return model.clientName || ""
                        case 2: return model.clientType || ""
                        case 3: return model.status || ""
                        case 4: return model.methodology || ""
                        case 5: return Number(model.budget || 0).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                        case 6: return model.projectType || ""
                        case 7: return model.startDate || ""
                        case 8: return model.plannedEndDate || ""
                        case 9: return model.actualEndDate || ""
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
