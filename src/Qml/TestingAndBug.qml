import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    Layout.fillWidth: true
    Layout.fillHeight: true

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            id: bar
            width: parent.width
            Layout.fillWidth: true
            TabButton {
                text: "Баги"
                width: implicitWidth
            }
            TabButton {
                text: "Релизы"
                width: implicitWidth
            }
            TabButton {
                text: "Эффективность QA"
                width: implicitWidth
            }
        }

        StackLayout {
            currentIndex: bar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Страница 0: Баги
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для багов
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Проект
                        ComboBox {
                            id: bugProjectCombo
                            width: 200
                            height: 32
                            model: bugsModel.projectList
                        }

                        // Статусы (множественный выбор через чекбоксы)
                        Column {
                            spacing: 5
                            Text { 
                                text: "Статус:"
                                font.bold: true
                                color: "#333333"
                            }
                            Row {
                                spacing: 5
                                CheckBox { 
                                    id: statusNew
                                    text: "новая"
                                    checked: false
                                    contentItem: Text {
                                        text: statusNew.text
                                        color: "#333333"
                                        leftPadding: statusNew.indicator.width + statusNew.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: statusInProgress
                                    text: "в_работе"
                                    checked: false
                                    contentItem: Text {
                                        text: statusInProgress.text
                                        color: "#333333"
                                        leftPadding: statusInProgress.indicator.width + statusInProgress.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: statusFixed
                                    text: "исправлена"
                                    checked: false
                                    contentItem: Text {
                                        text: statusFixed.text
                                        color: "#333333"
                                        leftPadding: statusFixed.indicator.width + statusFixed.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: statusVerified
                                    text: "проверена"
                                    checked: false
                                    contentItem: Text {
                                        text: statusVerified.text
                                        color: "#333333"
                                        leftPadding: statusVerified.indicator.width + statusVerified.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }

                        // Серьезность (множественный выбор)
                        Column {
                            spacing: 5
                            Text { 
                                text: "Серьезность:"
                                font.bold: true
                                color: "#333333"
                            }
                            Row {
                                spacing: 5
                                CheckBox { 
                                    id: severityBlocking
                                    text: "блокирующая"
                                    checked: false
                                    contentItem: Text {
                                        text: severityBlocking.text
                                        color: "#333333"
                                        leftPadding: severityBlocking.indicator.width + severityBlocking.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: severityCritical
                                    text: "критическая"
                                    checked: false
                                    contentItem: Text {
                                        text: severityCritical.text
                                        color: "#333333"
                                        leftPadding: severityCritical.indicator.width + severityCritical.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: severityMedium
                                    text: "средняя"
                                    checked: false
                                    contentItem: Text {
                                        text: severityMedium.text
                                        color: "#333333"
                                        leftPadding: severityMedium.indicator.width + severityMedium.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox { 
                                    id: severityLow
                                    text: "низкая"
                                    checked: false
                                    contentItem: Text {
                                        text: severityLow.text
                                        color: "#333333"
                                        leftPadding: severityLow.indicator.width + severityLow.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }

                        // Дата обнаружения от
                        TextField {
                            id: bugFoundFromField
                            width: 180
                            height: 32
                            placeholderText: "Найдена от (ГГГГ-ММ-ДД)"
                        }

                        // Дата обнаружения до
                        TextField {
                            id: bugFoundToField
                            width: 180
                            height: 32
                            placeholderText: "Найдена до (ГГГГ-ММ-ДД)"
                        }

                        // Кнопка поиска
                        Button {
                            text: "Найти"
                            height: 32
                            background: Rectangle {
                                color: "#2196F3"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                let statuses = []
                                if (statusNew.checked) statuses.push("новая")
                                if (statusInProgress.checked) statuses.push("в_работе")
                                if (statusFixed.checked) statuses.push("исправлена")
                                if (statusVerified.checked) statuses.push("проверена")

                                let severities = []
                                if (severityBlocking.checked) severities.push("блокирующая")
                                if (severityCritical.checked) severities.push("критическая")
                                if (severityMedium.checked) severities.push("средняя")
                                if (severityLow.checked) severities.push("низкая")

                                bugsModel.applyFilters(
                                    bugProjectCombo.currentIndex,
                                    statuses,
                                    severities,
                                    bugFoundFromField.text,
                                    bugFoundToField.text
                                )
                            }
                        }
                    }

                    // Таблица багов
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 0

                            HorizontalHeaderView {
                                id: bugsHeaderView
                                syncView: bugsTableView
                                Layout.fillWidth: true
                                model: ["Проект", "Название", "Описание", "Статус", "Серьезность", "Найден кем", "Исправлен кем", "Создан", "Дата обнаружения", "Дата исправления"]
                            }

                            TableView {
                                id: bugsTableView
                                resizableColumns: true
                                interactive: true
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                columnSpacing: 1
                                rowSpacing: 1
                                clip: true

                                columnWidthProvider: function (column) {
                                    let minWidths = [150, 200, 250, 120, 120, 200, 200, 150, 120, 120];
                                    return minWidths[column] || 100;
                                }

                                model: bugsModel

                                delegate: Rectangle {
                                    implicitHeight: 50
                                    border.width: 1
                                    color: row % 2 === 0 ? "white" : "#f9f9f9"

                                    MouseArea {
                                        anchors.fill: parent
                                        onDoubleClicked: {
                                            // Показать Alert с информацией о баг
                                            let projectName = model.projectName || ""
                                            let title = model.title || ""
                                            let description = model.description || ""
                                            let status = model.status || ""
                                            let severity = model.severity || ""
                                            let foundBy = model.foundBy || ""
                                            let fixedBy = model.fixedBy || ""
                                            let createdAt = model.createdAt || ""
                                            let foundDate = model.foundDate || ""
                                            let fixedDate = model.fixedDate || ""
                                            
                                            bugDialog.bugInfo = "Проект: " + projectName +
                                                "\nНазвание: " + title + 
                                                "\nОписание: " + description + 
                                                "\nСтатус: " + status + 
                                                "\nСерьезность: " + severity + 
                                                "\nНайден кем: " + foundBy + 
                                                "\nИсправлен кем: " + fixedBy + 
                                                "\nСоздан: " + createdAt + 
                                                "\nДата обнаружения: " + foundDate + 
                                                "\nДата исправления: " + fixedDate
                                            bugDialog.open()
                                        }
                                    }

                                    Text {
                                        anchors.fill: parent
                                        anchors.margins: 5
                                        text: {
                                            switch (column) {
                                                case 0: return model.projectName || ""
                                                case 1: return model.title || ""
                                                case 2: return model.description || ""
                                                case 3: return model.status || ""
                                                case 4: return model.severity || ""
                                                case 5: return model.foundBy || ""
                                                case 6: return model.fixedBy || ""
                                                case 7: return model.createdAt || ""
                                                case 8: return model.foundDate || ""
                                                case 9: return model.fixedDate || ""
                                                default: return ""
                                            }
                                        }
                                        elide: Text.ElideRight
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Страница 1: Релизы
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для релизов
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Дата от
                        TextField {
                            id: releaseFromDateField
                            width: 180
                            height: 32
                            placeholderText: "Дата от (ГГГГ-ММ-ДД)"
                        }

                        // Дата до
                        TextField {
                            id: releaseToDateField
                            width: 180
                            height: 32
                            placeholderText: "Дата до (ГГГГ-ММ-ДД)"
                        }

                        // Кнопка поиска
                        Button {
                            text: "Найти"
                            height: 32
                            background: Rectangle {
                                color: "#2196F3"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                releasesModel.loadFiltered(
                                    releaseFromDateField.text,
                                    releaseToDateField.text
                                )
                            }
                        }
                    }

                    // Таблица релизов
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 0

                            HorizontalHeaderView {
                                id: releasesHeaderView
                                syncView: releasesTableView
                                Layout.fillWidth: true
                                model: ["Проект", "Версия", "Дата релиза", "Changelog"]
                            }

                            TableView {
                                id: releasesTableView
                                resizableColumns: true
                                interactive: true
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                columnSpacing: 1
                                rowSpacing: 1
                                clip: true

                                columnWidthProvider: function (column) {
                                    let minWidths = [200, 120, 150, 400];
                                    return minWidths[column] || 100;
                                }

                                model: releasesModel

                                delegate: Rectangle {
                                    implicitHeight: 50
                                    border.width: 1
                                    color: row % 2 === 0 ? "white" : "#f9f9f9"

                                    Text {
                                        anchors.fill: parent
                                        anchors.margins: 5
                                        text: {
                                            switch (column) {
                                                case 0: return model.projectName || ""
                                                case 1: return model.version || ""
                                                case 2: return model.releaseDate || ""
                                                case 3: return model.changelog || ""
                                                default: return ""
                                            }
                                        }
                                        elide: Text.ElideRight
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }
                    }
                }
            }

            // Страница 2: Эффективность QA
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры для эффективности QA
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Проект
                        ComboBox {
                            id: efficiencyProjectCombo
                            width: 200
                            height: 32
                            model: testingEfficiencyModel.projectList
                        }

                        // Период от
                        TextField {
                            id: efficiencyFromDateField
                            width: 180
                            height: 32
                            placeholderText: "Период от (ГГГГ-ММ-ДД)"
                        }

                        // Период до
                        TextField {
                            id: efficiencyToDateField
                            width: 180
                            height: 32
                            placeholderText: "Период до (ГГГГ-ММ-ДД)"
                        }

                        // Кнопка поиска
                        Button {
                            text: "Найти"
                            height: 32
                            background: Rectangle {
                                color: "#2196F3"
                                radius: 4
                            }
                            contentItem: Text {
                                text: parent.text
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            onClicked: {
                                testingEfficiencyModel.loadFiltered(
                                    efficiencyProjectCombo.currentIndex,
                                    efficiencyFromDateField.text,
                                    efficiencyToDateField.text
                                )
                            }
                        }
                    }

                    // Таблица эффективности QA
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 5
                            spacing: 0

                            HorizontalHeaderView {
                                id: efficiencyHeaderView
                                syncView: efficiencyTableView
                                Layout.fillWidth: true
                                model: ["Проект", "Баги в тесте", "Баги после релиза", "Коэффициент эффективности"]
                            }

                            TableView {
                                id: efficiencyTableView
                                resizableColumns: true
                                interactive: true
                                Layout.fillWidth: true
                                Layout.fillHeight: true
                                columnSpacing: 1
                                rowSpacing: 1
                                clip: true

                                columnWidthProvider: function (column) {
                                    let minWidths = [250, 150, 180, 220];
                                    return minWidths[column] || 100;
                                }

                                model: testingEfficiencyModel

                                delegate: Rectangle {
                                    implicitHeight: 50
                                    border.width: 1
                                    color: row % 2 === 0 ? "white" : "#f9f9f9"

                                    Text {
                                        anchors.centerIn: parent
                                        text: {
                                            switch (column) {
                                                case 0: return model.projectName || ""
                                                case 1: return model.bugsInTest || 0
                                                case 2: return model.bugsAfterRelease || 0
                                                case 3: return Number(model.efficiencyRatio).toFixed(2)
                                                default: return ""
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Alert Dialog для отображения информации о баге
    Dialog {
        id: bugDialog
        title: "Информация о баге"
        modal: true
        anchors.centerIn: parent
        width: 500
        height: 400

        property string bugInfo: ""

        contentItem: Rectangle {
            color: "white"
            Text {
                anchors.fill: parent
                anchors.margins: 20
                text: bugDialog.bugInfo
                font.pixelSize: 14
                wrapMode: Text.WordWrap
            }
        }

        standardButtons: Dialog.Ok
    }
}
