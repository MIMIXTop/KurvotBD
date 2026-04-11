import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    title: "Информация о проекте"
    modal: true
    anchors.centerIn: parent
    width: 400
    height: 200

    property string projectInfo: ""

    contentItem: Rectangle {
        color: "white"
        Text {
            anchors.centerIn: parent
            text: root.projectInfo
            font.pixelSize: 14
            wrapMode: Text.WordWrap
        }
    }

    standardButtons: Dialog.Ok
}
