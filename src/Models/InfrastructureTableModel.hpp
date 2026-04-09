#pragma once

#include <QAbstractTableModel>
#include <QStringList>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class InfrastructureTableModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(QStringList projectList READ projectList NOTIFY projectListChanged)

public:
    enum InfrastructureRoles {
        ProjectId = Qt::UserRole + 1,
        ProjectName,
        LicenseName,
        ExpiryDate,
        Cost,
        ResourceProvider,
        ResourceCostPerHour
    };
    Q_ENUM(InfrastructureRoles)

    explicit InfrastructureTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList projectList() const { return m_projectList; }

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void applyFilters(bool isActive, int projectIndex);

signals:
    void projectListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::InfrastructureReportResult> infrastructure;
    std::shared_ptr<DataBase> database;
    
    QStringList m_projectList;

    void updateProjectList();
};
