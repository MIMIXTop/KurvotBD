import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: adminPanel
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10
        
        Text {
            text: "Администрирование"
            font.pixelSize: 24
            font.bold: true
            color: "#2196F3"
        }
        
        Text {
            text: "Универсальная таблица для управления данными (заглушка)"
            font.pixelSize: 14
            color: "#666666"
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f5f5f5"
            radius: 8
            
            Text {
                anchors.centerIn: parent
                text: "Здесь будет универсальная таблица с CRUD операциями.\nПользователь реализует это самостоятельно."
                horizontalAlignment: Text.AlignHCenter
                color: "#888888"
                font.pixelSize: 14
            }
        }
    }
}