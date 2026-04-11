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
                        DatePicker {
                            id: profitFromDateField
                            selectedDate: "01.01.2024"
                            placeholderText: "Дата от (ДД.ММ.ГГГГ)"
                        }

                        // Дата до
                        DatePicker {
                            id: profitToDateField
                            selectedDate: "31.12.2026"
                            placeholderText: "Дата до (ДД.ММ.ГГГГ)"
                        }

                        // Дата до (старое поле - удалить)
                        TextField {
                            id: profitToDateFieldOld
                            width: 180
                            height: 32
                            placeholderText: "Дата до (ГГГГ-ММ-ДД)"
                            visible: false
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
                                let fromDate = profitFromDateField.selectedDate.length === 10 ? 
                                    profitFromDateField.toSqlFormat(profitFromDateField.selectedDate) : ""
                                let toDate = profitToDateField.selectedDate.length === 10 ? 
                                    profitToDateField.toSqlFormat(profitToDateField.selectedDate) : ""
                                
                                profitabilityModel.applyFilters(
                                    fromDate,
                                    toDate,
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

                        ProfitabilityTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: profitabilityModel
                            onItemDoubleClicked: function(info) {
                                profitDialog.profitInfo = info
                                profitDialog.open()
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

                        InfrastructureTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: infrastructureModel
                            onItemDoubleClicked: function(info) {
                                infraDialog.infraInfo = info
                                infraDialog.open()
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

                        MonthlyFinancialTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: monthlyFinancialModel
                            onItemDoubleClicked: function(info) {
                                monthlyDialog.monthlyInfo = info
                                monthlyDialog.open()
                            }
                        }
                    }
                }
            }
        }
    }

    // Dialog для рентабельности
    ProfitDialog {
        id: profitDialog
    }

    // Dialog для инфраструктуры
    InfraDialog {
        id: infraDialog
    }

    // Dialog для ежемесячного отчёта
    MonthlyDialog {
        id: monthlyDialog
    }
}
