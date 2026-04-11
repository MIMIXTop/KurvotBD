import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Components/Tables"
import "Components/Dialogs"

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
                        DatePicker {
                            id: completedFromField
                            selectedDate: ""
                            placeholderText: "Завершен от (ДД.ММ.ГГГГ)"
                        }

                        // Даты завершения до
                        DatePicker {
                            id: completedToField
                            selectedDate: ""
                            placeholderText: "Завершен до (ДД.ММ.ГГГГ)"
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

                                let fromDate = completedFromField.selectedDate.length === 10 ? 
                                    completedFromField.toSqlFormat(completedFromField.selectedDate) : ""
                                let toDate = completedToField.selectedDate.length === 10 ? 
                                    completedToField.toSqlFormat(completedToField.selectedDate) : ""

                                projectsModel.applyFilters(
                                    statuses,
                                    fromDate,
                                    toDate,
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

                        ProjectsTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: projectsModel
                            onItemDoubleClicked: function(info) {
                                alertDialog.projectInfo = info
                                alertDialog.open()
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
                        DatePicker {
                            id: clientFromDateField
                            selectedDate: ""
                            placeholderText: "Период от (ДД.ММ.ГГГГ)"
                        }

                        // Период до
                        DatePicker {
                            id: clientToDateField
                            selectedDate: ""
                            placeholderText: "Период до (ДД.ММ.ГГГГ)"
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
                                let fromDate = clientFromDateField.selectedDate.length === 10 ? 
                                    clientFromDateField.toSqlFormat(clientFromDateField.selectedDate) : ""
                                let toDate = clientToDateField.selectedDate.length === 10 ? 
                                    clientToDateField.toSqlFormat(clientToDateField.selectedDate) : ""
                                
                                clientsModel.loadFiltered(
                                    fromDate,
                                    toDate
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

                        ClientsTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: clientsModel
                        }
                    }
                }
            }
        }
    }

    // Alert Dialog для отображения информации о проекте
    ProjectDialog {
        id: alertDialog
    }
}
