#include "ReleasesTableModel.hpp"
#include <future>

ReleasesTableModel::ReleasesTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
    loadAll();
}

int ReleasesTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(releases.size());
}

int ReleasesTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) return 0;
    return 4; // project_name, version, release_date, changelog
}

QVariant ReleasesTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(releases.size()))
        return {};

    const auto &item = releases[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.project_name);
            case 1: return QString::fromStdString(item.version);
            case 2: return QString::fromStdString(item.release_date);
            case 3: return QString::fromStdString(item.changelog);
            default: return {};
        }
    }

    if (role >= ProjectName) {
        switch (role) {
            case ProjectName: return QString::fromStdString(item.project_name);
            case Version: return QString::fromStdString(item.version);
            case ReleaseDate: return QString::fromStdString(item.release_date);
            case Changelog: return QString::fromStdString(item.changelog);
            default: return {};
        }
    }

    return {};
}

void ReleasesTableModel::loadAll() {
    beginResetModel();
    releases = database->getReleaseReport();
    endResetModel();
}

void ReleasesTableModel::loadFiltered(const QString &fromDate, const QString &toDate) {
    std::optional<std::string> optFrom = fromDate.isEmpty() ? std::nullopt : std::optional<std::string>(fromDate.toStdString());
    std::optional<std::string> optTo = toDate.isEmpty() ? std::nullopt : std::optional<std::string>(toDate.toStdString());

    auto res = std::async([this, optFrom, optTo] {
        return database->getReleaseReport(optFrom, optTo);
    });

    beginResetModel();
    releases = res.get();
    endResetModel();
}

QHash<int, QByteArray> ReleasesTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[ProjectName] = "projectName";
    roles[Version] = "version";
    roles[ReleaseDate] = "releaseDate";
    roles[Changelog] = "changelog";
    return roles;
}
