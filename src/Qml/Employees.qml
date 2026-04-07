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
                text: "Сотрудники"
                width: implicitWidth
            }
            TabButton {
                text: "Кадры"
                width: implicitWidth
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.topMargin: 8
            Layout.bottomMargin: 8
            spacing: 6

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                ComboBox {
                    id: deptCombo
                    Layout.preferredWidth: 250
                    model: [
                        "Все отделы",
                        "Отдел разработки веб-приложений",
                        "Отдел мобильной разработки",
                        "Отдел тестирования ПО",
                        "Отдел технической поддержки",
                        "Отдел управления проектами",
                        "Административный отдел",
                        "Отдел системной архитектуры"
                    ]
                }

                TextField {
                    id: positionField
                    Layout.preferredWidth: 200
                    placeholderText: "Должность"
                }

                TextField {
                    id: minExpField
                    Layout.preferredWidth: 100
                    placeholderText: "Стаж от"
                    validator: IntValidator { bottom: 0 }
                }

                TextField {
                    id: maxExpField
                    Layout.preferredWidth: 100
                    placeholderText: "Стаж до"
                    validator: IntValidator { bottom: 0 }
                }

                Item { Layout.fillWidth: true }

                Button {
                    text: "Найти"
                    onClicked: {
                        let minExp = minExpField.text.length > 0 ? parseInt(minExpField.text) : -1
                        let maxExp = maxExpField.text.length > 0 ? parseInt(maxExpField.text) : -1
                        let minAge = minAgeField.text.length > 0 ? parseInt(minAgeField.text) : -1
                        let maxAge = maxAgeField.text.length > 0 ? parseInt(maxAgeField.text) : -1
                        let minSal = minSalaryField.text.length > 0 ? parseFloat(minSalaryField.text) : -1
                        let maxSal = maxSalaryField.text.length > 0 ? parseFloat(maxSalaryField.text) : -1
                        employeesTableModel.applyFilters(
                            deptCombo.currentIndex, positionField.text,
                            minExp, maxExp,
                            minAge, maxAge,
                            minSal, maxSal,
                            activeCheck.checked
                        )
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                TextField {
                    id: minAgeField
                    Layout.preferredWidth: 100
                    placeholderText: "Возраст от"
                    validator: IntValidator { bottom: 0; top: 120 }
                }

                TextField {
                    id: maxAgeField
                    Layout.preferredWidth: 100
                    placeholderText: "Возраст до"
                    validator: IntValidator { bottom: 0; top: 120 }
                }

                TextField {
                    id: minSalaryField
                    Layout.preferredWidth: 120
                    placeholderText: "Зарплата от"
                    validator: DoubleValidator { bottom: 0 }
                }

                TextField {
                    id: maxSalaryField
                    Layout.preferredWidth: 120
                    placeholderText: "Зарплата до"
                    validator: DoubleValidator { bottom: 0 }
                }

                CheckBox {
                    id: activeCheck
                    text: "Активные"
                    checked: true
                }

                Item { Layout.fillWidth: true }
            }
        }

        Rectangle {
            id: tiel
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "white"
            radius: 15
            border{
                width: 10
                color: "white"
            }

            StackLayout {
                anchors.fill: parent
                currentIndex: bar.currentIndex

                Item {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        HorizontalHeaderView {
                            id: horizontalHeader
                            syncView: tableView
                            Layout.fillWidth: true

                            model: ["Имя", "Отдел", "Должность", "Возраст", "Зарплата", "Опыт", "Email", "Телефон"]
                        }

                        TableView {
                            id: tableView
                            resizableColumns: true
                            interactive: true
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            columnSpacing: 1
                            rowSpacing: 1
                            clip: true

                            columnWidthProvider: function (column) {
                                let minWidths = [300, 230, 180, 50, 150, 80, 180, 120];
                                return minWidths[column] || 100;
                            }

                            model: employeesTableModel

                            delegate: Rectangle {
                                id: tile
                                implicitHeight: 50
                                border.width: 1

                                Text {
                                    anchors.centerIn: parent

                                    text: {
                                        if (column === 0) return model.employeeName
                                        if (column === 1) return model.departmentName
                                        if (column === 2) return model.position
                                        if (column === 3) return model.age
                                        if (column === 4) {
                                            return Number(model.salary).toLocaleString(Qt.locale("ru_RU"), 'f', 0)
                                        }
                                        if (column === 5) return model.experienceYears
                                        if (column === 6) return model.email
                                        if (column === 7) return model.phone
                                        return ""
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