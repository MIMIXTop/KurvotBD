import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    signal itemDoubleClicked(string monthlyInfo)
    
    HorizontalHeaderView {
        id: monthlyHeaderView
        syncView: monthlyTableView
        Layout.fillWidth: true
        model: ["Проект", "ЗП", "Лицензии", "Облако", "Итого"]
    }

    TableView {
        id: monthlyTableView
        resizableColumns: true
        interactive: false
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [250, 150, 150, 150, 150];
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
                    let expensesSalary = model.expensesSalary || 0
                    let expensesLicenses = model.expensesLicenses || 0
                    let expensesCloud = model.expensesCloud || 0
                    let totalMonthlyCost = model.totalMonthlyCost || 0
                    
                    let info = "Проект: " + projectName + 
                        "\n\nРасходы на ЗП: " + Number(expensesSalary).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                        "\nРасходы на лицензии: " + Number(expensesLicenses).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                        "\nРасходы на облако: " + Number(expensesCloud).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                        "\n\nИтого за месяц: " + Number(totalMonthlyCost).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                    root.itemDoubleClicked(info)
                }
            }

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: {
                    switch (column) {
                        case 0: return model.projectName || ""
                        case 1: return Number(model.expensesSalary || 0).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                        case 2: return Number(model.expensesLicenses || 0).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                        case 3: return Number(model.expensesCloud || 0).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                        case 4: return Number(model.totalMonthlyCost || 0).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
