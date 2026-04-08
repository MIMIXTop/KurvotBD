#include "EmployeesWorkloadModel.hpp"

#include <ranges>
#include <algorithm>
#include <future>

EmployeesWorkloadModel::EmployeesWorkloadModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("postgresql://postgres:1234@localhost:5432/postgres"))
{
    updateEmployeeList();
    updateProjectList();
    loadAll();
}

int EmployeesWorkloadModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(workloadData.size());
}

int EmployeesWorkloadModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return 4; // Сотрудник, Проект, Часы, Задачи
}

QVariant EmployeesWorkloadModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(workloadData.size()))
        return {};

    const auto &item = workloadData[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.full_name);
            case 1: return QString::fromStdString(item.project_name);
            case 2: return QString::number(item.total_hours, 'f', 2);
            case 3: return static_cast<int>(item.task_count);
            default: return {};
        }
    }

    if (role >= EmployeeId) {
        switch (role) {
            case EmployeeId: return item.employee_id;
            case EmployeeName: return QString::fromStdString(item.full_name);
            case ProjectId: return item.project_id;
            case ProjectName: return QString::fromStdString(item.project_name);
            case TotalHours: return item.total_hours;
            case TaskCount: return static_cast<int>(item.task_count);
            default: return {};
        }
    }

    return {};
}

void EmployeesWorkloadModel::loadAll() {
    beginResetModel();
    workloadData = database->getEmployeeWorkload();
    endResetModel();
}

void EmployeesWorkloadModel::loadByEmployees(const QVariantList &employeeIds) {
    std::vector<int> ids;
    for (const auto &v : employeeIds) {
        ids.push_back(v.toInt());
    }

    beginResetModel();
    if (ids.empty()) {
        workloadData = database->getEmployeeWorkload();
    } else {
        workloadData = database->getEmployeeWorkload(ids, std::nullopt, std::nullopt, std::nullopt);
    }
    endResetModel();
}

void EmployeesWorkloadModel::loadFiltered(
    const QVariantList &employeeIds,
    const QVariantList &projectIds,
    const QString &fromDate,
    const QString &toDate)
{
    std::optional<std::vector<int>> empIds;
    std::optional<std::vector<int>> projIds;
    std::optional<std::string> from;
    std::optional<std::string> to;

    if (!employeeIds.isEmpty()) {
        std::vector<int> ids;
        for (const auto &v : employeeIds) {
            ids.push_back(v.toInt());
        }
        empIds = ids;
    }

    if (!projectIds.isEmpty()) {
        std::vector<int> ids;
        for (const auto &v : projectIds) {
            ids.push_back(v.toInt());
        }
        projIds = ids;
    }

    if (!fromDate.isEmpty()) {
        from = fromDate.toStdString();
    }

    if (!toDate.isEmpty()) {
        to = toDate.toStdString();
    }

    beginResetModel();
    workloadData = database->getEmployeeWorkload(empIds, projIds, from, to);
    endResetModel();
}

QHash<int, QByteArray> EmployeesWorkloadModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[EmployeeId] = "employeeId";
    roles[EmployeeName] = "employeeName";
    roles[ProjectId] = "projectId";
    roles[ProjectName] = "projectName";
    roles[TotalHours] = "totalHours";
    roles[TaskCount] = "taskCount";
    return roles;
}

void EmployeesWorkloadModel::updateEmployeeList() {
    m_employeeList = {"Все сотрудники"};

    auto res = std::async([this] {
        return database->selectAll(lib::Models::Employee{});
    });

    std::ranges::for_each(res.get(), [this](const lib::Models::Model &m) {
        std::visit(lib::util::match{
                       [this](const lib::Models::Employee &emp) {
                           m_employeeList << QString::fromStdString(emp.last_name + " " + emp.first_name);
                       },
                       [](auto &&) {}
                   }, m);
    });

    emit employeeListChanged();
}

void EmployeesWorkloadModel::updateProjectList() {
    m_projectList = {"Все проекты"};

    auto res = std::async([this] {
        return database->selectAll(lib::Models::Project{});
    });

    std::ranges::for_each(res.get(), [this](const lib::Models::Model &m) {
        std::visit(lib::util::match{
                       [this](const lib::Models::Project &proj) {
                           m_projectList << QString::fromStdString(proj.name);
                       },
                       [](auto &&) {}
                   }, m);
    });

    emit projectListChanged();
}