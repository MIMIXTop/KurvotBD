//
// Created by mimixtop on 12.04.2026.
//

#include "AdminTable.hpp"

#include <future>
#include <iostream>
#include <QDebug>

#include "FormMeta.hpp"
#include "util/util.hpp"

int AdminTable::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_models.size());
}

int AdminTable::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;

    if (!m_models.empty()) {
        return static_cast<int>(std::visit(lib::util::FieldCounter{}, m_models.front()));
    }
    
    return static_cast<int>(m_headerList.size());
}

QVariant AdminTable::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_models.size()) {
        return {};
    }

    auto &&item = m_models[index.row()];
    if (role == Qt::DisplayRole) {
        return std::visit([&](auto &&model) {
                              return lib::util::getFieldAsQVariant(model, index.column());
                          }
                          , item);
    }
    return {};
}

QVariant AdminTable::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole) {
        return {};
    }

    if (orientation == Qt::Horizontal) {
        if (section >= 0 && section < m_headerList.size()) {
            return m_headerList[section];
        }
        return QString("Поле %1").arg(section + 1);
    }

    return {};
}

void AdminTable::loadTableData() {
    if (m_currentTableName.isEmpty()) {
        return;
    }

    beginResetModel();
    m_models.clear();

    auto modelType = lib::util::createModelFromTableName(m_currentTableName.toStdString());
    m_models = database->selectAll(modelType);
    qDebug() << "Loaded" << m_models.size() << "records for table:" << m_currentTableName;

    updateHeaderList();
    endResetModel();
}

QHash<int, QByteArray> AdminTable::roleNames() const {
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles[Qt::DisplayRole] = "display";
    return roles;
}

void AdminTable::updateHeaderList() {

    auto res = database->getTableField(m_currentTableName.toStdString());

    m_headerList.clear();

    for (auto&& headerName: res) {
        m_headerList << QString::fromStdString(headerName);
    }
    emit headerListChanged();
}

void AdminTable::updateTableNameList() {
    auto res = database->getTablesNames();

    m_tableNameList.clear();

    for (auto&& tableName : res) {
        m_tableNameList << QString::fromStdString(tableName);
    }
    emit tableNameListChanged();
}

QVariantMap AdminTable::getRowData(int row) {
    QVariantMap result;

    if (row < 0 || row >= m_models.size()) {
        return result;
    }

    auto &&item = m_models[row];

    // 1. Маппинг имени таблицы в имя модели (так же, как в FormHelper)
    static const QMap<QString, QString> tableToModelMapping = {
        {"department", "Department"},
        {"position", "Position"},
        {"client", "Client"},
        {"software_license", "SoftwareLicense"},
        {"softwarelicense", "SoftwareLicense"},
        {"employee", "Employee"},
        {"developer_specialization", "DeveloperSpecialization"},
        {"developerspecialization", "DeveloperSpecialization"},
        {"tester_specialization", "TesterSpecialization"},
        {"testerspecialization", "TesterSpecialization"},
        {"manager_certification", "ManagerCertification"},
        {"managercertification", "ManagerCertification"},
        {"project", "Project"},
        {"project_phase", "ProjectPhase"},
        {"projectphase", "ProjectPhase"},
        {"project_assignment", "ProjectAssignment"},
        {"projectassignment", "ProjectAssignment"},
        {"license_allocation", "LicenseAllocation"},
        {"licenseallocation", "LicenseAllocation"},
        {"cloudresource", "CloudResource"},
        {"cloud_resource", "CloudResource"},
        {"worklog", "WorkLog"},
        {"work_log", "WorkLog"},
        {"bug", "Bug"},
        {"release", "Release"},
        {"documentation", "Documentation"},
        {"project_specification", "ProjectSpecification"},
        {"projectspecification", "ProjectSpecification"}
    };

    QString modelName = tableToModelMapping.value(m_currentTableName, m_currentTableName);

    // 2. Получаем метаданные полей из FormMeta
    auto formDef = lib::FormMeta::getFormDefinition(modelName.toStdString());

    // 3. Заполняем данными, используя field.name в качестве ключей
    for (size_t col = 0; col < formDef.fields.size(); ++col) {
        // Безопасность: проверяем, что индекс не превышает количество колонок в модели
        if (col >= static_cast<size_t>(columnCount())) {
            break;
        }

        const auto& field = formDef.fields[col];
        QString fieldName = QString::fromStdString(field.name); // Ключ: "name", "type" и т.д.

        QVariant fieldValue = std::visit([&](auto &&model) {
            return lib::util::getFieldAsQVariant(model, static_cast<int>(col));
        }, item);
        
        result[fieldName] = fieldValue;
    }
    
    return result;
}
