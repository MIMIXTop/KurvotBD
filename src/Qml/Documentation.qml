import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Components/Tables"
import "Components/Dialogs"

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

            DocumentationTable {
                anchors.fill: parent
                anchors.margins: 5
                tableModel: documentationModel
                onItemDoubleClicked: function(info, content) {
                    docDialog.docInfo = info
                    docDialog.docContent = content
                    docDialog.open()
                }
            }
        }
    }

    DocDialog {
        id: docDialog
    }
}
