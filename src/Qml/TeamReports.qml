import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    Layout.fillWidth: true
    Layout.fillHeight: true

    Component.onCompleted: {
        updateTeamFilters()
    }

    function updateTeamFilters() {
        let roles = []
        if (roleDev.checked) roles.push("разработчик")
        if (roleTester.checked) roles.push("тестировщик")
        if (roleAnalyst.checked) roles.push("аналитик")
        if (roleManager.checked) roles.push("менеджер_проекта")
        if (roleArchitect.checked) roles.push("архитектор")
        if (roleTechWriter.checked) roles.push("технический_писатель")

        let positions = []
        if (teamPositionCombo.currentIndex > 0) {
            let positionText = teamPositionCombo.currentText
            if (positionText && positionText.length > 0) {
                positions.push(positionText)
            }
        }

        projectTeamModel.applyFilters(
            teamProjectCombo.currentIndex,
            roles,
            positions,
            teamActiveCheck.checked,
            0 // Default sort by Role
        )
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
                id: teamProjectCombo
                width: 200
                height: 32
                model: projectTeamModel.projectList
                onCurrentIndexChanged: updateTeamFilters()
            }

            // Должности
            ComboBox {
                id: teamPositionCombo
                width: 200
                height: 32
                model: ["Все должности"].concat(projectTeamModel.positionList)
                onCurrentIndexChanged: updateTeamFilters()
            }

            // Роли
            Column {
                spacing: 5
                Text {
                    text: "Роли:"
                    font.bold: true
                    color: "#333333"
                }
                Row {
                    spacing: 5
                    CheckBox {
                        id: roleDev
                        text: "разработчик"
                        onCheckedChanged: updateTeamFilters()
                        contentItem: Text {
                            text: roleDev.text
                            color: "#333333"
                            leftPadding: roleDev.indicator.width + roleDev.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    CheckBox {
                        id: roleTester
                        text: "тестировщик"
                        onCheckedChanged: updateTeamFilters()
                        contentItem: Text {
                            text: roleTester.text
                            color: "#333333"
                            leftPadding: roleTester.indicator.width + roleTester.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    CheckBox {
                        id: roleAnalyst
                        text: "аналитик"
                        onCheckedChanged: updateTeamFilters()
                        contentItem: Text {
                            text: roleAnalyst.text
                            color: "#333333"
                            leftPadding: roleAnalyst.indicator.width + roleAnalyst.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    CheckBox {
                        id: roleManager
                        text: "менеджер_проекта"
                        onCheckedChanged: updateTeamFilters()
                        contentItem: Text {
                            text: roleManager.text
                            color: "#333333"
                            leftPadding: roleManager.indicator.width + roleManager.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    CheckBox {
                        id: roleArchitect
                        text: "архитектор"
                        onCheckedChanged: updateTeamFilters()
                        contentItem: Text {
                            text: roleArchitect.text
                            color: "#333333"
                            leftPadding: roleArchitect.indicator.width + roleArchitect.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    CheckBox {
                        id: roleTechWriter
                        text: "технический_писатель"
                        onCheckedChanged: updateTeamFilters()
                        contentItem: Text {
                            text: roleTechWriter.text
                            color: "#333333"
                            leftPadding: roleTechWriter.indicator.width + roleTechWriter.spacing
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }

            // Только активные
            CheckBox {
                id: teamActiveCheck
                text: "Только активные"
                checked: true
                onCheckedChanged: updateTeamFilters()
                contentItem: Text {
                    text: teamActiveCheck.text
                    color: "#333333"
                    leftPadding: teamActiveCheck.indicator.width + teamActiveCheck.spacing
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
                onClicked: updateTeamFilters()
            }
        }

        // Таблица команды
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
                    id: teamHeaderView
                    syncView: teamTableView
                    Layout.fillWidth: true
                    model: ["Проект", "ФИО", "Должность", "Роль", "Фаза", "Часы"]
                }

                TableView {
                    id: teamTableView
                    resizableColumns: true
                    interactive: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    columnSpacing: 1
                    rowSpacing: 1
                    clip: true

                    columnWidthProvider: function (column) {
                        let minWidths = [200, 200, 150, 150, 150, 80];
                        return minWidths[column] || 100;
                    }

                    model: projectTeamModel

                    delegate: Rectangle {
                        implicitHeight: 50
                        border.width: 1
                        color: row % 2 === 0 ? "white" : "#f9f9f9"

                        MouseArea {
                            anchors.fill: parent
                            onDoubleClicked: {
                                teamDialog.teamInfo = "Проект: " + (model.projectName || "") +
                                    "\nФИО: " + (model.fullName || "") +
                                    "\nДолжность: " + (model.positionTitle || "") +
                                    "\nРоль: " + (model.projectRole || "") +
                                    "\nФаза: " + (model.phaseName || "") +
                                    "\nДата начала: " + (model.startDate || "") +
                                    "\nДата окончания: " + (model.endDate || "") +
                                    "\nЧасы выделено: " + (model.hoursAllocated || 0)
                                teamDialog.open()
                            }
                        }

                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            text: {
                                switch (column) {
                                    case 0: return model.projectName || ""
                                    case 1: return model.fullName || ""
                                    case 2: return model.positionTitle || ""
                                    case 3: return model.projectRole || ""
                                    case 4: return model.phaseName || ""
                                    case 5: return (model.hoursAllocated || 0).toString()
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

    // Dialog для команды
    Dialog {
        id: teamDialog
        title: "Информация о сотруднике"
        modal: true
        anchors.centerIn: parent
        width: 500
        height: 350

        property string teamInfo: ""

        contentItem: Rectangle {
            color: "white"
            Text {
                anchors.fill: parent
                anchors.margins: 20
                text: teamDialog.teamInfo
                font.pixelSize: 14
                wrapMode: Text.WordWrap
            }
        }

        standardButtons: Dialog.Ok
    }
}
