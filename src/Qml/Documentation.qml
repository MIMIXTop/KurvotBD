import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    Layout.fillWidth: true
    Layout.fillHeight: true

    Component.onCompleted: {
        updateDocFilters()
    }

    function updateDocFilters() {
        let projectName = ""
        if (docProjectCombo.currentIndex > 0) {
            projectName = docProjectCombo.currentText
        }

        let docType = ""
        if (docTypeCombo.currentIndex > 0) {
            docType = docTypeCombo.currentText
        }

        documentationModel.applyFilters(projectName, docType)
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
                id: docProjectCombo
                width: 200
                height: 32
                model: ["Все проекты"].concat(documentationModel.projectList)
                onCurrentIndexChanged: updateDocFilters()
            }

            // Тип документации
            ComboBox {
                id: docTypeCombo
                width: 200
                height: 32
                model: ["Все типы"].concat(documentationModel.docTypeList)
                onCurrentIndexChanged: updateDocFilters()
            }

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
                onClicked: updateDocFilters()
            }
        }

        // Таблица документации
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
                    id: docHeaderView
                    syncView: docTableView
                    Layout.fillWidth: true
                    model: ["Проект", "Тип", "Версия/Дата", "Автор", "Последнее обновление", "Путь"]
                }

                TableView {
                    id: docTableView
                    resizableColumns: true
                    interactive: false
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    columnSpacing: 1
                    rowSpacing: 1
                    clip: true

                    columnWidthProvider: function (column) {
                        let minWidths = [200, 150, 120, 150, 120, 200];
                        return minWidths[column] || 100;
                    }

                    model: documentationModel

                    delegate: Rectangle {
                        implicitHeight: 50
                        border.width: 1
                        color: row % 2 === 0 ? "white" : "#f9f9f9"

                        MouseArea {
                            anchors.fill: parent
                            onDoubleClicked: {
                                docDialog.docInfo = "ID: " + (model.docId || 0) +
                                    "\nПроект: " + (model.projectName || "") +
                                    "\nТип: " + (model.docType || "") +
                                    "\nАвтор: " + (model.authorName || "") +
                                    "\nДата создания: " + (model.creationDate || "") +
                                    "\nПоследнее обновление: " + (model.lastUpdate || "") +
                                    "\nПуть: " + (model.storagePath || "")

                                docDialog.docContent = model.documentText || ""
                                docDialog.open()
                            }
                        }

                        Text {
                            anchors.fill: parent
                            anchors.margins: 5
                            text: {
                                switch (column) {
                                    case 0:
                                        return model.projectName || ""
                                    case 1:
                                        return model.docType || ""
                                    case 2:
                                        return model.creationDate || ""
                                    case 3:
                                        return model.authorName || ""
                                    case 4:
                                        return model.lastUpdate || ""
                                    case 5:
                                        return model.storagePath || ""
                                    default:
                                        return ""
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

    // Dialog для документации
    Dialog {
        id: docDialog
        title: "Информация о документе"
        modal: true
        anchors.centerIn: parent
        width: 700
        height: 600

        property string docInfo: ""
        property string docContent: ""

        contentItem: Rectangle {
            color: "white"
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 10

                Text {
                    text: docDialog.docInfo
                    font.pixelSize: 14
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                }

                Text {
                    text: "Содержание / ТЗ:"
                    font.bold: true
                    visible: docDialog.docContent.length > 0
                }

                TextArea {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: docDialog.docContent.length > 0
                    text: docDialog.docContent
                    readOnly: true
                    wrapMode: Text.WordWrap
                    font.pixelSize: 13
                }
            }
        }

        standardButtons: Dialog.Ok
    }
}
