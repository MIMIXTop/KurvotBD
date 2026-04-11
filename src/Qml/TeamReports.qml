import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Components/Tables"
import "Components/Dialogs"

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

            TeamTable {
                anchors.fill: parent
                anchors.margins: 5
                tableModel: projectTeamModel
                onItemDoubleClicked: function(info) {
                    teamDialog.teamInfo = info
                    teamDialog.open()
                }
            }
        }
    }

    TeamDialog {
        id: teamDialog
    }
}
