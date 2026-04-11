import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    title: "Информация о баге"
    modal: true
    anchors.centerIn: parent
    width: 600
    height: 400

    property string bugInfo: ""

    contentItem: Rectangle {
        color: "white"
        Text {
            anchors.fill: parent
            anchors.margins: 20
            text: root.bugInfo
            font.pixelSize: 14
            wrapMode: Text.WordWrap
        }
    }

    standardButtons: Dialog.Ok
}
