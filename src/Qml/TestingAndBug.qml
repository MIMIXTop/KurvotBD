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
                text: "Баги"
                width: implicitWidth
            }
            TabButton {
                text: "Релизы"
                width: implicitWidth
            }
            TabButton {
                text: "Эффективность QA"
                width: implicitWidth
            }
        }

        StackLayout {
            currentIndex: bar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Страница 0: Баги
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для багов
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Проект
                        ComboBox {
                            id: bugProjectCombo
                            width: 200
                            height: 32
                            model: bugsModel.projectList
                        }

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
                                    id: statusNew
                                    text: "новая"
                                    checked: false
                                    contentItem: Text {
                                        text: statusNew.text
                                        color: "#333333"
                                        leftPadding: statusNew.indicator.width + statusNew.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: statusInProgress
                                    text: "в_работе"
                                    checked: false
                                    contentItem: Text {
                                        text: statusInProgress.text
                                        color: "#333333"
                                        leftPadding: statusInProgress.indicator.width + statusInProgress.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: statusFixed
                                    text: "исправлена"
                                    checked: false
                                    contentItem: Text {
                                        text: statusFixed.text
                                        color: "#333333"
                                        leftPadding: statusFixed.indicator.width + statusFixed.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: statusVerified
                                    text: "проверена"
                                    checked: false
                                    contentItem: Text {
                                        text: statusVerified.text
                                        color: "#333333"
                                        leftPadding: statusVerified.indicator.width + statusVerified.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }

                        // Серьезность (множественный выбор)
                        Column {
                            spacing: 5
                            Text { 
                                text: "Серьезность:"
                                font.bold: true
                                color: "#333333"
                            }
                            Row {
                                spacing: 5
                                CheckBox { 
                                    id: severityBlocking
                                    text: "блокирующая"
                                    checked: false
                                    contentItem: Text {
                                        text: severityBlocking.text
                                        color: "#333333"
                                        leftPadding: severityBlocking.indicator.width + severityBlocking.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: severityCritical
                                    text: "критическая"
                                    checked: false
                                    contentItem: Text {
                                        text: severityCritical.text
                                        color: "#333333"
                                        leftPadding: severityCritical.indicator.width + severityCritical.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: severityMedium
                                    text: "средняя"
                                    checked: false
                                    contentItem: Text {
                                        text: severityMedium.text
                                        color: "#333333"
                                        leftPadding: severityMedium.indicator.width + severityMedium.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: severityLow
                                    text: "низкая"
                                    checked: false
                                    contentItem: Text {
                                        text: severityLow.text
                                        color: "#333333"
                                        leftPadding: severityLow.indicator.width + severityLow.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }

                        // Дата обнаружения от
                        DatePicker {
                            id: bugFoundFromField
                            selectedDate: ""
                            placeholderText: "Найдена от (ДД.ММ.ГГГГ)"
                        }

                        // Дата обнаружения до
                        DatePicker {
                            id: bugFoundToField
                            selectedDate: ""
                            placeholderText: "Найдена до (ДД.ММ.ГГГГ)"
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
                                if (statusNew.checked) statuses.push("новая")
                                if (statusInProgress.checked) statuses.push("в_работе")
                                if (statusFixed.checked) statuses.push("исправлена")
                                if (statusVerified.checked) statuses.push("проверена")

                                let severities = []
                                if (severityBlocking.checked) severities.push("блокирующая")
                                if (severityCritical.checked) severities.push("критическая")
                                if (severityMedium.checked) severities.push("средняя")
                                if (severityLow.checked) severities.push("низкая")

                                let fromDate = bugFoundFromField.selectedDate.length === 10 ? 
                                    bugFoundFromField.toSqlFormat(bugFoundFromField.selectedDate) : ""
                                let toDate = bugFoundToField.selectedDate.length === 10 ? 
                                    bugFoundToField.toSqlFormat(bugFoundToField.selectedDate) : ""

                                bugsModel.applyFilters(
                                    bugProjectCombo.currentIndex,
                                    statuses,
                                    severities,
                                    fromDate,
                                    toDate
                                )
                            }
                        }
                    }

                    // Таблица багов
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        BugsTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: bugsModel
                            onItemDoubleClicked: function(info) {
                                bugDialog.bugInfo = info
                                bugDialog.open()
                            }
                        }
                    }
                }
            }

            // Страница 1: Релизы
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для релизов
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Дата от
                        DatePicker {
                            id: releaseFromDateField
                            selectedDate: ""
                            placeholderText: "Дата от (ДД.ММ.ГГГГ)"
                        }

                        // Дата до
                        DatePicker {
                            id: releaseToDateField
                            selectedDate: ""
                            placeholderText: "Дата до (ДД.ММ.ГГГГ)"
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
                                let fromDate = releaseFromDateField.selectedDate.length === 10 ? 
                                    releaseFromDateField.toSqlFormat(releaseFromDateField.selectedDate) : ""
                                let toDate = releaseToDateField.selectedDate.length === 10 ? 
                                    releaseToDateField.toSqlFormat(releaseToDateField.selectedDate) : ""
                                
                                releasesModel.loadFiltered(
                                    fromDate,
                                    toDate
                                )
                            }
                        }
                    }

                    // Таблица релизов
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        ReleasesTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: releasesModel
                        }
                    }
                }
            }

            // Страница 2: Эффективность QA
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для эффективности QA
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Проект
                        ComboBox {
                            id: efficiencyProjectCombo
                            width: 200
                            height: 32
                            model: testingEfficiencyModel.projectList
                        }

                        // Период от
                        DatePicker {
                            id: efficiencyFromDateField
                            selectedDate: ""
                            placeholderText: "Период от (ДД.ММ.ГГГГ)"
                        }

                        // Период до
                        DatePicker {
                            id: efficiencyToDateField
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
                                let fromDate = efficiencyFromDateField.selectedDate.length === 10 ? 
                                    efficiencyFromDateField.toSqlFormat(efficiencyFromDateField.selectedDate) : ""
                                let toDate = efficiencyToDateField.selectedDate.length === 10 ? 
                                    efficiencyToDateField.toSqlFormat(efficiencyToDateField.selectedDate) : ""
                                
                                testingEfficiencyModel.loadFiltered(
                                    efficiencyProjectCombo.currentIndex,
                                    fromDate,
                                    toDate
                                )
                            }
                        }
                    }

                    // Таблица эффективности QA
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        EfficiencyTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: testingEfficiencyModel
                        }
                    }
                }
            }
        }
    }

    // Alert Dialog для отображения информации о баге
    BugDialog {
        id: bugDialog
    }
}
