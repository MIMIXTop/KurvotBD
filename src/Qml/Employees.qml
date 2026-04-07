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