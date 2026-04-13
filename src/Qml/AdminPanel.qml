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
                model: adminModel.tableNameList

                onCurrentTextChanged: {
                    adminModel.currentTableName = currentText
                    statusText.text = "Выбрана таблица: " + currentText
                    statusText.color = "#2196F3"
                }
                Component.onCompleted: {
                    adminModel.currentTableName = currentText
                }
            }
            
            Button {
                text: "Загрузить данные"
                highlighted: true
                onClicked: {
                    adminModel.loadTableData()
                    statusText.text = "Загружены данные для таблицы: " + tableSelector.currentText
                    statusText.color = "#4CAF50"
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
                // Table
                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "white"
                    border.color: "#dddddd"
                    border.width: 1
                    clip: true

                    HorizontalHeaderView {
                        id: tableHeader
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.margins: 1
                        syncView: tableView
                        z: 1
                        model: adminModel.headerList
                        delegate: Rectangle {
                            implicitWidth: 150
                            implicitHeight: 40
                            color: "#f0f0f0"
                            border.color: "#dddddd"
                            border.width: 1

                            Text {
                                anchors.centerIn: parent
                                text: modelData
                                font.bold: true
                                color: "#333333"
                            }
                        }
                    }

                    TableView {
                        id: tableView
                        anchors.top: tableHeader.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom

                        model: adminModel

                        columnWidthProvider: function(column) { return 150; }
                        rowHeightProvider: function(row) { return 40; }

                        delegate: Rectangle {
                            color: row % 2 === 0 ? "white" : "#fcfcfc" // Зебра
                            border.color: "#eeeeee"
                            border.width: 1

                            Text {
                                anchors.fill: parent
                                anchors.margins: 8
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                text: model.display !== undefined ? model.display : ""
                                elide: Text.ElideRight
                                color: "#444444"
                            }
                        }
                    }

                    Text {
                        anchors.centerIn: parent
                        text: "Нет данных для отображения"
                        color: "#999999"
                        font.pixelSize: 16
                        visible: tableView.rows === 0
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