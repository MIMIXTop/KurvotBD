import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#2c3e50"

    property alias model: listView.model
    signal sectionSelected(int index)

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 0
        clip: true

        delegate: Rectangle {
            width: ListView.view.width
            height: 60
            color: mouseArea.containsMouse ? "#34495e" : "transparent"

            Text {
                anchors.fill: parent
                anchors.leftMargin: 20
                anchors.rightMargin: 20
                text: model.name
                color: "white"
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: root.sectionSelected(index)
            }
        }
    }
}
