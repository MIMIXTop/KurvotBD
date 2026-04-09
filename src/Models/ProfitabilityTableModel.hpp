#pragma once

#include <QAbstractTableModel>
#include <QStringList>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class ProfitabilityTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum ProfitabilityRoles {
        ProjectId = Qt::UserRole + 1,
        ProjectName,
        TotalCosts,
        Budget,
        EfficiencyRatio
    };
    Q_ENUM(ProfitabilityRoles)

    explicit ProfitabilityTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void applyFilters(const QString &fromDate, const QString &toDate, bool isActive);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::ProjectProfitabilityResult> profitability;
    std::shared_ptr<DataBase> database;
};
