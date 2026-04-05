import QtQuick

Rectangle {
    id: root

    width: ListView.view ? (ListView.view.width - ListView.view.leftMargin - ListView.view.rightMargin) : 200
    height: 50
    color: mouseArea.pressed ? "darkred" : "indianred"
    radius: 8

    property string title: ""

    signal clicked()

    Text {
        anchors.centerIn: parent
        text: root.title
        color: "white"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
        hoverEnabled: true
    }
}