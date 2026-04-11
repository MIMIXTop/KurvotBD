import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: loginDialog
    title: "Вход в систему"
    modal: true
    anchors.centerIn: parent
    width: 400
    height: 350
    
    property alias username: usernameField.text
    property alias password: passwordField.text
    
    background: Rectangle {
        color: "white"
        border.color: "#cccccc"
        border.width: 1
        radius: 8
    }
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15
        
        Text {
            text: "Kurvot - IT Team Management"
            font.pixelSize: 18
            font.bold: true
            color: "#2196F3"
            horizontalAlignment: Text.AlignHCenter
        }
        
        Text {
            text: "Введите ваши учетные данные"
            font.pixelSize: 12
            color: "#666666"
            horizontalAlignment: Text.AlignHCenter
        }
        
        TextField {
            id: usernameField
            placeholderText: "Имя пользователя "
            Layout.fillWidth: true
            height: 40
            font.pixelSize: 14
        }
        
        TextField {
            id: passwordField
            placeholderText: "Пароль"
            echoMode: TextInput.Password
            Layout.fillWidth: true
            height: 40
            font.pixelSize: 14
        }
        
        Text {
            id: errorLabel
            text: ""
            color: "red"
            font.pixelSize: 12
            visible: false
            horizontalAlignment: Text.AlignHCenter
        }
        
        Button {
            text: "Войти"
            Layout.fillWidth: true
            height: 40
            font.pixelSize: 14
            highlighted: true
            
            onClicked: {
                if (usernameField.text.trim() === "" || passwordField.text.trim() === "") {
                    errorLabel.text = "Заполните все поля"
                    errorLabel.visible = true
                    return
                }
                SessionManager.login(usernameField.text, passwordField.text)
            }
        }
    }
    
    Connections {
        target: SessionManager
        
        function onLoginSuccess() {
            loginDialog.close()
        }
        
        function onLoginFailed(error) {
            errorLabel.text = error
            errorLabel.visible = true
        }
    }
}