#pragma once

#include <QAbstractTableModel>
#include <QStringList>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class ProjectsTableModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(QStringList statusList READ statusList NOTIFY statusListChanged)
    Q_PROPERTY(QStringList methodologyList READ methodologyList NOTIFY methodologyListChanged)
    Q_PROPERTY(QStringList clientTypeList READ clientTypeList NOTIFY clientTypeListChanged)

public:
    enum ProjectRoles {
        ProjectId = Qt::UserRole + 1,
        ClientId,
        ProjectName,
        ClientName,
        ClientType,
        Status,
        Type,
        Methodology,
        StartDate,
        PlannedEndDate,
        ActualEndDate,
        Budget
    };

    explicit ProjectsTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList statusList() const { return m_statusList; }
    QStringList methodologyList() const { return m_methodologyList; }
    QStringList clientTypeList() const { return m_clientTypeList; }

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void applyFilters(
        const QVariantList &statuses,
        const QString &completedFrom,
        const QString &completedTo,
        const QVariantList &clientTypes,
        int methodologyIndex,
        double minBudget,
        double maxBudget,
        bool isActive
    );

signals:
    void statusListChanged();
    void methodologyListChanged();
    void clientTypeListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::ProjectFilterResult> projects;
    std::shared_ptr<DataBase> database;
    
    QStringList m_statusList;
    QStringList m_methodologyList;
    QStringList m_clientTypeList;

    void updateStatusList();
    void updateMethodologyList();
    void updateClientTypeList();
};
