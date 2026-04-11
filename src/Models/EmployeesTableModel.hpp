#pragma once

#include <QAbstractTableModel>

#include "SqlFunctions.hpp"
#include  "lib/src/DataBase.hpp"

class QObject;

class EmployeesTableModel : public QAbstractTableModel {

    Q_OBJECT

    Q_PROPERTY(QStringList departmentList READ departmentList NOTIFY departmentListChanged)
    Q_PROPERTY(QStringList positionList READ positionList NOTIFY positionListChanged)

public:
    enum EmployeesTableRoles {
        NameRole = Qt::UserRole + 1,
        DepartmentName,
        Position,
        Age,
        Salary,
        ExperienceYears,
        Email,
        Phone,
        CertType,
        CertIssueDate,
        ProgrammingLanguages,
        Frameworks,
        BackendExp,
        FrontendExp,
        MobileExp,
        DevExperienceYears,
        TestingTypes,
        AutomationTools,
        Certifications,
        ManagerCertType,
        ManagerIssueDate,
        ManagerExpiryDate,
        ProjectComplexity
    };

    explicit EmployeesTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList departmentList() const {return m_departmentList;}
    QStringList positionList() const {return m_positionList;}

    Q_INVOKABLE void applyFilters(
        int departmentIndex, int position,
        int minExp, int maxExp,
        int minAge, int maxAge,
        double minSalary, double maxSalary,
        bool isActive);

signals:
    void departmentListChanged();
    void positionListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;


private:



    std::vector<lib::SqlFunctions::EmployeeFilterResult> employees;
    std::shared_ptr<DataBase> database;
    QStringList m_departmentList;
    QStringList m_positionList;
    void updateDepartmentList();
    void updateEmployee();
    void updatePositionList();
};

