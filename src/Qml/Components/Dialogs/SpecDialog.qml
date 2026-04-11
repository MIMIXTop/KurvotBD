import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    title: "Техническое задание"
    modal: true
    anchors.centerIn: parent
    width: 700
    height: 600

    property string specInfo: ""
    property string specContent: ""

    contentItem: Rectangle {
        color: "white"
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            Text {
                text: root.specInfo
                font.pixelSize: 14
                Layout.fillWidth: true
                wrapMode: Text.WordWrap
            }

            Text {
                text: "Техническое задание:"
                font.bold: true
                font.pixelSize: 16
                visible: root.specContent.length > 0
            }

            TextArea {
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: root.specContent.length > 0
                text: root.specContent
                readOnly: true
                wrapMode: Text.WordWrap
                font.pixelSize: 13
            }
        }
    }

    standardButtons: Dialog.Ok
}
