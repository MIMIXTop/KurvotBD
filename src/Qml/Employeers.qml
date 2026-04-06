import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            width: parent.width
            Layout.fillWidth: true
            TabButton {
                text: "Сотрудники"
                width: implicitWidth
            }
            TabButton {
                text: "Кадры"
                width: implicitWidth
            }
        }

        Rectangle {
            id: tiel
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "red"
            radius: 15
            border{
                width: 10
                color: "white"
            }
        }
    }
}