//
// Created by mimixtop on 06.04.2026.
//

#include "EmployeesTableModel.hpp"

#include <future>
#include <iostream>

#include "SqlFunctions.hpp"

EmployeesTableModel::EmployeesTableModel(QObject *parent) : QAbstractTableModel(parent) {
   database = std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass");

    updateDepartmentList();
    updatePositionList();
    updateEmployee();
}

int EmployeesTableModel::rowCount(const QModelIndex &parent) const {
    return employees.size();
}

int EmployeesTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 8;
}

QVariant EmployeesTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= employees.size())
        return QVariant();

    const auto &res = employees.at(index.row());

    switch (role) {
        case NameRole: return QString::fromStdString(res.full_name);
        case DepartmentName: return QString::fromStdString(res.department_name);
        case Position: return QString::fromStdString(res.position_title);
        case Age: return res.age;
        case Salary: return res.salary;
        case ExperienceYears: return res.experience_years;
        case Email: return QString::fromStdString(res.email);
        case Phone: return QString::fromStdString(res.phone);

        case Qt::DisplayRole:
            if (index.column() == 0) return QString::fromStdString(res.full_name);
            if (index.column() == 1) return QString::fromStdString(res.department_name);
            if (index.column() == 2) return QString::fromStdString(res.position_title);
            if (index.column() == 3) return res.age;
            if (index.column() == 4) return res.salary;
            if (index.column() == 5) return static_cast<uint>(res.experience_years);
            if (index.column() == 6) return QString::fromStdString(res.email);
            if (index.column() == 7) return QString::fromStdString(res.phone);
            break;
    }
    return {};
}

QHash<int, QByteArray> EmployeesTableModel::roleNames() const {
    return {
        {NameRole, "employeeName"},
        {DepartmentName, "departmentName"},
        {Position, "positionTitle"},
        {Age, "age"},
        {Salary, "salary"},
        {ExperienceYears, "yearsOfExperience"},
        {Email, "email"},
        {Phone, "phoneNumber"},
    };
}

void EmployeesTableModel::updateEmployee() {
    beginResetModel();
    employees = database->getEmployeeByFilters();
    endResetModel();
}

void EmployeesTableModel::applyFilters(
    int departmentIndex, int position,
    int minExp, int maxExp,
    int minAge, int maxAge,
    double minSalary, double maxSalary,
    bool isActive) {
    std::optional<std::vector<int> > deptIds;
    if (departmentIndex > 0) {
        deptIds = std::vector<int>{departmentIndex};
    }

    std::optional<std::vector<int> > posTitles;
    if (position > 0) {
        posTitles = std::vector<int>{position};
    }

    std::optional<int> optMinExp = (minExp >= 0) ? std::optional<int>(minExp) : std::nullopt;
    std::optional<int> optMaxExp = (maxExp >= 0) ? std::optional<int>(maxExp) : std::nullopt;
    std::optional<int> optMinAge = (minAge >= 0) ? std::optional<int>(minAge) : std::nullopt;
    std::optional<int> optMaxAge = (maxAge >= 0) ? std::optional<int>(maxAge) : std::nullopt;
    std::optional<double> optMinSal = (minSalary >= 0) ? std::optional<double>(minSalary) : std::nullopt;
    std::optional<double> optMaxSal = (maxSalary >= 0) ? std::optional<double>(maxSalary) : std::nullopt;
    std::optional<bool> optIsActive = isActive;

    auto res = std::async(
        [this, deptIds, posTitles, optMinExp, optMaxExp, optMinAge, optMaxAge, optMinSal, optMaxSal, optIsActive] {
return database->getEmployeeByFilters(deptIds, posTitles, optMinExp, optMaxExp, optMinAge, optMaxAge,
                                                   optMinSal, optMaxSal, std::nullopt, optIsActive);
        });

    beginResetModel();
    employees = res.get();
    endResetModel();

    std::println(std::cout, "{}", employees.begin()->position_title);
}

void EmployeesTableModel::updateDepartmentList() {
    m_departmentList = {"Все отделы"};

    auto res = std::async([this] {
        return database->selectAll(lib::Models::Department{});
    });

    std::ranges::for_each(res.get(), [this](const lib::Models::Model &m) {
        std::visit(lib::util::match{
                       [this](const lib::Models::Department &dep) {
                           m_departmentList << QString::fromStdString(dep.name);
                       },
                       [](auto &&) {
                       }
                   }, m);
    });

    emit departmentListChanged();
}

void EmployeesTableModel::updatePositionList() {
    m_positionList = {"Все позиции"};

    auto res = std::async([this] {
        return database->selectAll(lib::Models::Position{});
    });

    std::ranges::for_each(res.get(), [this](const lib::Models::Model &m) {
        std::visit(lib::util::match{
                       [this](const lib::Models::Position &pos) {
                           m_positionList << QString::fromStdString(pos.title);
                       },
                       [](auto) {
                       }
                   }, m);
    });

    emit positionListChanged();
}
