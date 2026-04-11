import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    signal itemDoubleClicked(string infraInfo)
    
    HorizontalHeaderView {
        id: infraHeaderView
        syncView: infraTableView
        Layout.fillWidth: true
        model: ["Проект", "Лицензия", "Срок действия", "Стоимость", "Провайдер", "Стоимость/час"]
    }

    TableView {
        id: infraTableView
        resizableColumns: true
        interactive: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [200, 200, 120, 120, 120, 120];
            return minWidths[column] || 100;
        }

        model: root.tableModel

        delegate: Rectangle {
            implicitHeight: 50
            border.width: 1
            
            // Цветовая индикация для истекающих лицензий
            color: {
                let expiry = model.expiryDate || ""
                if (expiry !== "") {
                    let expiryDate = new Date(expiry)
                    let today = new Date()
                    let daysUntilExpiry = (expiryDate - today) / (1000 * 60 * 60 * 24)
                    
                    if (daysUntilExpiry < 0) {
                        return "#ffcdd2"  // 🔴 Красный - истекла
                    }
                    if (daysUntilExpiry >= 0 && daysUntilExpiry <= 30) {
                        return "#fff9c4"  // 🟡 Желтый - осталось ~месяц
                    }
                    if (daysUntilExpiry > 30) {
                        return "#c8e6c9"  // 🟢 Зеленый - действительна
                    }
                }
                return row % 2 === 0 ? "white" : "#f9f9f9"
            }

            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    let projectId = model.projectId || 0
                    let projectName = model.projectName || ""
                    let licenseName = model.licenseName || ""
                    let expiryDate = model.expiryDate || ""
                    let cost = model.cost || 0
                    let resourceProvider = model.resourceProvider || ""
                    let resourceCostPerHour = model.resourceCostPerHour || 0
                    
                    let info = "ID проекта: " + projectId + 
                        "\nПроект: " + projectName + 
                        "\nЛицензия: " + licenseName + 
                        "\nСрок действия: " + expiryDate + 
                        "\nСтоимость: " + Number(cost).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                        "\nПровайдер: " + resourceProvider + 
                        "\nСтоимость/час: " + Number(resourceCostPerHour).toFixed(4)
                    root.itemDoubleClicked(info)
                }
            }

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: {
                    switch (column) {
                        case 0: return model.projectName || ""
                        case 1: return model.licenseName || ""
                        case 2: return model.expiryDate || ""
                        case 3: return Number(model.cost || 0).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                        case 4: return model.resourceProvider || ""
                        case 5: return Number(model.resourceCostPerHour || 0).toFixed(4)
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
