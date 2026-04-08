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

        StackLayout {
            currentIndex: bar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Страница 0: Сотрудники
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    Flow {
                        id: filterContainer
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10

                        Layout.preferredHeight: implicitHeight

                        ComboBox {
                            id: deptCombo
                            width: 250
                            height: 32
                            model: employeesTableModel.departmentList
                        }

                        ComboBox {
                            id: positionField
                            width: 200
                            height: 32
                            model: employeesTableModel.positionList
                        }

                        TextField {
                            id: minExpField
                            width: 100
                            height: 32
                            placeholderText: "Стаж от"
                            validator: IntValidator {
                                bottom: 0
                            }
                        }

                        TextField {
                            id: maxExpField
                            width: 100
                            height: 32
                            placeholderText: "Стаж до"
                            validator: IntValidator {
                                bottom: 0
                            }
                        }

                        TextField {
                            id: minAgeField
                            width: 100
                            height: 32
                            placeholderText: "Возраст от"
                            validator: IntValidator {
                                bottom: 0; top: 120
                            }
                        }

                        TextField {
                            id: maxAgeField
                            width: 100
                            height: 32
                            placeholderText: "Возраст до"
                            validator: IntValidator {
                                bottom: 0; top: 120
                            }
                        }

                        TextField {
                            id: minSalaryField
                            width: 120
                            height: 32
                            placeholderText: "Зарплата от"
                            validator: DoubleValidator {
                                bottom: 0
                            }
                        }

                        TextField {
                            id: maxSalaryField
                            width: 120
                            height: 32
                            placeholderText: "Зарплата до"
                            validator: DoubleValidator {
                                bottom: 0
                            }
                        }

                        CheckBox {
                            id: activeCheck
                            height: 32
                            text: "Активные"
                            checked: true
                        }

                        Button {
                            text: "Найти"
                            height: 32
                            onClicked: {
                                let minExp = minExpField.text.length > 0 ? parseInt(minExpField.text) : -1
                                let maxExp = maxExpField.text.length > 0 ? parseInt(maxExpField.text) : -1
                                let minAge = minAgeField.text.length > 0 ? parseInt(minAgeField.text) : -1
                                let maxAge = maxAgeField.text.length > 0 ? parseInt(maxAgeField.text) : -1
                                let minSal = minSalaryField.text.length > 0 ? parseFloat(minSalaryField.text) : -1
                                let maxSal = maxSalaryField.text.length > 0 ? parseFloat(maxSalaryField.text) : -1
                                employeesTableModel.applyFilters(
                                    deptCombo.currentIndex,
                                    positionField.currentIndex,
                                    minExp, maxExp,
                                    minAge, maxAge,
                                    minSal, maxSal,
                                    activeCheck.checked
                                )
                            }
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "white"
                        radius: 5
                        border {
                            width: 10
                            color: "white"
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 1
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
                                            switch (column) {
                                                case 0:
                                                    return model.employeeName
                                                case 1:
                                                    return model.departmentName
                                                case 2:
                                                    return model.position
                                                case 3:
                                                    return model.age
                                                case 4:
                                                    return Number(model.salary).toLocaleString(Qt.locale("ru_RU"), 'f', 0)
                                                case 5:
                                                    return model.experienceYears
                                                case 6:
                                                    return model.email
                                                case 7:
                                                    return model.phone
                                                default:
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

            // Страница 1: Кадры (Загрузка сотрудников)
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для загрузки
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Выбор сотрудника (множественный выбор)
                        ComboBox {
                            id: workloadEmployeeCombo
                            width: 250
                            height: 32
                            model: workloadModel.employeeList
                            editable: true
                        }

                        // Выбор проекта
                        ComboBox {
                            id: workloadProjectCombo
                            width: 250
                            height: 32
                            model: workloadModel.projectList
                        }

                        // Период от
                        TextField {
                            id: workloadFromDateField
                            width: 150
                            height: 32
                            placeholderText: "Период от (ГГГГ-ММ-ДД)"
                        }

                        // Период до
                        TextField {
                            id: workloadToDateField
                            width: 150
                            height: 32
                            placeholderText: "Период до (ГГГГ-ММ-ДД)"
                        }

                        // Кнопка поиска
                        Button {
                            text: "Найти"
                            height: 32
                            onClicked: {
                                let employeeIds = []
                                let selectedEmployee = workloadEmployeeCombo.currentIndex
                                if (selectedEmployee > 0) {
                                    employeeIds = [selectedEmployee - 1]
                                }

                                let projectIds = []
                                let selectedProject = workloadProjectCombo.currentIndex
                                if (selectedProject > 0) {
                                    projectIds = [selectedProject - 1]
                                }

                                let fromDate = workloadFromDateField.text
                                let toDate = workloadToDateField.text

                                workloadModel.loadFiltered(
                                    employeeIds,
                                    projectIds,
                                    fromDate,
                                    toDate
                                )
                            }
                        }
                    }

                    // Таблица загрузки
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
                                id: workloadHeaderView
                                syncView: workloadTableView
                                Layout.fillWidth: true
                                model: ["Сотрудник", "Проект", "Часов", "Задач"]
                            }

                            TableView {
                                id: workloadTableView
                                resizableColumns: true
                                interactive: true
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                columnSpacing: 1
                                rowSpacing: 1
                                clip: true

                                columnWidthProvider: function (column) {
                                    let minWidths = [250, 200, 100, 80];
                                    return minWidths[column] || 100;
                                }

                                model: workloadModel

                                delegate: Rectangle {
                                    implicitHeight: 50
                                    border.width: 1
                                    color: row % 2 === 0 ? "white" : "#f9f9f9"

                                    Text {
                                        anchors.centerIn: parent
                                        text: {
                                            switch (column) {
                                                case 0: return model.employeeName || ""
                                                case 1: return model.projectName || ""
                                                case 2: return Number(model.totalHours).toLocaleString(Qt.locale("ru_RU"), 'f', 2)
                                                case 3: return model.taskCount || 0
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
}
