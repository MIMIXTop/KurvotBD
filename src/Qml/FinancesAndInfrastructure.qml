import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    Layout.fillWidth: true
    Layout.fillHeight: true

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            id: bar
            width: parent.width
            Layout.fillWidth: true
            TabButton {
                text: "Рентабельность"
                width: implicitWidth
            }
            TabButton {
                text: "Лицензии и Серверы"
                width: implicitWidth
            }
            TabButton {
                text: "Ежемесячный отчёт"
                width: implicitWidth
            }
        }

        StackLayout {
            currentIndex: bar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Страница 0: Рентабельность
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для рентабельности
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Дата от
                        TextField {
                            id: profitFromDateField
                            width: 180
                            height: 32
                            placeholderText: "Дата от (ГГГГ-ММ-ДД)"
                        }

                        // Дата до
                        TextField {
                            id: profitToDateField
                            width: 180
                            height: 32
                            placeholderText: "Дата до (ГГГГ-ММ-ДД)"
                        }

                        // Только активные проекты
                        CheckBox {
                            id: profitActiveCheck
                            text: "Только активные проекты"
                            checked: false
                            contentItem: Text {
                                text: profitActiveCheck.text
                                color: "#333333"
                                leftPadding: profitActiveCheck.indicator.width + profitActiveCheck.spacing
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        // Кнопка поиска
                        Button {
                            text: "Найти"
                            height: 32
                            background: Rectangle {
                                color: "#2196F3"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                profitabilityModel.applyFilters(
                                    profitFromDateField.text,
                                    profitToDateField.text,
                                    profitActiveCheck.checked
                                )
                            }
                        }
                    }

                    // Таблица рентабельности
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 0

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

                                model: profitabilityModel

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
                                            let projectId = model.projectId || 0
                                            let projectName = model.projectName || ""
                                            let totalCosts = model.totalCosts || 0
                                            let budget = model.budget || 0
                                            let efficiencyRatio = model.efficiencyRatio || 0
                                            
                                            profitDialog.profitInfo =
                                                "Проект: " + projectName +
                                                "\nЗатраты: " + Number(totalCosts).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                                                "\nБюджет: " + Number(budget).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                                                "\nКоэффициент эффективности: " + Number(efficiencyRatio).toFixed(2) + 
                                                "\n\nСтатус: " + (efficiencyRatio > 0 ? "Прибыльный проект" : efficiencyRatio < 0 ? "Убыточный проект" : "Нулевая рентабельность")
                                            profitDialog.open()
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
                    }
                }
            }

            // Страница 1: Лицензии и Серверы
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для инфраструктуры
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Только активные проекты
                        CheckBox {
                            id: infraActiveCheck
                            text: "Только активные проекты"
                            checked: false
                            contentItem: Text {
                                text: infraActiveCheck.text
                                color: "#333333"
                                leftPadding: infraActiveCheck.indicator.width + infraActiveCheck.spacing
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        // Проект
                        ComboBox {
                            id: infraProjectCombo
                            width: 200
                            height: 32
                            model: infrastructureModel.projectList
                        }

                        // Кнопка поиска
                        Button {
                            text: "Найти"
                            height: 32
                            background: Rectangle {
                                color: "#2196F3"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                infrastructureModel.applyFilters(
                                    infraActiveCheck.checked,
                                    infraProjectCombo.currentIndex
                                )
                            }
                        }
                    }

                    // Таблица инфраструктуры
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 0

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

                                model: infrastructureModel

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
                                            if (daysUntilExpiry < 30 && daysUntilExpiry >= 0) {
                                                return "#ffebee"  // красный - истекает скоро
                                            }
                                            if (daysUntilExpiry < 0) {
                                                return "#ffcdd2"  // тёмно-красный - уже истекла
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
                                            
                                            infraDialog.infraInfo = "ID проекта: " + projectId + 
                                                "\nПроект: " + projectName + 
                                                "\nЛицензия: " + licenseName + 
                                                "\nСрок действия: " + expiryDate + 
                                                "\nСтоимость: " + Number(cost).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                                                "\nПровайдер: " + resourceProvider + 
                                                "\nСтоимость/час: " + Number(resourceCostPerHour).toFixed(4)
                                            infraDialog.open()
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
                    }
                }
            }

            // Страница 2: Ежемесячный отчёт
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для ежемесячного отчёта
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Месяц
                        ComboBox {
                            id: monthCombo
                            width: 150
                            height: 32
                            model: ["Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"]
                            currentIndex: 3  // Апрель (текущий месяц)
                        }

                        // Год
                        TextField {
                            id: yearField
                            width: 100
                            height: 32
                            text: "2026"
                            placeholderText: "ГГГГ"
                        }

                        // Кнопка показать отчёт
                        Button {
                            text: "Показать отчёт"
                            height: 32
                            background: Rectangle {
                                color: "#2196F3"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                let month = monthCombo.currentIndex + 1
                                let year = parseInt(yearField.text)
                                if (!isNaN(year) && year > 1900 && year < 2100) {
                                    monthlyFinancialModel.loadReport(month, year)
                                }
                            }
                        }
                    }

                    // Таблица ежемесячного отчёта
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 0

                            HorizontalHeaderView {
                                id: monthlyHeaderView
                                syncView: monthlyTableView
                                Layout.fillWidth: true
                                model: ["Проект", "ЗП", "Лицензии", "Облако", "Итого"]
                            }

                            TableView {
                                id: monthlyTableView
                                resizableColumns: true
                                interactive: true
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                columnSpacing: 1
                                rowSpacing: 1
                                clip: true

                                columnWidthProvider: function (column) {
                                    let minWidths = [250, 150, 150, 150, 150];
                                    return minWidths[column] || 100;
                                }

                                model: monthlyFinancialModel

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
                                            
                                            monthlyDialog.monthlyInfo = "Проект: " + projectName + 
                                                "\n\nРасходы на ЗП: " + Number(expensesSalary).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                                                "\nРасходы на лицензии: " + Number(expensesLicenses).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                                                "\nРасходы на облако: " + Number(expensesCloud).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2}) + 
                                                "\n\nИтого за месяц: " + Number(totalMonthlyCost).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                                            monthlyDialog.open()
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
                    }
                }
            }
        }
    }

    // Dialog для рентабельности
    Dialog {
        id: profitDialog
        title: "Информация о рентабельности"
        modal: true
        anchors.centerIn: parent
        width: 500
        height: 300

        property string profitInfo: ""

        contentItem: Rectangle {
            color: "white"
            Text {
                anchors.fill: parent
                anchors.margins: 20
                text: profitDialog.profitInfo
                font.pixelSize: 14
                wrapMode: Text.WordWrap
            }
        }

        standardButtons: Dialog.Ok
    }

    // Dialog для инфраструктуры
    Dialog {
        id: infraDialog
        title: "Информация об инфраструктуре"
        modal: true
        anchors.centerIn: parent
        width: 500
        height: 350

        property string infraInfo: ""

        contentItem: Rectangle {
            color: "white"
            Text {
                anchors.fill: parent
                anchors.margins: 20
                text: infraDialog.infraInfo
                font.pixelSize: 14
                wrapMode: Text.WordWrap
            }
        }

        standardButtons: Dialog.Ok
    }

    // Dialog для ежемесячного отчёта
    Dialog {
        id: monthlyDialog
        title: "Детали ежемесячного отчёта"
        modal: true
        anchors.centerIn: parent
        width: 500
        height: 300

        property string monthlyInfo: ""

        contentItem: Rectangle {
            color: "white"
            Text {
                anchors.fill: parent
                anchors.margins: 20
                text: monthlyDialog.monthlyInfo
                font.pixelSize: 14
                wrapMode: Text.WordWrap
            }
        }

        standardButtons: Dialog.Ok
    }
}
