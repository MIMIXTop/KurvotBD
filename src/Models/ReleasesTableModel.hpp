#pragma once

#include <QAbstractTableModel>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class ReleasesTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    enum ReleaseRoles {
        ProjectName = Qt::UserRole + 1,
        Version,
        ReleaseDate,
        Changelog
    };

    explicit ReleasesTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void loadFiltered(const QString &fromDate, const QString &toDate);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::ReleaseReportResult> releases;
    std::shared_ptr<DataBase> database;
};
