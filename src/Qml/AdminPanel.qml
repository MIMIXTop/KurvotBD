import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Components/Forms"

Item {
    id: adminPanel
    
    property int selectedRow: -1
    property var selectedRowData: ({})
    
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
                    // Сбрасываем выбранную строку при смене таблицы
                    selectedRow = -1
                    selectedRowData = {}
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
                            color: selectedRow === row ? "#e3f2fd" : (row % 2 === 0 ? "white" : "#fcfcfc")
                            border.color: selectedRow === row ? "#2196F3" : "#eeeeee"
                            border.width: selectedRow === row ? 2 : 1

                            Text {
                                anchors.fill: parent
                                anchors.margins: 8
                                verticalAlignment: Text.AlignVCenter
                                horizontalAlignment: Text.AlignHCenter
                                text: model.display !== undefined ? model.display : ""
                                elide: Text.ElideRight
                                color: "#444444"
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    selectedRow = row
                                    // Получаем данные выбранной строки
                                    selectedRowData = adminModel.getRowData(row)
                                    statusText.text = "Выбрана строка: " + (row + 1)
                                    statusText.color = "#2196F3"
                                }
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
                        enabled: adminModel.currentTableName !== ""
                        onClicked: {
                            editForm.isEditMode = false
                            editForm.formTitle = adminModel.currentTableName
                            
                            // Преобразуем имя таблицы в имя модели
                            let modelName = formHelper.tableNameToModelName(adminModel.currentTableName)
                            editForm.currentModel = modelName

                            // Загрузка конфигурации формы
                            let configStr = formHelper.getFormConfig(modelName)
                            let config = JSON.parse(configStr)

                            // Загрузка данных для FK
                            let fkData = {}
                            for (let i = 0; i < config.fields.length; i++) {
                                let field = config.fields[i]
                                if (field.fk_info) {
                                    fkData[field.fk_info.source_table] = formHelper.getForeignKeyData(field.fk_info.source_table)
                                }
                            }
                            editForm.fkData = fkData

                            // Инициализация пустых данных
                            let initialData = {}
                            for (let i = 0; i < config.fields.length; i++) {
                                let field = config.fields[i]
                                if (!field.is_auto_generated && !field.is_primary_key) {
                                    if (field.type === "Boolean") initialData[field.name] = false
                                    else if (field.type === "Number" || field.type === "DoubleNumber") initialData[field.name] = 0
                                    else initialData[field.name] = ""
                                }
                            }
                            
                            editForm.setupForm(config.fields, initialData)
                            editForm.open()
                        }
                    }
                    
                    Button {
                        text: "Редактировать"
                        icon.name: "document-edit"
                        enabled: selectedRow >= 0
                        onClicked: {
                            editForm.isEditMode = true
                            editForm.formTitle = adminModel.currentTableName
                            
                            // Преобразуем имя таблицы в имя модели
                            let modelName = formHelper.tableNameToModelName(adminModel.currentTableName)
                            editForm.currentModel = modelName
                            
                            console.log("Current table name:", adminModel.currentTableName)
                            console.log("Model name:", modelName)

                            // Загрузка конфигурации формы
                            let configStr = formHelper.getFormConfig(modelName)
                            console.log("Config string:", configStr)
                            let config = JSON.parse(configStr)
                            console.log("Parsed config:", JSON.stringify(config))

                            // Загрузка данных для FK
                            let fkData = {}
                            for (let i = 0; i < config.fields.length; i++) {
                                let field = config.fields[i]
                                if (field.fk_info) {
                                    fkData[field.fk_info.source_table] = formHelper.getForeignKeyData(field.fk_info.source_table)
                                }
                            }
                            editForm.fkData = fkData

                            // selectedRowData уже содержит правильные имена полей (field.name)
                            console.log("Selected row data:", JSON.stringify(selectedRowData))
                            console.log("Config fields:", JSON.stringify(config.fields.map(f => f.name)))

                            // Загрузка данных выбранной строки
                            console.log("FINAL DATA TO FORM:", JSON.stringify(selectedRowData))
                            editForm.setupForm(config.fields, selectedRowData)
                            editForm.open()
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

    // Форма редактирования/добавления
    GenericEditForm {
        id: editForm
        onSaveRequested: function(data) {
            let formData = editForm.getFormData()
            console.log("Save requested for:", editForm.currentModel, formData)
            
            let success = false
            if (editForm.isEditMode) {
                // Режим редактирования - находим primary key из конфигурации
                let configStr = formHelper.getFormConfig(editForm.currentModel)
                let config = JSON.parse(configStr)
                
                // Ищем primary key поле
                let pkField = null
                for (let i = 0; i < config.fields.length; i++) {
                    if (config.fields[i].is_primary_key) {
                        pkField = config.fields[i].name
                        break
                    }
                }
                
                if (!pkField) {
                    errorDialog.errorText = "Ошибка: не найден первичный ключ для модели " + editForm.currentModel
                    errorDialog.open()
                    return
                }
                
                let recordId = selectedRowData[pkField]
                console.log("Primary key field:", pkField, "ID:", recordId)
                
                success = formHelper.updateRecord(editForm.currentModel, recordId, formData)
                if (success) {
                    statusText.text = "Запись обновлена: " + editForm.currentModel
                    statusText.color = "#4CAF50"
                    selectedRow = -1
                    selectedRowData = {}
                    adminModel.loadTableData()
                    editForm.close()
                } else {
                    errorDialog.errorText = "Ошибка при обновлении записи:\n\n" + formHelper.lastError
                    errorDialog.open()
                }
            } else {
                // Режим добавления
                success = formHelper.insertRecord(editForm.currentModel, formData)
                if (success) {
                    statusText.text = "Запись добавлена: " + editForm.currentModel
                    statusText.color = "#4CAF50"
                    adminModel.loadTableData()
                    editForm.close()
                } else {
                    errorDialog.errorText = "Ошибка при добавлении записи:\n\n" + formHelper.lastError
                    errorDialog.open()
                }
            }
        }
    }

    // Диалог для отображения ошибок БД
    Dialog {
        id: errorDialog
        title: "Ошибка сохранения"
        modal: true
        anchors.centerIn: parent
        width: Math.min(500, parent.width - 40)
        
        property string errorText: ""
        
        contentItem: Rectangle {
            color: "white"
            implicitHeight: errorTextItem.implicitHeight + 40
            
            Text {
                id: errorTextItem
                anchors.fill: parent
                anchors.margins: 20
                text: errorDialog.errorText
                wrapMode: Text.Wrap
                color: "#d32f2f"
                font.pixelSize: 13
            }
        }
        
        standardButtons: Dialog.Ok
    }
}