import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    title: "Информация о рентабельности"
    modal: true
    anchors.centerIn: parent
    width: 500
    height: 300

    property string profitInfo: ""

    contentItem: Rectangle {
        color: "white"
        Text {
            anchors.fill: parent
            anchors.margins: 20
            text: root.profitInfo
            font.pixelSize: 14
            wrapMode: Text.WordWrap
        }
    }

    standardButtons: Dialog.Ok
}
