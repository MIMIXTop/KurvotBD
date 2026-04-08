#pragma once

#include <QAbstractTableModel>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class ClientsTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum ClientRoles {
        ClientId = Qt::UserRole + 1,
        ClientName,
        ActiveProjects,
        TotalSpend,
        EarliestProjectDate,
        LatestProjectDate
    };

    explicit ClientsTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void loadFiltered(const QString &fromDate, const QString &toDate);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::ClientActivityResult> clients;
    std::shared_ptr<DataBase> database;
};
