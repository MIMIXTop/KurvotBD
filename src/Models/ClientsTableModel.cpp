#include "ClientsTableModel.hpp"
#include <future>

ClientsTableModel::ClientsTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
    loadAll();
}

int ClientsTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(clients.size());
}

int ClientsTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return 5; // 5 колонок: Клиент, Активные проекты, Общие затраты, Первый проект, Последний проект
}

QVariant ClientsTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(clients.size()))
        return {};

    const auto &item = clients[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.client_name);
            case 1: return static_cast<int>(item.active_projects);
            case 2: return QString::number(item.total_spend, 'f', 2);
            case 3: return QString::fromStdString(item.earliest_project_date);
            case 4: return QString::fromStdString(item.latest_project_date);
            default: return {};
        }
    }

    if (role >= ClientId) {
        switch (role) {
            case ClientId: return item.client_id;
            case ClientName: return QString::fromStdString(item.client_name);
            case ActiveProjects: return static_cast<int>(item.active_projects);
            case TotalSpend: return item.total_spend;
            case EarliestProjectDate: return QString::fromStdString(item.earliest_project_date);
            case LatestProjectDate: return QString::fromStdString(item.latest_project_date);
            default: return {};
        }
    }

    return {};
}

void ClientsTableModel::loadAll() {
    beginResetModel();
    clients = database->getClientActivity();
    endResetModel();
}

void ClientsTableModel::loadFiltered(const QString &fromDate, const QString &toDate) {
    std::optional<std::string> optFrom = fromDate.isEmpty() ? std::nullopt : std::optional<std::string>(fromDate.toStdString());
    std::optional<std::string> optTo = toDate.isEmpty() ? std::nullopt : std::optional<std::string>(toDate.toStdString());

    auto res = std::async([this, optFrom, optTo] {
        return database->getClientActivity(optFrom, optTo);
    });

    beginResetModel();
    clients = res.get();
    endResetModel();
}

QHash<int, QByteArray> ClientsTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ClientId] = "clientId";
    roles[ClientName] = "clientName";
    roles[ActiveProjects] = "activeProjects";
    roles[TotalSpend] = "totalSpend";
    roles[EarliestProjectDate] = "earliestProjectDate";
    roles[LatestProjectDate] = "latestProjectDate";
    return roles;
}
