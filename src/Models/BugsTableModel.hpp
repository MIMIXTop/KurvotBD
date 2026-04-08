#pragma once

#include <QAbstractTableModel>
#include <QStringList>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class BugsTableModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(QStringList projectList READ projectList NOTIFY projectListChanged)
    Q_PROPERTY(QStringList statusList READ statusList NOTIFY statusListChanged)
    Q_PROPERTY(QStringList severityList READ severityList NOTIFY severityListChanged)

public:
    enum BugRoles {
        BugId = Qt::UserRole + 1,
        ProjectName,
        Title,
        Description,
        Status,
        Severity,
        FoundBy,
        FixedBy,
        CreatedAt,
        FoundDate,
        FixedDate,
        TotalCount
    };
    Q_ENUM(BugRoles)

    explicit BugsTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList projectList() const { return m_projectList; }
    QStringList statusList() const { return m_statusList; }
    QStringList severityList() const { return m_severityList; }

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void applyFilters(
        int projectIndex,
        const QVariantList &statuses,
        const QVariantList &severities,
        const QString &foundFrom,
        const QString &foundTo
    );

signals:
    void projectListChanged();
    void statusListChanged();
    void severityListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::BugFilterResult> bugs;
    std::shared_ptr<DataBase> database;
    
    QStringList m_projectList;
    QStringList m_statusList;
    QStringList m_severityList;

    void updateProjectList();
    void updateStatusList();
    void updateSeverityList();
};
