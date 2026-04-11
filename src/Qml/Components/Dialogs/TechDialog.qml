import QtQuick
import QtQuick.Controls

Dialog {
    id: root
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
            text: root.techInfo
            font.pixelSize: 14
            wrapMode: Text.WordWrap
        }
    }

    standardButtons: Dialog.Ok
}
