import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    title: "Информация об инфраструктуре"
    modal: true
    anchors.centerIn: parent
    width: 500
    height: 350

    property string infraInfo: ""

    contentItem: Rectangle {
        color: "white"
        Text {
            anchors.fill: parent
            anchors.margins: 20
            text: root.infraInfo
            font.pixelSize: 14
            wrapMode: Text.WordWrap
        }
    }

    standardButtons: Dialog.Ok
}
