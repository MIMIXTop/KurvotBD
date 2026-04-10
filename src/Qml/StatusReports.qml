import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    Layout.fillWidth: true
    Layout.fillHeight: true

    function updateStatusReport() {
        let projectId = projectTeamModel.getProjectId(statusProjectCombo.currentIndex)
        if (projectId > 0 && 
            statusFromDate.selectedDate.length === 10 && 
            statusToDate.selectedDate.length === 10) {
            projectStatusModel.loadStatus(
                projectId,
                statusFromDate.toSqlFormat(statusFromDate.selectedDate),
                statusToDate.toSqlFormat(statusToDate.selectedDate)
            )
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Фильтры
        Flow {
            Layout.fillWidth: true
            Layout.margins: 10
            spacing: 10
            Layout.preferredHeight: implicitHeight

            // Проект
            ComboBox {
                id: statusProjectCombo
                width: 200
                height: 32
                model: projectTeamModel.projectList
                
                onCurrentIndexChanged: updateStatusReport()
            }

            // Дата от
            DatePicker {
                id: statusFromDate
                selectedDate: "01.01.2024"
                placeholderText: "Дата от (ДД.ММ.ГГГГ)"
                onSelectedDateChanged: updateStatusReport()
            }

            // Дата до
            DatePicker {
                id: statusToDate
                selectedDate: "31.12.2026"
                placeholderText: "Дата до (ДД.ММ.ГГГГ)"
                onSelectedDateChanged: updateStatusReport()
            }

            // Кнопка поиска
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
                onClicked: updateStatusReport()
            }
        }

        // Карточки с метриками
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f5f5f5"
            radius: 8
            Layout.margins: 5

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                // Заголовок
                Text {
                    text: projectStatusModel.hasData ?
                        "Статус-отчёт: " + projectStatusModel.projectName :
                        "Выберите проект и период для отображения отчёта"
                    font.pixelSize: 18
                    font.bold: true
                    color: "#333333"
                }

                // Карточки метрик
                GridLayout {
                    columns: 3
                    rowSpacing: 15
                    columnSpacing: 15
                    Layout.fillWidth: true

                    // Карточка 1: Текущая фаза
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.preferredHeight: 100
                        color: "#e3f2fd"
                        radius: 8

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 5
                            Text {
                                text: "Текущая фаза"
                                font.pixelSize: 14
                                color: "#1565c0"
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: projectStatusModel.currentPhase || "-"
                                font.pixelSize: 20
                                color: "#0d47a1"
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }

                    // Карточка 2: Задач выполнено
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.preferredHeight: 100
                        color: "#e8f5e9"
                        radius: 8

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 5
                            Text {
                                text: "Задач выполнено"
                                font.pixelSize: 14
                                color: "#2e7d32"
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: projectStatusModel.tasksLogged.toString()
                                font.pixelSize: 20
                                color: "#1b5e20"
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }

                    // Карточка 3: Критических рисков
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.preferredHeight: 100
                        color: projectStatusModel.criticalRisksCount > 3 ? "#ffebee" : "#fff3e0"
                        radius: 8

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 5
                            Text {
                                text: "Критических рисков"
                                font.pixelSize: 14
                                color: projectStatusModel.criticalRisksCount > 3 ? "#c62828" : "#e65100"
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: projectStatusModel.criticalRisksCount.toString()
                                font.pixelSize: 20
                                color: projectStatusModel.criticalRisksCount > 3 ? "#b71c1c" : "#bf360c"
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }

                    // Карточка 4: Часов потрачено
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.preferredHeight: 100
                        color: "#f3e5f5"
                        radius: 8

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 5
                            Text {
                                text: "Часов потрачено"
                                font.pixelSize: 14
                                color: "#6a1b9a"
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: Number(projectStatusModel.teamHoursSpent).toFixed(1)
                                font.pixelSize: 20
                                color: "#4a148c"
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }

                    // Карточка 5: Период отчёта
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.minimumWidth: 200
                        Layout.preferredHeight: 100
                        color: "#fafafa"
                        radius: 8
                        border.color: "#e0e0e0"
                        border.width: 1

                        ColumnLayout {
                            anchors.centerIn: parent
                            spacing: 5
                            Text {
                                text: "Период отчёта"
                                font.pixelSize: 14
                                color: "#616161"
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                            Text {
                                text: projectStatusModel.reportPeriod || "-"
                                font.pixelSize: 14
                                color: "#212121"
                                font.bold: true
                                Layout.alignment: Qt.AlignHCenter
                            }
                        }
                    }
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }
}
