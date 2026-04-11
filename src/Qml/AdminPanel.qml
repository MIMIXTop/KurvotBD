import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: adminPanel
    
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 10
            
            Text {
                text: "Администрирование БД"
                font.pixelSize: 24
                font.bold: true
                color: "#2196F3"
            }
            
            Item { Layout.fillWidth: true }
            
            Text {
                text: "Пользователь: " + SessionManager.currentUser
                font.pixelSize: 14
                color: "#666666"
            }
        }
        
        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#cccccc"
        }
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 10
            
            Text {
                text: "Выберите таблицу:"
                font.pixelSize: 14
                color: "#333333"
            }
            
            ComboBox {
                id: tableSelector
                Layout.preferredWidth: 300
                model: [
                    "employees",
                    "departments",
                    "positions",
                    "projects",
                    "clients",
                    "bugs",
                    "releases",
                    "technologies",
                    "project_technologies",
                    "project_team",
                    "documentation",
                    "specifications",
                    "profitability",
                    "infrastructure",
                    "monthly_financial"
                ]
                
                onCurrentTextChanged: {
                    statusText.text = "Выбрана таблица: " + currentText
                    statusText.color = "#2196F3"
                }
            }
            
            Button {
                text: "Загрузить данные"
                highlighted: true
                onClicked: {
                    statusText.text = "Функция загрузки данных не реализована. Реализуйте самостоятельно."
                    statusText.color = "#ff9800"
                }
            }
            
            Item { Layout.fillWidth: true }
        }
        
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#f5f5f5"
            radius: 8
            border.color: "#cccccc"
            border.width: 1
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15
                
                Text {
                    text: "Универсальная таблица"
                    font.pixelSize: 18
                    font.bold: true
                    color: "#333333"
                }
                
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "white"
                    border.color: "#dddddd"
                    border.width: 1
                    
                    Text {
                        anchors.centerIn: parent
                        text: "Здесь будет отображаться содержимое выбранной таблицы.\n\nДля реализации необходимо:\n1. Создать универсальную модель данных\n2. Получить схему таблицы из information_schema\n3. Реализовать TableView с динамическими колонками\n4. Добавить диалоги для CRUD операций"
                        horizontalAlignment: Text.AlignHCenter
                        color: "#888888"
                        font.pixelSize: 14
                        wrapMode: Text.WordWrap
                    }
                }
                
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    
                    Button {
                        text: "Добавить"
                        icon.name: "list-add"
                        enabled: false
                        onClicked: {
                            statusText.text = "Функция добавления не реализована"
                            statusText.color = "#ff9800"
                        }
                    }
                    
                    Button {
                        text: "Редактировать"
                        icon.name: "document-edit"
                        enabled: false
                        onClicked: {
                            statusText.text = "Функция редактирования не реализована"
                            statusText.color = "#ff9800"
                        }
                    }
                    
                    Button {
                        text: "Удалить"
                        icon.name: "edit-delete"
                        enabled: false
                        onClicked: {
                            statusText.text = "Функция удаления не реализована"
                            statusText.color = "#ff9800"
                        }
                    }
                    
                    Item { Layout.fillWidth: true }
                    
                    Button {
                        text: "Обновить"
                        icon.name: "view-refresh"
                        onClicked: {
                            statusText.text = "Функция обновления не реализована"
                            statusText.color = "#ff9800"
                        }
                    }
                }
            }
        }
        
        Rectangle {
            Layout.fillWidth: true
            height: 40
            color: "#e3f2fd"
            radius: 4
            border.color: "#2196F3"
            border.width: 1
            
            Text {
                id: statusText
                anchors.fill: parent
                anchors.margins: 10
                text: "Готов к работе. Выберите таблицу для начала."
                color: "#666666"
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
            }
        }
    }
}