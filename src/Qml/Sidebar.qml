import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    color: "lightblue"
    property alias model: listView.model

    signal filterSelected(int selectedIndex)

    ListView {
        id: listView
        anchors.fill: parent
        spacing: 10
        clip: true

        topMargin: 15
        bottomMargin: 15
        leftMargin: 15
        rightMargin: 15

        delegate: FilterDelegate {
            title: model.name

            onClicked: root.filterSelected(index)
        }
    }
}