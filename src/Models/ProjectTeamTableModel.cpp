#include "ProjectTeamTableModel.hpp"
#include <future>
#include <iostream>

ProjectTeamTableModel::ProjectTeamTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
    updateProjectList();
    updateRoleList();
    updatePositionList();
}

int ProjectTeamTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(teamData.size());
}

int ProjectTeamTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 6; // ProjectName, FullName, PositionTitle, ProjectRole, PhaseName, HoursAllocated
}

QVariant ProjectTeamTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(teamData.size()))
        return {};

    const auto &item = teamData[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.project_name);
            case 1: return QString::fromStdString(item.full_name);
            case 2: return QString::fromStdString(item.position_title);
            case 3: return QString::fromStdString(item.project_role);
            case 4: return QString::fromStdString(item.phase_name);
            case 5: return item.hours_allocated;
            default: return {};
        }
    }

    if (role >= AssignmentId) {
        switch (role) {
            case AssignmentId: return item.assignment_id;
            case EmployeeId: return item.employee_id;
            case ProjectId: return item.project_id;
            case PhaseId: return item.phase_id;
            case FullName: return QString::fromStdString(item.full_name);
            case PositionTitle: return QString::fromStdString(item.position_title);
            case ProjectRole: return QString::fromStdString(item.project_role);
            case ProjectName: return QString::fromStdString(item.project_name);
            case PhaseName: return QString::fromStdString(item.phase_name);
            case CurrentPhase: return QString::fromStdString(item.current_phase);
            case StartDate: return QString::fromStdString(item.start_date);
            case EndDate: return QString::fromStdString(item.end_date);
            case HoursAllocated: return item.hours_allocated;
            default: return {};
        }
    }

    return {};
}

void ProjectTeamTableModel::loadAll() {
    beginResetModel();
    teamData = database->getProjectTeam();
    endResetModel();

    if (!teamData.empty()) {
        std::println(std::cout, "{}", teamData.begin()->phase_name);
    }
}

void ProjectTeamTableModel::applyFilters(int projectIndex, const QVariantList &roles, const QVariantList &positions, bool isActive, int sortIndex) {
    std::optional<int> optProjectId;
    if (projectIndex > 0 && projectIndex <= static_cast<int>(m_projectIds.size())) {
        optProjectId = m_projectIds[projectIndex - 1];
    }

    std::optional<std::vector<std::string>> rolesVec;
    if (!roles.isEmpty()) {
        std::vector<std::string> vec;
        for (const auto &v : roles) {
            vec.push_back(v.toString().toStdString());
        }
        rolesVec = vec;
    }

    std::optional<std::vector<std::string>> positionsVec;
    if (!positions.isEmpty()) {
        std::vector<std::string> vec;
        for (const auto &v : positions) {
            vec.push_back(v.toString().toStdString());
        }
        positionsVec = vec;
    }

    std::optional<bool> optIsActive = isActive ? std::optional<bool>(true) : std::nullopt;

    auto res = std::async([this, optProjectId, rolesVec, positionsVec, optIsActive] {
        return database->getProjectTeam(optProjectId, std::nullopt, rolesVec, positionsVec, optIsActive);
    });

    beginResetModel();
    teamData = res.get();

    // Sorting
    if (sortIndex == 0) { // Role
        std::ranges::sort(teamData, [](const auto &a, const auto &b) {
            return a.project_role < b.project_role;
        });
    } else if (sortIndex == 1) { // Position
        std::ranges::sort(teamData, [](const auto &a, const auto &b) {
            return a.position_title < b.position_title;
        });
    } else if (sortIndex == 2) { // Full Name
        std::ranges::sort(teamData, [](const auto &a, const auto &b) {
            return a.full_name < b.full_name;
        });
    }

    endResetModel();
}

QHash<int, QByteArray> ProjectTeamTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[AssignmentId] = "assignmentId";
    roles[EmployeeId] = "employeeId";
    roles[ProjectId] = "projectId";
    roles[PhaseId] = "phaseId";
    roles[FullName] = "fullName";
    roles[PositionTitle] = "positionTitle";
    roles[ProjectRole] = "projectRole";
    roles[ProjectName] = "projectName";
    roles[PhaseName] = "phaseName";
    roles[CurrentPhase] = "currentPhase";
    roles[StartDate] = "startDate";
    roles[EndDate] = "endDate";
    roles[HoursAllocated] = "hoursAllocated";
    return roles;
}

int ProjectTeamTableModel::getProjectId(int index) const {
    if (index > 0 && index <= static_cast<int>(m_projectIds.size())) {
        return m_projectIds[index - 1];
    }
    return 0;
}

void ProjectTeamTableModel::updateProjectList() {
    m_projectList = {"Все проекты"};
    m_projectIds.clear();

    auto res = std::async([this] {
        return database->selectAll(lib::Models::Project{});
    });

    auto projects = res.get();
    for (const auto &variant : projects) {
        std::visit([this](const auto &model) {
            using T = std::decay_t<decltype(model)>;
            if constexpr (std::is_same_v<T, lib::Models::Project>) {
                m_projectList.append(QString::fromStdString(model.name));
                m_projectIds.push_back(model.project_id);
            }
        }, variant);
    }

    emit projectListChanged();
}

void ProjectTeamTableModel::updateRoleList() {
    m_roleList = {"разработчик", "тестировщик", "аналитик", "менеджер_проекта", "архитектор", "технический_писатель"};
    emit roleListChanged();
}

void ProjectTeamTableModel::updatePositionList() {
    auto res = std::async([this] {
        return database->selectAll(lib::Models::Position{});
    });

    auto positions = res.get();
    m_positionList.clear();

    m_positionList.append("Все должности");

    for (const auto &variant : positions) {
        std::visit([this](const auto &model) {
            using T = std::decay_t<decltype(model)>;
            if constexpr (std::is_same_v<T, lib::Models::Position>) {
                m_positionList.append(QString::fromStdString(model.title));
            }
        }, variant);
    }

    emit positionListChanged();
}
