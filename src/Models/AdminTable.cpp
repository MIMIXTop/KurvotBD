//
// Created by mimixtop on 12.04.2026.
//

#include "AdminTable.hpp"

#include <future>
#include <iostream>
#include <QDebug>

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
