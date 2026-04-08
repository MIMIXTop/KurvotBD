#include "BugsTableModel.hpp"
#include <future>
#include <iostream>
#include <ranges>

BugsTableModel::BugsTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
    updateProjectList();
    updateStatusList();
    updateSeverityList();
    loadAll();
}

int BugsTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(bugs.size());
}

int BugsTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return 10; // bug_id, project, title, status, severity, found_date, fixed_date
}

QVariant BugsTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(bugs.size()))
        return {};

    const auto &item = bugs[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return item.bug_id;
            case 1: return QString::fromStdString(item.project_name);
            case 2: return QString::fromStdString(item.title);
            case 3: return QString::fromStdString(item.description); // Добавлено
            case 4: return QString::fromStdString(item.status);
            case 5: return QString::fromStdString(item.severity);
            case 6: return QString::fromStdString(item.found_by_name); // Добавлено
            case 7: return QString::fromStdString(item.fixed_by_name); // Добавлено
            case 8: return QString::fromStdString(item.created_at);    // Добавлено
            case 9: return QString::fromStdString(item.found_date);
            case 10: return QString::fromStdString(item.fixed_date);
            default: return {};
        }
    }

    if (role >= BugId) {
        switch (role) {
            case BugId: return item.bug_id;
            case ProjectName: return QString::fromStdString(item.project_name);
            case Title: return QString::fromStdString(item.title);
            case Description: return QString::fromStdString(item.description);
            case Status: return QString::fromStdString(item.status);
            case Severity: return QString::fromStdString(item.severity);
            case FoundBy: return QString::fromStdString(item.found_by_name);
            case FixedBy: return QString::fromStdString(item.fixed_by_name);
            case CreatedAt: return QString::fromStdString(item.created_at);
            case FoundDate: return QString::fromStdString(item.found_date);
            case FixedDate: return QString::fromStdString(item.fixed_date);
            case TotalCount: return static_cast<int>(item.total_count);
            default: return {};
        }
    }

    return {};
}

void BugsTableModel::loadAll() {
    beginResetModel();
    bugs = database->getBugsByFilter();
    endResetModel();
}

void BugsTableModel::applyFilters(
    int projectIndex,
    const QVariantList &statuses,
    const QVariantList &severities,
    const QString &foundFrom,
    const QString &foundTo)
{
    std::optional<int> optProjectId;
    if (projectIndex > 0) {
        optProjectId = projectIndex; // Предполагаем, что индекс соответствует project_id
    }

    std::optional<std::vector<std::string>> statusVec;
    if (!statuses.isEmpty()) {
        std::vector<std::string> vec;
        for (const auto &v : statuses) {
            vec.push_back(v.toString().toStdString());
        }
        statusVec = vec;
    }

    std::optional<std::vector<std::string>> severityVec;
    if (!severities.isEmpty()) {
        std::vector<std::string> vec;
        for (const auto &v : severities) {
            vec.push_back(v.toString().toStdString());
        }
        severityVec = vec;
    }

    std::optional<std::string> optFoundFrom = foundFrom.isEmpty() ? std::nullopt : std::optional<std::string>(foundFrom.toStdString());
    std::optional<std::string> optFoundTo = foundTo.isEmpty() ? std::nullopt : std::optional<std::string>(foundTo.toStdString());

    auto res = std::async([this, optProjectId, statusVec, severityVec, optFoundFrom, optFoundTo] {
        return database->getBugsByFilter(optProjectId, statusVec, severityVec, optFoundFrom, optFoundTo);
    });

    beginResetModel();
    bugs = res.get();
    endResetModel();
}

QHash<int, QByteArray> BugsTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[BugId] = "bugId";
    roles[ProjectName] = "projectName";
    roles[Title] = "title";
    roles[Description] = "description";
    roles[Status] = "status";
    roles[Severity] = "severity";
    roles[FoundBy] = "foundBy";
    roles[FixedBy] = "fixedBy";
    roles[CreatedAt] = "createdAt";
    roles[FoundDate] = "foundDate";
    roles[FixedDate] = "fixedDate";
    roles[TotalCount] = "totalCount";
    return roles;
}

void BugsTableModel::updateProjectList() {
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

void BugsTableModel::updateStatusList() {
    m_statusList = {"новая", "в_работе", "исправлена", "проверена"};
    emit statusListChanged();
}

void BugsTableModel::updateSeverityList() {
    m_severityList = {"блокирующая", "критическая", "средняя", "низкая"};
    emit severityListChanged();
}
