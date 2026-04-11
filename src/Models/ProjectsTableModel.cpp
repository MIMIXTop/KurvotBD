#include "ProjectsTableModel.hpp"
#include <future>
#include <ranges>

ProjectsTableModel::ProjectsTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
    updateStatusList();
    updateMethodologyList();
    updateClientTypeList();
    loadAll();
}

int ProjectsTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(projects.size());
}

int ProjectsTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return 10; // 10 колонок
}

QVariant ProjectsTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(projects.size()))
        return {};

    const auto &item = projects[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.project_name);
            case 1: return QString::fromStdString(item.client_name);
            case 2: return QString::fromStdString(item.client_type);
            case 3: return QString::fromStdString(item.status);
            case 4: return QString::fromStdString(item.methodology);
            case 5: return QString::number(item.budget, 'f', 2);
            case 6: return QString::fromStdString(item.type);
            case 7: return QString::fromStdString(item.start_date);
            case 8: return QString::fromStdString(item.planned_end_date);
            case 9: return QString::fromStdString(item.actual_end_date);
            default: return {};
        }
    }

    if (role >= ProjectId) {
        switch (role) {
            case ProjectId: return item.project_id;
            case ClientId: return item.client_id;
            case ProjectName: return QString::fromStdString(item.project_name);
            case ClientName: return QString::fromStdString(item.client_name);
            case ClientType: return QString::fromStdString(item.client_type);
            case Status: return QString::fromStdString(item.status);
            case Type: return QString::fromStdString(item.type);
            case Methodology: return QString::fromStdString(item.methodology);
            case StartDate: return QString::fromStdString(item.start_date);
            case PlannedEndDate: return QString::fromStdString(item.planned_end_date);
            case ActualEndDate: return QString::fromStdString(item.actual_end_date);
            case Budget: return item.budget;
            default: return {};
        }
    }

    return {};
}

void ProjectsTableModel::loadAll() {
    beginResetModel();
    projects = database->getProjectsByFilters();
    endResetModel();
}

void ProjectsTableModel::applyFilters(
    const QVariantList &statuses,
    const QString &completedFrom,
    const QString &completedTo,
    const QVariantList &clientTypes,
    int methodologyIndex,
    double minBudget,
    double maxBudget,
    bool isActive)
{
    std::vector<std::string> statusVec;
    for (const auto &v : statuses) {
        statusVec.push_back(v.toString().toStdString());
    }

    std::optional<std::string> optCompletedFrom = completedFrom.isEmpty() ? std::nullopt : std::optional<std::string>(completedFrom.toStdString());
    std::optional<std::string> optCompletedTo = completedTo.isEmpty() ? std::nullopt : std::optional<std::string>(completedTo.toStdString());
    
    std::optional<int> optClientId = std::nullopt; // Пока не используем выбор конкретного клиента
    
    std::optional<std::string> optMethodology;
    if (methodologyIndex > 0 && methodologyIndex <= m_methodologyList.size()) {
        optMethodology = m_methodologyList[methodologyIndex].toStdString();
    }

    std::optional<std::vector<std::string>> clientTypeVec;
    if (!clientTypes.isEmpty()) {
        std::vector<std::string> types;
        for (const auto &v : clientTypes) {
            types.push_back(v.toString().toStdString());
        }
        clientTypeVec = types;
    }

    std::optional<double> optMinBudget = (minBudget >= 0) ? std::optional<double>(minBudget) : std::nullopt;
    std::optional<double> optMaxBudget = (maxBudget >= 0) ? std::optional<double>(maxBudget) : std::nullopt;
    std::optional<bool> optIsActive = isActive;

    auto res = std::async([this, statusVec, optCompletedFrom, optCompletedTo, optClientId, optMethodology, clientTypeVec, optMinBudget, optMaxBudget, optIsActive] {
        return database->getProjectsByFilters(statusVec, optCompletedFrom, optCompletedTo, optClientId, optMethodology, clientTypeVec, optMinBudget, optMaxBudget, optIsActive);
    });

    beginResetModel();
    projects = res.get();
    endResetModel();
}

QHash<int, QByteArray> ProjectsTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ProjectId] = "projectId";
    roles[ClientId] = "clientId";
    roles[ProjectName] = "projectName";
    roles[ClientName] = "clientName";
    roles[ClientType] = "clientType";
    roles[Status] = "status";
    roles[Type] = "projectType";
    roles[Methodology] = "methodology";
    roles[StartDate] = "startDate";
    roles[PlannedEndDate] = "plannedEndDate";
    roles[ActualEndDate] = "actualEndDate";
    roles[Budget] = "budget";
    return roles;
}

void ProjectsTableModel::updateStatusList() {
    m_statusList = {"Все", "в_разработке", "завершен", "приостановлен", "отменен"};
    emit statusListChanged();
}

void ProjectsTableModel::updateMethodologyList() {
    m_methodologyList = {"Все", "Scrum", "Kanban", "Waterfall"};
    emit methodologyListChanged();
}

void ProjectsTableModel::updateClientTypeList() {
    m_clientTypeList = {"Все", "корпоративный", "государственный", "частный"};
    emit clientTypeListChanged();
}
