#pragma once

#include <QAbstractTableModel>

#include "SqlFunctions.hpp"

class QObject;

class EmployeesTableModel : public QAbstractTableModel {

    Q_OBJECT

public:
    enum EmployeesTableRoles {
        NameRole = Qt::UserRole + 1,
        DepartmentName,
        Position,
        Age,
        Salary,
        ExperienceYears,
        Email,
        Phone
    };

    explicit EmployeesTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::EmployeeFilterResult> employees;
};

