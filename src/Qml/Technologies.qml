import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Components/Tables"
import "Components/Dialogs"

Item {
    Layout.fillWidth: true
    Layout.fillHeight: true

    Component.onCompleted: {
        technologiesModel.applyFilters(techActiveCheck.checked, techSearchField.text)
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

            // Поиск технологии
            TextField {
                id: techSearchField
                width: 200
                height: 32
                placeholderText: "Поиск технологии"
                onTextChanged: technologiesModel.applyFilters(techActiveCheck.checked, text)
            }

            // Только активные проекты
            CheckBox {
                id: techActiveCheck
                text: "Только активные проекты"
                checked: true
                onCheckedChanged: technologiesModel.applyFilters(checked, techSearchField.text)
                contentItem: Text {
                    text: techActiveCheck.text
                    color: "#333333"
                    leftPadding: techActiveCheck.indicator.width + techActiveCheck.spacing
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
                    technologiesModel.applyFilters(
                        techActiveCheck.checked,
                        techSearchField.text
                    )
                }
            }
        }

        // Таблица технологий
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f5f5f5"
            radius: 8
            Layout.margins: 5

            TechnologiesTable {
                anchors.fill: parent
                anchors.margins: 5
                tableModel: technologiesModel
                onItemDoubleClicked: function(info) {
                    techDialog.techInfo = info
                    techDialog.open()
                }
            }
        }
    }

    TechDialog {
        id: techDialog
    }
}
