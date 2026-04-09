#include "InfrastructureTableModel.hpp"
#include <future>

InfrastructureTableModel::InfrastructureTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
    updateProjectList();
}

int InfrastructureTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(infrastructure.size());
}

int InfrastructureTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 6; // ProjectName, LicenseName, ExpiryDate, Cost, ResourceProvider, ResourceCostPerHour
}

QVariant InfrastructureTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(infrastructure.size()))
        return {};

    const auto &item = infrastructure[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.project_name);
            case 1: return QString::fromStdString(item.license_name);
            case 2: return QString::fromStdString(item.expiry_date);
            case 3: return item.cost;
            case 4: return QString::fromStdString(item.resource_provider);
            case 5: return item.resource_cost_per_hour;
            default: return {};
        }
    }

    if (role >= ProjectId) {
        switch (role) {
            case ProjectId: return item.project_id;
            case ProjectName: return QString::fromStdString(item.project_name);
            case LicenseName: return QString::fromStdString(item.license_name);
            case ExpiryDate: return QString::fromStdString(item.expiry_date);
            case Cost: return item.cost;
            case ResourceProvider: return QString::fromStdString(item.resource_provider);
            case ResourceCostPerHour: return item.resource_cost_per_hour;
            default: return {};
        }
    }

    return {};
}

void InfrastructureTableModel::loadAll() {
    beginResetModel();
    infrastructure = database->getInfrastructureReport();
    endResetModel();
}

void InfrastructureTableModel::applyFilters(bool isActive, int projectIndex) {
    std::optional<bool> optIsActive = isActive ? std::optional<bool>(true) : std::nullopt;
    std::optional<int> optProjectId;
    if (projectIndex > 0) {
        optProjectId = projectIndex; // index 0 = "Все проекты", index 1+ = project_id
    }

    auto res = std::async([this, optIsActive, optProjectId] {
        return database->getInfrastructureReport(optIsActive, optProjectId);
    });

    beginResetModel();
    infrastructure = res.get();
    endResetModel();
}

QHash<int, QByteArray> InfrastructureTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ProjectId] = "projectId";
    roles[ProjectName] = "projectName";
    roles[LicenseName] = "licenseName";
    roles[ExpiryDate] = "expiryDate";
    roles[Cost] = "cost";
    roles[ResourceProvider] = "resourceProvider";
    roles[ResourceCostPerHour] = "resourceCostPerHour";
    return roles;
}

void InfrastructureTableModel::updateProjectList() {
    m_projectList = {"Все проекты"};

    auto res = std::async([this] {
        return database->selectAll(lib::Models::Project{});
    });

    auto projects = res.get();
    for (const auto &variant : projects) {
        std::visit([this](const auto &model) {
            using T = std::decay_t<decltype(model)>;
            if constexpr (std::is_same_v<T, lib::Models::Project>) {
                m_projectList.append(QString::fromStdString(model.name));
            }
        }, variant);
    }

    emit projectListChanged();
}
