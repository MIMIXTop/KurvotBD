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
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        EmployeesTable {
                            anchors.fill: parent
                            anchors.margins: 1
                            tableModel: employeesTableModel
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
                        DatePicker {
                            id: workloadFromDateField
                            selectedDate: "01.01.2024"
                            placeholderText: "Период от (ДД.ММ.ГГГГ)"
                        }

                        // Период до
                        DatePicker {
                            id: workloadToDateField
                            selectedDate: "31.12.2026"
                            placeholderText: "Период до (ДД.ММ.ГГГГ)"
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

                                let fromDate = workloadFromDateField.selectedDate.length === 10 ? 
                                    workloadFromDateField.toSqlFormat(workloadFromDateField.selectedDate) : ""
                                let toDate = workloadToDateField.selectedDate.length === 10 ? 
                                    workloadToDateField.toSqlFormat(workloadToDateField.selectedDate) : ""

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

                        WorkloadTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: workloadModel
                        }
                    }
                }
            }
        }
    }
}
