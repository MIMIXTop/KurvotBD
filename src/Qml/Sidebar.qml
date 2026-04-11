import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: "#2c3e50"

    property alias model: listView.model
    signal sectionSelected(int index)
    signal logoutClicked()

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0
            clip: true

            delegate: Rectangle {
                width: ListView.view.width
                height: 60
                color: mouseArea.containsMouse ? "#34495e" : "transparent"
                visible: {
                    if (index === 5) {
                        return SessionManager.isAdmin
                    }
                    return true
                }

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

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 5

            Rectangle {
                height: 1
                color: "#34495e"
            }

            Button {
                text: "Выход"
                visible: SessionManager.isLoggedIn
                Layout.fillWidth: true
                height: 50
                background: Rectangle { color: "#c0392b" }
                contentItem: Text {
                    text: parent.text
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: root.logoutClicked()
            }
        }
    }
}
