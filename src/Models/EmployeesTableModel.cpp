//
// Created by mimixtop on 06.04.2026.
//

#include "EmployeesTableModel.hpp"

#include <future>

#include "SqlFunctions.hpp"

EmployeesTableModel::EmployeesTableModel(QObject *parent) : QAbstractTableModel(parent) {
    employees = {
        {1, 1, 1, "Иванов Иван Иванович", "Отдел разработки", "Программист", 34, 5, 150000.0, "ivanov@test.ru", "+7(999)123-45-67"},
        {2, 2, 2, "Петров Петр Петрович", "Отдел тестирования", "Тестировщик", 28, 3, 120000.0, "petrov@test.ru", "+7(999)234-56-78"},
        {3, 1, 1, "Сидоров Алексей Александрович", "Отдел разработки", "Старший программист", 38, 10, 200000.0, "sidorov@test.ru", "+7(999)345-67-89"},
        {4, 3, 3, "Козлова Мария Сергеевна", "Отдел техподдержки", "Инженер поддержки", 25, 1, 80000.0, "kozlova@test.ru", "+7(999)456-78-90"},
        {5, 2, 2, "Смирнов Дмитрий Олегович", "Отдел тестирования", "QA инженер", 30, 4, 110000.0, "smirnov@test.ru", "+7(999)567-89-01"},
    };

    database = std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass");
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

    const auto& res = employees.at(index.row());

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
        {Position, "position"},
        {Age, "age"},
        {Salary, "salary"},
        {ExperienceYears, "experienceYears"},
        {Email, "email"},
        {Phone, "phone"},
    };
}

void EmployeesTableModel::updateEmployee() {

    auto res = std::async([this] {
        return database->getEmployeeByFilters();
    });

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    employees.clear();
    employees = res.get();
    endInsertRows();

}

void EmployeesTableModel::applyFilters(
    int departmentIndex, const QString& position,
    int minExp, int maxExp,
    int minAge, int maxAge,
    double minSalary, double maxSalary,
    bool isActive) {

    std::optional<std::vector<int>> deptIds;
    if (departmentIndex > 0) {
        deptIds = std::vector<int>{departmentIndex};
    }

    std::optional<std::vector<std::string>> posTitles;
    if (!position.trimmed().isEmpty()) {
        posTitles = std::vector<std::string>{position.trimmed().toStdString()};
    }

    std::optional<int> optMinExp = (minExp >= 0) ? std::optional<int>(minExp) : std::nullopt;
    std::optional<int> optMaxExp = (maxExp >= 0) ? std::optional<int>(maxExp) : std::nullopt;
    std::optional<int> optMinAge = (minAge >= 0) ? std::optional<int>(minAge) : std::nullopt;
    std::optional<int> optMaxAge = (maxAge >= 0) ? std::optional<int>(maxAge) : std::nullopt;
    std::optional<double> optMinSal = (minSalary >= 0) ? std::optional<double>(minSalary) : std::nullopt;
    std::optional<double> optMaxSal = (maxSalary >= 0) ? std::optional<double>(maxSalary) : std::nullopt;
    std::optional<bool> optIsActive = isActive;

    auto res = std::async([this, deptIds, posTitles, optMinExp, optMaxExp, optMinAge, optMaxAge, optMinSal, optMaxSal, optIsActive] {
        return database->getEmployeeByFilters(deptIds, posTitles, optMinExp, optMaxExp, optMinAge, optMaxAge, optMinSal, optMaxSal, optIsActive);
    });

    beginResetModel();
    employees = res.get();
    endResetModel();

}
