#include "ProfitabilityTableModel.hpp"
#include <future>

ProfitabilityTableModel::ProfitabilityTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
}

int ProfitabilityTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(profitability.size());
}

int ProfitabilityTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 4; // ProjectName, TotalCosts, Budget, EfficiencyRatio
}

QVariant ProfitabilityTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(profitability.size()))
        return {};

    const auto &item = profitability[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.project_name);
            case 1: return item.total_costs;
            case 2: return item.budget;
            case 3: return item.efficiency_ratio;
            default: return {};
        }
    }

    if (role >= ProjectId) {
        switch (role) {
            case ProjectId: return item.project_id;
            case ProjectName: return QString::fromStdString(item.project_name);
            case TotalCosts: return item.total_costs;
            case Budget: return item.budget;
            case EfficiencyRatio: return item.efficiency_ratio;
            default: return {};
        }
    }

    return {};
}

void ProfitabilityTableModel::loadAll() {
    beginResetModel();
    profitability = database->getProjectProfitability();
    endResetModel();
}

void ProfitabilityTableModel::applyFilters(const QString &fromDate, const QString &toDate, bool isActive) {
    std::optional<std::string> optFromDate = fromDate.isEmpty() ? std::nullopt : std::optional<std::string>(fromDate.toStdString());
    std::optional<std::string> optToDate = toDate.isEmpty() ? std::nullopt : std::optional<std::string>(toDate.toStdString());
    std::optional<bool> optIsActive = std::optional<bool>(isActive);

    auto res = std::async([this, optFromDate, optToDate, optIsActive] {
        return database->getProjectProfitability(optFromDate, optToDate, optIsActive);
    });

    beginResetModel();
    profitability = res.get();
    endResetModel();
}

QHash<int, QByteArray> ProfitabilityTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ProjectId] = "projectId";
    roles[ProjectName] = "projectName";
    roles[TotalCosts] = "totalCosts";
    roles[Budget] = "budget";
    roles[EfficiencyRatio] = "efficiencyRatio";
    return roles;
}
