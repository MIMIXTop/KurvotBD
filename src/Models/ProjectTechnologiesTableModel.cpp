#include "ProjectTechnologiesTableModel.hpp"
#include <future>

ProjectTechnologiesTableModel::ProjectTechnologiesTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
}

int ProjectTechnologiesTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(techData.size());
}

int ProjectTechnologiesTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 2; // TechName, ProjectCount
}

QVariant ProjectTechnologiesTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(techData.size()))
        return {};

    const auto &item = techData[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.tech_name);
            case 1: return static_cast<qint64>(item.project_count);
            default: return {};
        }
    }

    if (role >= TechName) {
        switch (role) {
            case TechName: return QString::fromStdString(item.tech_name);
            case ProjectCount: return static_cast<qint64>(item.project_count);
            default: return {};
        }
    }

    return {};
}

void ProjectTechnologiesTableModel::loadAll() {
    beginResetModel();
    techData = database->getProjectTechnologies();
    endResetModel();
}

void ProjectTechnologiesTableModel::applyFilters(bool isActive, const QString &techName) {
    std::optional<bool> optIsActive = std::optional<bool>(isActive);
    std::optional<std::string> optTechName = techName.isEmpty() ? std::nullopt : std::optional<std::string>(techName.toStdString());

    auto res = std::async([this, optIsActive, optTechName] {
        return database->getProjectTechnologies(optIsActive, optTechName);
    });

    beginResetModel();
    techData = res.get();
    endResetModel();
}

QHash<int, QByteArray> ProjectTechnologiesTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TechName] = "techName";
    roles[ProjectCount] = "projectCount";
    return roles;
}

void ProjectTechnologiesTableModel::updateTechList() {
    m_techList.clear();
    for (const auto &item : techData) {
        m_techList.append(QString::fromStdString(item.tech_name));
    }
    emit techListChanged();
}
