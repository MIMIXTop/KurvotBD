#include "SpecificationTableModel.hpp"
#include <future>
#include <iostream>
#include "lib/src/Models.hpp"

SpecificationTableModel::SpecificationTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
    // Load project list
    try {
        auto projects = database->selectAll(lib::Models::Project{});
        for (const auto &variant : projects) {
            std::visit([this](const auto &model) {
                using T = std::decay_t<decltype(model)>;
                if constexpr (std::is_same_v<T, lib::Models::Project>) {
                    m_projectList.append(QString::fromStdString(model.name));
                }
            }, variant);
        }
        emit projectListChanged();
    } catch (const std::exception &e) {
        std::println(std::cerr, "Error loading project list: {}", e.what());
    }
}

int SpecificationTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(specs.size());
}

int SpecificationTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 5; // ProjectName, Version, AuthorName, CreationDate, LastUpdate
}

QVariant SpecificationTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(specs.size()))
        return {};

    const auto &item = specs[index.row()];

    switch (role) {
        case Qt::DisplayRole:
            return {};
        case SpecId:
            return item.spec_id;
        case ProjectName:
            return QString::fromStdString(item.project_name);
        case Version:
            return QString::fromStdString(item.version);
        case AuthorName:
            return QString::fromStdString(item.author_name);
        case CreationDate:
            return QString::fromStdString(item.creation_date);
        case LastUpdate:
            return QString::fromStdString(item.last_update);
        case DocumentText:
            return QString::fromStdString(item.document_text);
        default:
            return {};
    }
}

void SpecificationTableModel::loadAll() {
    beginResetModel();
    specs.clear();

    try {
        pqxx::work txn(database->getConnection());
        
        std::string sql = R"(
            SELECT s.spec_id, p.name AS project_name,
                   COALESCE(e.last_name || ' ' || e.first_name, 'Система') AS author_name,
                   TO_CHAR(s.created_at, 'YYYY-MM-DD') AS creation_date,
                   TO_CHAR(s.created_at, 'YYYY-MM-DD') AS last_update,
                   'Версия ' || s.version AS version,
                   s.document_text
            FROM ProjectSpecification s
            JOIN Project p ON s.project_id = p.project_id
            LEFT JOIN Employee e ON s.updated_by = e.employee_id
            ORDER BY s.created_at DESC
        )";

        pqxx::result res = txn.exec(sql);
        for (auto &&row : res) {
            SpecItem item;
            item.spec_id = row["spec_id"].as<int>();
            item.project_name = row["project_name"].as<std::string>();
            item.version = row["version"].as<std::string>();
            item.author_name = row["author_name"].as<std::string>();
            item.creation_date = row["creation_date"].as<std::string>();
            item.last_update = row["last_update"].as<std::string>();
            item.document_text = row["document_text"].as<std::string>();
            specs.push_back(item);
        }

        txn.commit();
    } catch (const std::exception &e) {
        std::println(std::cerr, "Error loading specifications: {}", e.what());
    }

    endResetModel();
}

void SpecificationTableModel::applyFilters(const QString &projectName) {
    beginResetModel();
    specs.clear();

    try {
        pqxx::work txn(database->getConnection());
        
        std::string whereClause = "";
        if (!projectName.isEmpty()) {
            whereClause = " AND p.name = " + txn.quote(projectName.toStdString());
        }
        
        std::string sql = R"(
            SELECT s.spec_id, p.name AS project_name,
                   COALESCE(e.last_name || ' ' || e.first_name, 'Система') AS author_name,
                   TO_CHAR(s.created_at, 'YYYY-MM-DD') AS creation_date,
                   TO_CHAR(s.created_at, 'YYYY-MM-DD') AS last_update,
                   'Версия ' || s.version AS version,
                   s.document_text
            FROM ProjectSpecification s
            JOIN Project p ON s.project_id = p.project_id
            LEFT JOIN Employee e ON s.updated_by = e.employee_id
            WHERE 1=1 )" + whereClause + R"(
            ORDER BY s.created_at DESC
        )";

        pqxx::result res = txn.exec(sql);
        for (auto &&row : res) {
            SpecItem item;
            item.spec_id = row["spec_id"].as<int>();
            item.project_name = row["project_name"].as<std::string>();
            item.version = row["version"].as<std::string>();
            item.author_name = row["author_name"].as<std::string>();
            item.creation_date = row["creation_date"].as<std::string>();
            item.last_update = row["last_update"].as<std::string>();
            item.document_text = row["document_text"].as<std::string>();
            specs.push_back(item);
        }

        txn.commit();
    } catch (const std::exception &e) {
        std::println(std::cerr, "Error applying filters: {}", e.what());
    }

    endResetModel();
}

QHash<int, QByteArray> SpecificationTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[SpecId] = "specId";
    roles[ProjectName] = "projectName";
    roles[Version] = "version";
    roles[AuthorName] = "authorName";
    roles[CreationDate] = "creationDate";
    roles[LastUpdate] = "lastUpdate";
    roles[DocumentText] = "documentText";
    return roles;
}
