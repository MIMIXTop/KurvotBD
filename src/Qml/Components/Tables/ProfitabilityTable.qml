import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    signal itemDoubleClicked(string profitInfo)
    
    HorizontalHeaderView {
        id: profitHeaderView
        syncView: profitTableView
        Layout.fillWidth: true
        model: ["Проект", "Затраты", "Бюджет", "Коэффициент эффективности"]
    }

    TableView {
        id: profitTableView
        resizableColumns: true
        interactive: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [250, 150, 150, 200];
            return minWidths[column] || 100;
        }

        model: root.tableModel

        delegate: Rectangle {
            implicitHeight: 50
            border.width: 1
            
            // Цветовая индикация
            color: {
                let ratio = model.efficiencyRatio || 0
                if (ratio > 0) return "#e8f5e9"  // зелёный - прибыльно
                if (ratio < 0) return "#ffebee"  // красный - убыток
                return row % 2 === 0 ? "white" : "#f9f9f9"
            }

            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    let projectName = model.projectName || ""
                    let totalCosts = model.totalCosts || 0
                    let budget = model.budget || 0
                    let efficiencyRatio = model.efficiencyRatio || 0
                    
                    let info = "Проект: " + projectName +
                        "\nЗатраты: " + Number(totalCosts).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                        "\nБюджет: " + Number(budget).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                        "\nКоэффициент эффективности: " + Number(efficiencyRatio).toFixed(2) + 
                        "\n\nСтатус: " + (efficiencyRatio > 0 ? "Прибыльный проект" : efficiencyRatio < 0 ? "Убыточный проект" : "Нулевая рентабельность")
                    root.itemDoubleClicked(info)
                }
            }

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: {
                    switch (column) {
                        case 0: return model.projectName || ""
                        case 1: return Number(model.totalCosts || 0).toLocaleString('ru-RU', 'f', 2)
                        case 2: return Number(model.budget || 0).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                        case 3: return Number(model.efficiencyRatio || 0).toFixed(2)
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
