import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "../.."
import "../../.."

Dialog {
    id: root
    modal: true
    anchors.centerIn: parent
    width: Math.min(700, parent.width - 40)
    height: Math.min(600, parent.height - 40)
    title: (isEditMode ? "Редактирование: " : "Добавление: ") + formTitle

    property string formTitle: ""
    property bool isEditMode: false
    property var fkData: ({})
    property string currentModel: ""

    signal saveRequested(var data)
    signal cancelRequested()

    property var formFields: []
    property int formFieldsVersion: 0

    function setupForm(fieldsConfig, initialData) {
        formFields = []

        for (let i = 0; i < fieldsConfig.length; i++) {
            let field = fieldsConfig[i]
            if (!field.is_auto_generated && (!field.is_primary_key || isEditMode)) {
                let value = ""
                if (field.type === "Boolean") value = false
                else if (field.type === "Number" || field.type === "DoubleNumber") value = 0

                if (initialData[field.name] !== undefined) {
                    value = initialData[field.name]
                }

                formFields.push({
                    "name": field.name,
                    "display_name": field.display_name,
                    "type": field.type,
                    "enum_options": field.enum_options || [],
                    "fk_info": field.fk_info || null,
                    "value": value
                })
            }
        }
        formFieldsVersion++
    }

    function getFormData() {
        let data = {}
        for (let i = 0; i < formFields.length; i++) {
            let item = formFields[i]
            data[item.name] = item.value
        }
        return data
    }

    function validateForm() {
        let errors = []
        
        for (let i = 0; i < formFields.length; i++) {
            let field = formFields[i]
            let value = field.value
            
            // Проверка обязательных текстовых полей
            if ((field.type === "Text" || field.type === "TextArea") && 
                field.type !== "OptionalDate" && field.type !== "OptionalForeignKey") {
                if (!value || String(value).trim() === "") {
                    errors.push("Поле '" + field.display_name + "' обязательно для заполнения")
                }
            }
            
            // Проверка обязательных числовых полей
            if (field.type === "Number" || field.type === "DoubleNumber") {
                if (value === undefined || value === null || value === "") {
                    errors.push("Поле '" + field.display_name + "' обязательно для заполнения")
                } else if (isNaN(value)) {
                    errors.push("Поле '" + field.display_name + "' должно содержать число")
                } else if (field.type === "Number" && !Number.isInteger(Number(value))) {
                    errors.push("Поле '" + field.display_name + "' должно быть целым числом")
                }
            }
            
            // Проверка обязательных дат
            if (field.type === "Date") {
                if (!value || String(value).trim() === "") {
                    errors.push("Поле '" + field.display_name + "' обязательно для заполнения")
                }
            }
            
            // Проверка обязательных FK
            if (field.type === "ForeignKey") {
                if (!value || value === 0) {
                    errors.push("Необходимо выбрать значение для поля '" + field.display_name + "'")
                }
            }
        }
        
        return errors
    }

    DialogButtonBox {
        standardButtons: DialogButtonBox.NoButton
        visible: false
    }

    contentItem: Rectangle {
        color: "white"
        radius: 4

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 20
            spacing: 10

            Flickable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                contentHeight: formColumn.implicitHeight
                clip: true

                ColumnLayout {
                    id: formColumn
                    width: parent.width
                    spacing: 8

                    Repeater {
                        model: root.formFieldsVersion && root.formFields.length

                        ColumnLayout {
                            spacing: 4

                            property var field: root.formFields[index]

                            Text {
                                text: field.display_name + (field.type === "OptionalDate" || field.type === "OptionalForeignKey" ? " (необязательно)" : "")
                                font.pixelSize: 13
                                font.bold: true
                                color: "#333333"
                            }

                            // 1. Однострочный текст
                            TextField {
                                visible: field.type === "Text"
                                Layout.fillWidth: true
                                placeholderText: "Введите " + field.display_name.toLowerCase()
                                font.pixelSize: 13

                                text: {
                                    root.formFieldsVersion
                                    return field.value !== undefined ? String(field.value) : ""
                                }

                                // ВАЖНО: Защита от невидимого редактирования
                                onTextEdited: if (visible) field.value = text
                            }

                            // 2. Многострочный текст (ПРАВИЛЬНАЯ реализация TextArea)
                            ScrollView {
                                visible: field.type === "TextArea"
                                Layout.fillWidth: true
                                Layout.preferredHeight: 120
                                clip: true

                                TextArea {
                                    placeholderText: "Введите " + field.display_name.toLowerCase()
                                    font.pixelSize: 13
                                    wrapMode: TextEdit.Wrap

                                    text: {
                                        root.formFieldsVersion
                                        return field.value !== undefined ? String(field.value) : ""
                                    }

                                    onTextEdited: if (field.type === "TextArea") field.value = text
                                }
                            }

                            DatePicker {
                                visible: field.type === "Date" || field.type === "OptionalDate"
                                selectedDate: {
                                    let sqlDate = field.value
                                    if (!sqlDate) return ""
                                    let sqlStr = String(sqlDate)
                                    if (sqlStr.length < 10) return ""
                                    let parts = sqlStr.split('-')
                                    return parts[2] + "." + parts[1] + "." + parts[0]
                                }

                                onSelectedDateChanged: {
                                    if (visible && selectedDate && selectedDate.length === 10) {
                                        let parts = selectedDate.split('.')
                                        if (parts.length === 3) {
                                            field.value = parts[2] + "-" + parts[1] + "-" + parts[0]
                                        }
                                    }
                                }
                            }

                            SpinBox {
                                visible: field.type === "Number"
                                value: typeof field.value === "number" ? field.value : parseInt(field.value) || 0
                                from: 0
                                to: 999999
                                stepSize: 1
                                editable: true

                                onValueChanged: if (visible) field.value = value
                            }

                            DoubleSpinBox {
                                visible: field.type === "DoubleNumber"
                                value: Math.round((parseFloat(field.value) || 0) * 100)

                                onValueChanged: if (visible) field.value = value / 100
                            }

                            CheckBox {
                                visible: field.type === "Boolean"
                                checked: !!field.value
                                text: checked ? "Да" : "Нет"

                                onCheckedChanged: if (visible) field.value = checked
                            }

                            ComboBox {
                                visible: field.type === "Enum"
                                Layout.fillWidth: true
                                model: {
                                    let displayNames = []
                                    for (let opt of field.enum_options) {
                                        displayNames.push(opt.display_name)
                                    }
                                    return displayNames
                                }

                                currentIndex: {
                                    for (let i = 0; i < field.enum_options.length; i++) {
                                        if (field.enum_options[i].value === field.value) {
                                            return i
                                        }
                                    }
                                    return 0
                                }

                                onCurrentIndexChanged: {
                                    if (visible && currentIndex >= 0 && field.enum_options.length > 0) {
                                        field.value = field.enum_options[currentIndex].value
                                    }
                                }
                            }

                            ComboBox {
                                visible: field.type === "ForeignKey" || field.type === "OptionalForeignKey"
                                Layout.fillWidth: true

                                property var fkModel: {
                                    let tableKey = field.fk_info ? field.fk_info.source_table : ""
                                    if (root.fkData[tableKey]) {
                                        return root.fkData[tableKey]
                                    }
                                    return []
                                }

                                model: fkModel
                                textRole: "display"
                                valueRole: "value"

                                currentIndex: {
                                    let val = field.value
                                    for (let i = 0; i < fkModel.length; i++) {
                                        if (fkModel[i].value === val) {
                                            return i
                                        }
                                    }
                                    return 0
                                }

                                onCurrentIndexChanged: {
                                    if (visible && currentIndex >= 0 && currentIndex < fkModel.length) {
                                        field.value = fkModel[currentIndex].value
                                    }
                                }
                            }

                            TextField {
                                visible: field.type === "StringArray"
                                Layout.fillWidth: true
                                text: {
                                    let arr = field.value
                                    if (Array.isArray(arr)) return arr.join(", ")
                                    if (typeof arr === "string") return arr
                                    return ""
                                }
                                placeholderText: "Введите через запятую"
                                font.pixelSize: 13

                                onTextChanged: {
                                    if (visible) {
                                        let parts = text.split(",").map(s => s.trim()).filter(s => s.length > 0)
                                        field.value = parts
                                    }
                                }
                            }
                        }
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Item { Layout.fillWidth: true }

                Button {
                    text: "Отмена"
                    onClicked: {
                        root.cancelRequested()
                        root.close()
                    }
                }

                Button {
                    text: root.isEditMode ? "Сохранить" : "Создать"
                    highlighted: true
                    onClicked: {
                        // Валидация формы
                        let errors = root.validateForm()
                        if (errors.length > 0) {
                            errorDialog.errorText = errors.join("\n")
                            errorDialog.open()
                            return
                        }
                        
                        // Если валидация прошла, отправляем данные
                        root.saveRequested(root.getFormData())
                    }
                }
            }
        }
    }

    // Диалог для отображения ошибок валидации
    Dialog {
        id: errorDialog
        title: "Ошибка валидации"
        modal: true
        anchors.centerIn: parent
        width: Math.min(500, parent.width - 40)
        
        property string errorText: ""
        
        contentItem: Rectangle {
            color: "white"
            implicitHeight: errorText.implicitHeight + 40
            
            Text {
                id: errorText
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