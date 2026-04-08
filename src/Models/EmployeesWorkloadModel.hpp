#pragma once

#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class EmployeesWorkloadModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(QStringList employeeList READ employeeList NOTIFY employeeListChanged)
    Q_PROPERTY(QStringList projectList READ projectList NOTIFY projectListChanged)

public:
    enum WorkloadRoles {
        EmployeeId = Qt::UserRole + 1,
        EmployeeName,
        ProjectId,
        ProjectName,
        TotalHours,
        TaskCount
    };

    explicit EmployeesWorkloadModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList employeeList() const { return m_employeeList; }
    QStringList projectList() const { return m_projectList; }

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void loadByEmployees(const QVariantList &employeeIds);
    Q_INVOKABLE void loadFiltered(
        const QVariantList &employeeIds,
        const QVariantList &projectIds,
        const QString &fromDate,
        const QString &toDate
    );

signals:
    void employeeListChanged();
    void projectListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::EmployeeWorkloadResult> workloadData;
    QStringList m_employeeList;
    QStringList m_projectList;
    std::shared_ptr<DataBase> database;

    void updateEmployeeList();
    void updateProjectList();
};