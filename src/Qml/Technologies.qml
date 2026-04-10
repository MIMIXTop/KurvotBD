import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 5
                spacing: 0

                HorizontalHeaderView {
                    id: techHeaderView
                    syncView: techTableView
                    Layout.fillWidth: true
                    model: ["Технология", "Количество проектов"]
                }

                TableView {
                    id: techTableView
                    resizableColumns: true
                    interactive: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    columnSpacing: 1
                    rowSpacing: 1
                    clip: true

                    columnWidthProvider: function (column) {
                        let minWidths = [300, 200];
                        return minWidths[column] || 100;
                    }

                    model: technologiesModel

                    delegate: Rectangle {
                        implicitHeight: 50
                        border.width: 1
                        color: row % 2 === 0 ? "white" : "#f9f9f9"

                        MouseArea {
                            anchors.fill: parent
                            onDoubleClicked: {
                                techDialog.techInfo = "Технология: " + (model.techName || "") +
                                    "\nИспользуется в проектах: " + (model.projectCount || 0)
                                techDialog.open()
                            }
                        }

                        Text {
                            anchors.centerIn: parent
                            text: {
                                switch (column) {
                                    case 0: return model.techName || ""
                                    case 1: return (model.projectCount || 0).toString()
                                    default: return ""
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Dialog для технологий
    Dialog {
        id: techDialog
        title: "Информация о технологии"
        modal: true
        anchors.centerIn: parent
        width: 400
        height: 200

        property string techInfo: ""

        contentItem: Rectangle {
            color: "white"
            Text {
                anchors.fill: parent
                anchors.margins: 20
                text: techDialog.techInfo
                font.pixelSize: 14
                wrapMode: Text.WordWrap
            }
        }

        standardButtons: Dialog.Ok
    }
}
