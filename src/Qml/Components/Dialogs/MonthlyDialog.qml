import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    title: "Ежемесячный отчёт"
    modal: true
    anchors.centerIn: parent
    width: 500
    height: 300

    property string monthlyInfo: ""

    contentItem: Rectangle {
        color: "white"
        Text {
            anchors.fill: parent
            anchors.margins: 20
            text: root.monthlyInfo
            font.pixelSize: 14
            wrapMode: Text.WordWrap
        }
    }

    standardButtons: Dialog.Ok
}
