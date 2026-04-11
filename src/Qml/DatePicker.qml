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
        color: "black"
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
        
        onClicked: datePickerPopup.open()
    }
    
    Popup {
        id: datePickerPopup
        x: 0
        y: root.height + 2
        width: 360
        height: 180
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        
        background: Rectangle {
            color: "white"
            border.color: "#cccccc"
            border.width: 1
            radius: 4
        }
        
        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10
            spacing: 10
            
            RowLayout {
                Layout.fillWidth: true
                spacing: 5
                
                // День
                ComboBox {
                    id: dayCombo
                    Layout.preferredWidth: 70
                    model: {
                        let days = []
                        for (let i = 1; i <= 31; i++) {
                            days.push(("0" + i).slice(-2))
                        }
                        return days
                    }
                    currentIndex: {
                        if (dateField.text.length >= 2) {
                            let day = parseInt(dateField.text.substring(0, 2))
                            return day > 0 ? day - 1 : 0
                        }
                        return new Date().getDate() - 1
                    }
                    contentItem: Text {
                        text: dayCombo.displayText
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 5
                    }
                }
                
                // Месяц
                ComboBox {
                    id: monthCombo
                    Layout.preferredWidth: 90
                    model: ["Янв", "Фев", "Мар", "Апр", "Май", "Июн", "Июл", "Авг", "Сен", "Окт", "Ноя", "Дек"]
                    currentIndex: {
                        if (dateField.text.length >= 5) {
                            let month = parseInt(dateField.text.substring(3, 5))
                            return month > 0 ? month - 1 : 0
                        }
                        return new Date().getMonth()
                    }
                    contentItem: Text {
                        text: monthCombo.displayText
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 5
                    }
                }
                
                // Год
                ComboBox {
                    id: yearCombo
                    Layout.preferredWidth: 90
                    model: {
                        let years = []
                        for (let i = 2020; i <= 2030; i++) {
                            years.push(i.toString())
                        }
                        return years
                    }
                    currentIndex: {
                        if (dateField.text.length === 10) {
                            let year = parseInt(dateField.text.substring(6, 10))
                            return year >= 2020 && year <= 2030 ? year - 2020 : 6
                        }
                        return new Date().getFullYear() - 2020
                    }
                    contentItem: Text {
                        text: yearCombo.displayText
                        color: "white"
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 5
                    }
                }
            }
            
            Item { Layout.fillHeight: true }
            
            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                
                Button {
                    text: "Отмена"
                    Layout.fillWidth: true
                    onClicked: datePickerPopup.close()
                }
                
                Button {
                    text: "OK"
                    Layout.fillWidth: true
                    highlighted: true
                    onClicked: {
                        let day = dayCombo.currentText
                        let month = ("0" + (monthCombo.currentIndex + 1)).slice(-2)
                        let year = yearCombo.currentText
                        dateField.text = day + "." + month + "." + year
                        datePickerPopup.close()
                    }
                }
            }
        }
    }
}
