import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
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
                text: root.docInfo
                font.pixelSize: 14
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }

            Text {
                text: "Содержимое документа:"
                font.bold: true
                font.pixelSize: 16
                visible: root.docContent.length > 0
            }

            TextArea {
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: root.docContent.length > 0
                text: root.docContent
                readOnly: true
                wrapMode: Text.WordWrap
                font.pixelSize: 13
            }
        }
    }

    standardButtons: Dialog.Ok
}
