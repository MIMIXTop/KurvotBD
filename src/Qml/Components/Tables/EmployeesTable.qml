import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root
    spacing: 0
    
    property var tableModel
    
    HorizontalHeaderView {
        id: horizontalHeader
        syncView: tableView
        Layout.fillWidth: true
        model: ["Имя", "Отдел", "Должность", "Возраст", "Зарплата", "Опыт", "Email", "Телефон", "Специализация"]
    }

    TableView {
        id: tableView
        resizableColumns: true
        interactive: true
        Layout.fillWidth: true
        Layout.fillHeight: true
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        columnWidthProvider: function (column) {
            let minWidths = [250, 180, 150, 50, 120, 50, 180, 100, 250];
            return minWidths[column] || 100;
        }

        model: root.tableModel

        delegate: Rectangle {
            id: tile
            implicitHeight: 50
            border.width: 1

            Text {
                anchors.fill: parent
                anchors.margins: 5
                text: {
                    switch (column) {
                        case 0: return model.employeeName || ""
                        case 1: return model.departmentName || ""
                        case 2: return model.positionTitle || ""
                        case 3: return model.age || ""
                        case 4: return Number(model.salary || 0).toLocaleString(Qt.locale("ru_RU"), 'f', 2)
                        case 5: return model.yearsOfExperience || ""
                        case 6: return model.email || ""
                        case 7: return model.phoneNumber || ""
                        case 8:
                            if (model.backendExp || model.frontendExp || model.mobileExp) {
                                let parts = []
                                if (model.programmingLanguages && model.programmingLanguages.length > 0) {
                                    parts.push(model.programmingLanguages.join(", "))
                                }
                                if (model.backendExp) parts.push("Backend")
                                if (model.frontendExp) parts.push("Frontend")
                                if (model.mobileExp) parts.push("Mobile")
                                return parts.join(", ")
                            }
                            if (model.testingTypes && model.testingTypes.length > 0) {
                                return model.testingTypes.join(", ")
                            }
                            if (model.managerCertType) {
                                return "Менеджер: " + model.managerCertType
                            }
                            return ""
                        default: return ""
                    }
                }
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
