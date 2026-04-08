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
                text: "Проекты"
                width: implicitWidth
            }
            TabButton {
                text: "Заказчики"
                width: implicitWidth
            }
        }

        StackLayout {
            currentIndex: bar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Страница 0: Проекты
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для проектов
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Статусы (множественный выбор через чекбоксы)
                        Column {
                            spacing: 5
                            Text { 
                                text: "Статус:"
                                font.bold: true
                                color: "#333333"
                            }
                            Row {
                                spacing: 5
                                CheckBox { 
                                    id: statusDev
                                    text: "в_разработке"
                                    checked: false
                                    contentItem: Text {
                                        text: statusDev.text
                                        color: "#333333"
                                        leftPadding: statusDev.indicator.width + statusDev.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: statusDone
                                    text: "завершен"
                                    checked: false
                                    contentItem: Text {
                                        text: statusDone.text
                                        color: "#333333"
                                        leftPadding: statusDone.indicator.width + statusDone.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: statusPostponed
                                    text: "отложен"
                                    checked: false
                                    contentItem: Text {
                                        text: statusPostponed.text
                                        color: "#333333"
                                        leftPadding: statusPostponed.indicator.width + statusPostponed.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: statusClosedEarly
                                    text: "закрыт_досрочно"
                                    checked: false
                                    contentItem: Text {
                                        text: statusClosedEarly.text
                                        color: "#333333"
                                        leftPadding: statusClosedEarly.indicator.width + statusClosedEarly.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }

                        // Методология
                        ComboBox {
                            id: methodologyCombo
                            width: 150
                            height: 32
                            model: projectsModel.methodologyList
                        }

                        // Типы клиентов (множественный выбор)
                        Column {
                            spacing: 5
                            Text { 
                                text: "Тип клиента:"
                                font.bold: true
                                color: "#333333"
                            }
                            Row {
                                spacing: 5
                                CheckBox { 
                                    id: clientCorp
                                    text: "корпоративный"
                                    checked: false
                                    contentItem: Text {
                                        text: clientCorp.text
                                        color: "#333333"
                                        leftPadding: clientCorp.indicator.width + clientCorp.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: clientGov
                                    text: "государственный"
                                    checked: false
                                    contentItem: Text {
                                        text: clientGov.text
                                        color: "#333333"
                                        leftPadding: clientGov.indicator.width + clientGov.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: clientIP
                                    text: "ИП"
                                    checked: false
                                    contentItem: Text {
                                        text: clientIP.text
                                        color: "#333333"
                                        leftPadding: clientIP.indicator.width + clientIP.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }

                        // Бюджет от
                        TextField {
                            id: minBudgetField
                            width: 150
                            height: 32
                            placeholderText: "Бюджет от"
                            validator: DoubleValidator { bottom: 0 }
                        }

                        // Бюджет до
                        TextField {
                            id: maxBudgetField
                            width: 150
                            height: 32
                            placeholderText: "Бюджет до"
                            validator: DoubleValidator { bottom: 0 }
                        }

                        // Даты завершения от
                        TextField {
                            id: completedFromField
                            width: 180
                            height: 32
                            placeholderText: "Завершен от (ГГГГ-ММ-ДД)"
                        }

                        // Даты завершения до
                        TextField {
                            id: completedToField
                            width: 180
                            height: 32
                            placeholderText: "Завершен до (ГГГГ-ММ-ДД)"
                        }

                        // Активность
                        CheckBox {
                            id: projectActiveCheck
                            height: 32
                            text: "Только активные"
                            checked: true
                            contentItem: Text {
                                text: projectActiveCheck.text
                                color: "#333333"
                                leftPadding: projectActiveCheck.indicator.width + projectActiveCheck.spacing
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
                                let statuses = []
                                if (statusDev.checked) statuses.push("в_разработке")
                                if (statusDone.checked) statuses.push("завершен")
                                if (statusPostponed.checked) statuses.push("отложен")
                                if (statusClosedEarly.checked) statuses.push("закрыт_досрочно")

                                let clientTypes = []
                                if (clientCorp.checked) clientTypes.push("корпоративный")
                                if (clientGov.checked) clientTypes.push("государственный")
                                if (clientIP.checked) clientTypes.push("ИП")

                                let minBudget = minBudgetField.text.length > 0 ? parseFloat(minBudgetField.text) : -1
                                let maxBudget = maxBudgetField.text.length > 0 ? parseFloat(maxBudgetField.text) : -1

                                projectsModel.applyFilters(
                                    statuses,
                                    completedFromField.text,
                                    completedToField.text,
                                    clientTypes,
                                    methodologyCombo.currentIndex,
                                    minBudget,
                                    maxBudget,
                                    projectActiveCheck.checked
                                )
                            }
                        }
                    }

                    // Таблица проектов
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

                                model: projectsModel

                                delegate: Rectangle {
                                    implicitHeight: 50
                                    border.width: 1
                                    color: row % 2 === 0 ? "white" : "#f9f9f9"

                                    MouseArea {
                                        anchors.fill: parent
                                        onDoubleClicked: {
                                            // Показать Alert с информацией о проекте
                                            let projectId = projectsModel.data(projectsModel.index(row, 0), 257) // ProjectId role
                                            let projectName = model.projectName || ""
                                            let clientName = model.clientName || ""
                                            let budget = model.budget || 0
                                            
                                            alertDialog.projectInfo = "ID: " + projectId + "\nПроект: " + projectName + "\nКлиент: " + clientName + "\nБюджет: " + Number(budget).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                                            alertDialog.open()
                                        }
                                    }

                                    Text {
                                        anchors.centerIn: parent
                                        text: {
                                            switch (column) {
                                                case 0: return model.projectName || ""
                                                case 1: return model.clientName || ""
                                                case 2: return model.clientType || ""
                                                case 3: return model.status || ""
                                                case 4: return model.methodology || ""
                                                case 5: return Number(model.budget).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                                                case 6: return model.type || ""
                                                case 7: return model.startDate || ""
                                                case 8: return model.plannedEndDate || ""
                                                case 9: return model.actualEndDate || ""
                                                default: return ""
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Страница 1: Заказчики
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для заказчиков
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Период от
                        TextField {
                            id: clientFromDateField
                            width: 180
                            height: 32
                            placeholderText: "Период от (ГГГГ-ММ-ДД)"
                        }

                        // Период до
                        TextField {
                            id: clientToDateField
                            width: 180
                            height: 32
                            placeholderText: "Период до (ГГГГ-ММ-ДД)"
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
                                clientsModel.loadFiltered(
                                    clientFromDateField.text,
                                    clientToDateField.text
                                )
                            }
                        }
                    }

                    // Таблица заказчиков
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

                                model: clientsModel

                                delegate: Rectangle {
                                    implicitHeight: 50
                                    border.width: 1
                                    color: row % 2 === 0 ? "white" : "#f9f9f9"

                                    Text {
                                        anchors.centerIn: parent
                                        text: {
                                            switch (column) {
                                                case 0: return model.clientName || ""
                                                case 1: return model.activeProjects || 0
                                                case 2: return Number(model.totalSpend).toLocaleString('ru-RU', {minimumFractionDigits: 2, maximumFractionDigits: 2})
                                                case 3: return model.earliestProjectDate || ""
                                                case 4: return model.latestProjectDate || ""
                                                default: return ""
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Alert Dialog для отображения информации о проекте
    Dialog {
        id: alertDialog
        title: "Информация о проекте"
        modal: true
        anchors.centerIn: parent
        width: 400
        height: 200

        property string projectInfo: ""

        contentItem: Rectangle {
            color: "white"
            Text {
                anchors.centerIn: parent
                text: alertDialog.projectInfo
                font.pixelSize: 14
                wrapMode: Text.WordWrap
            }
        }

        standardButtons: Dialog.Ok
    }
}
