#include "TestingEfficiencyTableModel.hpp"
#include <future>
#include <ranges>

TestingEfficiencyTableModel::TestingEfficiencyTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
    updateProjectList();
    loadAll();
}

int TestingEfficiencyTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(efficiencyData.size());
}

int TestingEfficiencyTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return 4; // project_name, bugs_in_test, bugs_after_release, efficiency_ratio
}

QVariant TestingEfficiencyTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(efficiencyData.size()))
        return {};

    const auto &item = efficiencyData[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.project_name);
            case 1: return static_cast<int>(item.bugs_in_test);
            case 2: return static_cast<int>(item.bugs_after_release);
            case 3: return QString::number(item.efficiency_ratio, 'f', 2);
            default: return {};
        }
    }

    if (role >= ProjectName) {
        switch (role) {
            case ProjectName: return QString::fromStdString(item.project_name);
            case BugsInTest: return static_cast<int>(item.bugs_in_test);
            case BugsAfterRelease: return static_cast<int>(item.bugs_after_release);
            case EfficiencyRatio: return item.efficiency_ratio;
            default: return {};
        }
    }

    return {};
}

void TestingEfficiencyTableModel::loadAll() {
    beginResetModel();
    efficiencyData = database->getTestingEfficiency();
    endResetModel();
}

void TestingEfficiencyTableModel::loadFiltered(int projectIndex, const QString &fromDate, const QString &toDate) {
    std::optional<int> optProjectId;
    if (projectIndex > 0) {
        optProjectId = projectIndex;
    }

    std::optional<std::string> optFrom = fromDate.isEmpty() ? std::nullopt : std::optional<std::string>(fromDate.toStdString());
    std::optional<std::string> optTo = toDate.isEmpty() ? std::nullopt : std::optional<std::string>(toDate.toStdString());

    auto res = std::async([this, optProjectId, optFrom, optTo] {
        return database->getTestingEfficiency(optProjectId, optFrom, optTo);
    });

    beginResetModel();
    efficiencyData = res.get();
    endResetModel();
}

QHash<int, QByteArray> TestingEfficiencyTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ProjectName] = "projectName";
    roles[BugsInTest] = "bugsInTest";
    roles[BugsAfterRelease] = "bugsAfterRelease";
    roles[EfficiencyRatio] = "efficiencyRatio";
    return roles;
}

void TestingEfficiencyTableModel::updateProjectList() {
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
