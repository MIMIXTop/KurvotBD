import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    width: 180
    height: 32
    border.color: dateField.activeFocus ? "#2196F3" : "#cccccc"
    border.width: 1
    radius: 4
    color: "white"
    
    property alias selectedDate: dateField.text
    property string placeholderText: "ДД.ММ.ГГГГ"
    
    // Преобразование из DD.MM.YYYY в YYYY-MM-DD для SQL
    function toSqlFormat(dateStr) {
        if (dateStr.length !== 10) return dateStr
        let parts = dateStr.split('.')
        if (parts.length === 3) {
            return parts[2] + '-' + parts[1] + '-' + parts[0]
        }
        return dateStr
    }
    
    // Преобразование из YYYY-MM-DD в DD.MM.YYYY для отображения
    function fromSqlFormat(dateStr) {
        if (dateStr.length !== 10) return dateStr
        let parts = dateStr.split('-')
        if (parts.length === 3) {
            return parts[2] + '.' + parts[1] + '.' + parts[0]
        }
        return dateStr
    }
    
    TextField {
        id: dateField
        anchors.fill: parent
        anchors.margins: 1
        placeholderText: root.placeholderText
        font.pixelSize: 13
        verticalAlignment: Text.AlignVCenter
        background: Rectangle {
            color: "transparent"
        }
        
        // Валидация ввода
        validator: RegularExpressionValidator {
            regularExpression: /^(0[1-9]|[12][0-9]|3[01])\.(0[1-9]|1[0-2])\.\d{4}$/
        }
        
        onTextChanged: {
            // Автоматическое добавление точек при вводе
            if (text.length === 2 && !text.includes('.')) {
                text = text + '.'
            } else if (text.length === 5 && text.split('.').length === 2) {
                text = text + '.'
            }
        }
    }
    
    Button {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 2
        width: 28
        height: 28
        text: "📅"
        font.pixelSize: 14
        
        background: Rectangle {
            color: parent.hovered ? "#e3f2fd" : "transparent"
            radius: 3
        }
        
        onClicked: {
            // Установить текущую дату
            let today = new Date()
            let day = ("0" + today.getDate()).slice(-2)
            let month = ("0" + (today.getMonth() + 1)).slice(-2)
            let year = today.getFullYear()
            dateField.text = day + "." + month + "." + year
        }
    }
}
