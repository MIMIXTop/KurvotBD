import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import "Components/Tables"
import "Components/Dialogs"

Item {
    Layout.fillWidth: true
    Layout.fillHeight: true

    Component.onCompleted: {
        updateTeamFilters()
        updateDocFilters()
        updateSpecFilters()
    }

    function updateTeamFilters() {
        let roles = []
        if (roleDev.checked) roles.push("разработчик")
        if (roleTester.checked) roles.push("тестировщик")
        if (roleAnalyst.checked) roles.push("аналитик")
        if (roleManager.checked) roles.push("менеджер_проекта")
        if (roleArchitect.checked) roles.push("архитектор")
        if (roleTechWriter.checked) roles.push("технический_писатель")

        let positions = []
        if (teamPositionCombo.currentIndex > 0) {
            let positionText = teamPositionCombo.currentText
            if (positionText && positionText.length > 0) {
                positions.push(positionText)
            }
        }

        projectTeamModel.applyFilters(
            teamProjectCombo.currentIndex,
            roles,
            positions,
            teamActiveCheck.checked,
            0 // Default sort by Role
        )
    }

    function updateStatusReport() {
        let projectId = projectTeamModel.getProjectId(statusProjectCombo.currentIndex)
        console.log(projectId)

        if (projectId > 0 &&
            statusFromDate.selectedDate.length === 10 && 
            statusToDate.selectedDate.length === 10) {
            projectStatusModel.loadStatus(
                projectId,
                statusFromDate.toSqlFormat(statusFromDate.selectedDate),
                statusToDate.toSqlFormat(statusToDate.selectedDate)
            )
        }
    }

    function updateDocFilters() {
        let projectName = ""
        if (docProjectCombo.currentIndex > 0) {
            projectName = docProjectCombo.currentText
        }

        let docType = ""
        if (docTypeCombo.currentIndex > 0) {
            docType = docTypeCombo.currentText
        }

        documentationModel.applyFilters(projectName, docType)
    }

    function updateSpecFilters() {
        let projectName = ""
        if (specProjectCombo.currentIndex > 0) {
            projectName = specProjectCombo.currentText
        }

        specificationModel.applyFilters(projectName)
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TabBar {
            id: bar
            width: parent.width
            Layout.fillWidth: true
            TabButton {
                text: "Команда проекта"
                width: implicitWidth
            }
            TabButton {
                text: "Статус-отчёт"
                width: implicitWidth
            }
            TabButton {
                text: "Технологии"
                width: implicitWidth
            }
            TabButton {
                text: "Документация"
                width: implicitWidth
            }
            TabButton {
                text: "Спецификации"
                width: implicitWidth
            }
        }

        StackLayout {
            currentIndex: bar.currentIndex
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Страница 0: Команда проекта
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Проект
                        ComboBox {
                            id: teamProjectCombo
                            width: 200
                            height: 32
                            model: projectTeamModel.projectList
                            onCurrentIndexChanged: updateTeamFilters()
                        }

                        // Должности
                        ComboBox {
                            id: teamPositionCombo
                            width: 200
                            height: 32
                            model:projectTeamModel.positionList
                            onCurrentIndexChanged: updateTeamFilters()
                        }

                        // Роли
                        Column {
                            spacing: 5
                            Text {
                                text: "Роли:"
                                font.bold: true
                                color: "#333333"
                            }
                            Row {
                                spacing: 5
                                CheckBox {
                                    id: roleDev
                                    text: "разработчик"
                                    onCheckedChanged: updateTeamFilters()
                                    contentItem: Text {
                                        text: roleDev.text
                                        color: "#333333"
                                        leftPadding: roleDev.indicator.width + roleDev.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox {
                                    id: roleTester
                                    text: "тестировщик"
                                    onCheckedChanged: updateTeamFilters()
                                    contentItem: Text {
                                        text: roleTester.text
                                        color: "#333333"
                                        leftPadding: roleTester.indicator.width + roleTester.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox {
                                    id: roleAnalyst
                                    text: "аналитик"
                                    onCheckedChanged: updateTeamFilters()
                                    contentItem: Text {
                                        text: roleAnalyst.text
                                        color: "#333333"
                                        leftPadding: roleAnalyst.indicator.width + roleAnalyst.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox {
                                    id: roleManager
                                    text: "менеджер_проекта"
                                    onCheckedChanged: updateTeamFilters()
                                    contentItem: Text {
                                        text: roleManager.text
                                        color: "#333333"
                                        leftPadding: roleManager.indicator.width + roleManager.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox {
                                    id: roleArchitect
                                    text: "архитектор"
                                    onCheckedChanged: updateTeamFilters()
                                    contentItem: Text {
                                        text: roleArchitect.text
                                        color: "#333333"
                                        leftPadding: roleArchitect.indicator.width + roleArchitect.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                                CheckBox {
                                    id: roleTechWriter
                                    text: "технический_писатель"
                                    onCheckedChanged: updateTeamFilters()
                                    contentItem: Text {
                                        text: roleTechWriter.text
                                        color: "#333333"
                                        leftPadding: roleTechWriter.indicator.width + roleTechWriter.spacing
                                        verticalAlignment: Text.AlignVCenter
                                    }
                                }
                            }
                        }

                        // Только активные
                        CheckBox {
                            id: teamActiveCheck
                            text: "Только активные"
                            checked: true
                            onCheckedChanged: updateTeamFilters()
                            contentItem: Text {
                                text: teamActiveCheck.text
                                color: "#333333"
                                leftPadding: teamActiveCheck.indicator.width + teamActiveCheck.spacing
                                verticalAlignment: Text.AlignVCenter
                            }
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
                            onClicked: updateTeamFilters()
                        }
                    }

                    // Таблица команды
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        TeamTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: projectTeamModel
                            onItemDoubleClicked: function(info) {
                                teamDialog.teamInfo = info
                                teamDialog.open()
                            }
                        }
                    }
                }
            }

            // Страница 1: Статус-отчёт
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Проект
                        ComboBox {
                            id: statusProjectCombo
                            width: 200
                            height: 32
                            model: projectTeamModel.projectList
                            
                            onCurrentIndexChanged: updateStatusReport()
                        }

                        // Дата от
                        DatePicker {
                            id: statusFromDate
                            selectedDate: "01.01.2024"
                            placeholderText: "Дата от (ДД.ММ.ГГГГ)"
                            onSelectedDateChanged: updateStatusReport()
                        }

                        // Дата до
                        DatePicker {
                            id: statusToDate
                            selectedDate: "31.12.2026"
                            placeholderText: "Дата до (ДД.ММ.ГГГГ)"
                            onSelectedDateChanged: updateStatusReport()
                        }

                        // Кнопка поиска
                        Button {
                            text: "Показать отчёт"
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
                            onClicked: updateStatusReport()
                        }
                    }

                    // Карточки с метриками
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 20
                            spacing: 15

                            // Заголовок
                            Text {
                                text: projectStatusModel.hasData ?
                                    "Статус-отчёт: " + projectStatusModel.projectName :
                                    "Выберите проект и период для отображения отчёта"
                                font.pixelSize: 18
                                font.bold: true
                                color: "#333333"
                            }

                            // Карточки метрик
                            GridLayout {
                                columns: 3
                                rowSpacing: 15
                                columnSpacing: 15
                                Layout.fillWidth: true

                                // Карточка 1: Текущая фаза
                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 200
                                    Layout.preferredHeight: 100
                                    color: "#e3f2fd"
                                    radius: 8

                                    ColumnLayout {
                                        anchors.centerIn: parent
                                        spacing: 5
                                        Text {
                                            text: "Текущая фаза"
                                            font.pixelSize: 14
                                            color: "#1565c0"
                                            font.bold: true
                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                        Text {
                                            text: projectStatusModel.currentPhase || "-"
                                            font.pixelSize: 20
                                            color: "#0d47a1"
                                            font.bold: true
                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                    }
                                }

                                // Карточка 2: Задач выполнено
                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 200
                                    Layout.preferredHeight: 100
                                    color: "#e8f5e9"
                                    radius: 8

                                    ColumnLayout {
                                        anchors.centerIn: parent
                                        spacing: 5
                                        Text {
                                            text: "Задач выполнено"
                                            font.pixelSize: 14
                                            color: "#2e7d32"
                                            font.bold: true
                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                        Text {
                                            text: projectStatusModel.tasksLogged.toString()
                                            font.pixelSize: 20
                                            color: "#1b5e20"
                                            font.bold: true
                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                    }
                                }

                                // Карточка 3: Критических рисков
                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 200
                                    Layout.preferredHeight: 100
                                    color: projectStatusModel.criticalRisksCount > 3 ? "#ffebee" : "#fff3e0"
                                    radius: 8

                                    ColumnLayout {
                                        anchors.centerIn: parent
                                        spacing: 5
                                        Text {
                                            text: "Критических рисков"
                                            font.pixelSize: 14
                                            color: projectStatusModel.criticalRisksCount > 3 ? "#c62828" : "#e65100"
                                            font.bold: true
                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                        Text {
                                            text: projectStatusModel.criticalRisksCount.toString()
                                            font.pixelSize: 20
                                            color: projectStatusModel.criticalRisksCount > 3 ? "#b71c1c" : "#bf360c"
                                            font.bold: true
                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                    }
                                }

                                // Карточка 4: Часов потрачено
                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 200
                                    Layout.preferredHeight: 100
                                    color: "#f3e5f5"
                                    radius: 8

                                    ColumnLayout {
                                        anchors.centerIn: parent
                                        spacing: 5
                                        Text {
                                            text: "Часов потрачено"
                                            font.pixelSize: 14
                                            color: "#6a1b9a"
                                            font.bold: true
                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                        Text {
                                            text: Number(projectStatusModel.teamHoursSpent).toFixed(1)
                                            font.pixelSize: 20
                                            color: "#4a148c"
                                            font.bold: true
                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                    }
                                }

                                // Карточка 5: Период отчёта
                                Rectangle {
                                    Layout.fillWidth: true
                                    Layout.minimumWidth: 200
                                    Layout.preferredHeight: 100
                                    color: "#fafafa"
                                    radius: 8
                                    border.color: "#e0e0e0"
                                    border.width: 1

                                    ColumnLayout {
                                        anchors.centerIn: parent
                                        spacing: 5
                                        Text {
                                            text: "Период отчёта"
                                            font.pixelSize: 14
                                            color: "#616161"
                                            font.bold: true
                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                        Text {
                                            text: projectStatusModel.reportPeriod || "-"
                                            font.pixelSize: 14
                                            color: "#212121"
                                            font.bold: true
                                            Layout.alignment: Qt.AlignHCenter
                                        }
                                    }
                                }
                            }

                            Item {
                                Layout.fillHeight: true
                            }
                        }
                    }
                }
            }

            // Страница 2: Технологии
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Поиск технологии
                        TextField {
                            id: techSearchField
                            width: 200
                            height: 32
                            placeholderText: "Поиск технологии"
                            onTextChanged: technologiesModel.applyFilters(techActiveCheck.checked, text)
                        }

                        // Только активные проекты
                        CheckBox {
                            id: techActiveCheck
                            text: "Только активные проекты"
                            checked: true
                            onCheckedChanged: technologiesModel.applyFilters(checked, techSearchField.text)
                            contentItem: Text {
                                text: techActiveCheck.text
                                color: "#333333"
                                leftPadding: techActiveCheck.indicator.width + techActiveCheck.spacing
                                verticalAlignment: Text.AlignVCenter
                            }
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
                                technologiesModel.applyFilters(
                                    techActiveCheck.checked,
                                    techSearchField.text
                                )
                            }
                        }
                    }

                    // Таблица технологий
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        TechnologiesTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: technologiesModel
                            onItemDoubleClicked: function(info) {
                                techDialog.techInfo = info
                                techDialog.open()
                            }
                        }
                    }
                }
            }

            // Страница 3: Документация
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Проект
                        ComboBox {
                            id: docProjectCombo
                            width: 200
                            height: 32
                            model: ["Все проекты"].concat(documentationModel.projectList)
                            onCurrentIndexChanged: updateDocFilters()
                        }

                        // Тип документации
                        ComboBox {
                            id: docTypeCombo
                            width: 200
                            height: 32
                            model: ["Все типы"].concat(documentationModel.docTypeList)
                            onCurrentIndexChanged: updateDocFilters()
                        }

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
                            onClicked: updateDocFilters()
                        }
                    }

                    // Таблица документации
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        DocumentationTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: documentationModel
                            onItemDoubleClicked: function(info, content) {
                                docDialog.docInfo = info
                                docDialog.docContent = content
                                docDialog.open()
                            }
                        }
                    }
                }
            }

            // Страница 4: Спецификации
            Item {
                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    // Фильтры
                    Flow {
                        Layout.fillWidth: true
                        Layout.margins: 10
                        spacing: 10
                        Layout.preferredHeight: implicitHeight

                        // Проект
                        ComboBox {
                            id: specProjectCombo
                            width: 200
                            height: 32
                            model: ["Все проекты"].concat(specificationModel.projectList)
                            onCurrentIndexChanged: updateSpecFilters()
                        }

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
                            onClicked: updateSpecFilters()
                        }
                    }

                    // Таблица спецификаций
                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        color: "#f5f5f5"
                        radius: 8
                        Layout.margins: 5

                        SpecificationTable {
                            anchors.fill: parent
                            anchors.margins: 5
                            tableModel: specificationModel
                            onItemDoubleClicked: function(info, content) {
                                specDialog.specInfo = info
                                specDialog.specContent = content
                                specDialog.open()
                            }
                        }
                    }
                }
            }
        }
    }

    // Dialog для команды
    TeamDialog {
        id: teamDialog
    }

    // Dialog для технологий
    TechDialog {
        id: techDialog
    }

    // Dialog для документации
    DocDialog {
        id: docDialog
    }

    // Dialog для спецификаций
    SpecDialog {
        id: specDialog
    }
}
