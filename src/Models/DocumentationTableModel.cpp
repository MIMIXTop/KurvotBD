#include "DocumentationTableModel.hpp"
#include <future>
#include <iostream>
#include "lib/src/Models.hpp"

DocumentationTableModel::DocumentationTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
    m_docTypeList = {"архитектура", "API", "руководство_пользователя", "руководство_администратора", "спецификация"};
    emit docTypeListChanged();
    
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

int DocumentationTableModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return static_cast<int>(docs.size());
}

int DocumentationTableModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return 0;
    return 6; // ProjectName, DocType, AuthorName, CreationDate, LastUpdate, StoragePath
}

QVariant DocumentationTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= static_cast<int>(docs.size()))
        return {};

    const auto &item = docs[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString::fromStdString(item.project_name);
            case 1: return QString::fromStdString(item.doc_type);
            case 2: return QString::fromStdString(item.author_name);
            case 3: return QString::fromStdString(item.creation_date);
            case 4: return QString::fromStdString(item.last_update);
            case 5: return QString::fromStdString(item.storage_path);
            default: return {};
        }
    }

    if (role >= DocId) {
        switch (role) {
            case DocId: return item.doc_id;
            case ProjectName: return QString::fromStdString(item.project_name);
            case DocType: return QString::fromStdString(item.doc_type);
            case AuthorName: return QString::fromStdString(item.author_name);
            case CreationDate: return QString::fromStdString(item.creation_date);
            case LastUpdate: return QString::fromStdString(item.last_update);
            case StoragePath: return QString::fromStdString(item.storage_path);
            case DocumentText: return QString::fromStdString(item.document_text);
            default: return {};
        }
    }

    return {};
}

void DocumentationTableModel::loadAll() {
    beginResetModel();
    docs.clear();

    try {
        pqxx::work txn(database->getConnection());
        
        // Load Documentation
        std::string sqlDocs = R"(
            SELECT d.doc_id, p.name AS project_name, d.type AS doc_type,
                   e.last_name || ' ' || e.first_name AS author_name,
                   TO_CHAR(d.creation_date, 'YYYY-MM-DD') AS creation_date,
                   TO_CHAR(d.last_update, 'YYYY-MM-DD') AS last_update,
                   COALESCE(d.storage_path, '') AS storage_path
            FROM Documentation d
            JOIN Project p ON d.project_id = p.project_id
            JOIN Employee e ON d.author_id = e.employee_id
            ORDER BY d.creation_date DESC
        )";

        pqxx::result resDocs = txn.exec(sqlDocs);
        for (auto &&row : resDocs) {
            DocItem item;
            item.doc_id = row["doc_id"].as<int>();
            item.project_name = row["project_name"].as<std::string>();
            item.doc_type = row["doc_type"].as<std::string>();
            item.author_name = row["author_name"].as<std::string>();
            item.creation_date = row["creation_date"].as<std::string>();
            item.last_update = row["last_update"].as<std::string>();
            item.storage_path = row["storage_path"].as<std::string>();
            item.document_text = "";
            docs.push_back(item);
        }

        // Load ProjectSpecification as documentation
        std::string sqlSpecs = R"(
            SELECT s.spec_id, p.name AS project_name, 'спецификация' AS doc_type,
                   COALESCE(e.last_name || ' ' || e.first_name, 'Система') AS author_name,
                   TO_CHAR(s.created_at, 'YYYY-MM-DD') AS creation_date,
                   TO_CHAR(s.created_at, 'YYYY-MM-DD') AS last_update,
                   'Версия ' || s.version AS storage_path,
                   s.document_text
            FROM ProjectSpecification s
            JOIN Project p ON s.project_id = p.project_id
            LEFT JOIN Employee e ON s.updated_by = e.employee_id
            ORDER BY s.created_at DESC
        )";

        pqxx::result resSpecs = txn.exec(sqlSpecs);
        for (auto &&row : resSpecs) {
            DocItem item;
            item.doc_id = row["spec_id"].as<int>();
            item.project_name = row["project_name"].as<std::string>();
            item.doc_type = row["doc_type"].as<std::string>();
            item.author_name = row["author_name"].as<std::string>();
            item.creation_date = row["creation_date"].as<std::string>();
            item.last_update = row["last_update"].as<std::string>();
            item.storage_path = row["storage_path"].as<std::string>();
            item.document_text = row["document_text"].as<std::string>();
            docs.push_back(item);
        }

        txn.commit();
    } catch (const std::exception &e) {
        std::println(std::cerr, "Error loading documentation: {}", e.what());
    }

    endResetModel();
}

void DocumentationTableModel::applyFilters(const QString &projectName, const QString &docType) {
    beginResetModel();
    docs.clear();

    try {
        pqxx::work txn(database->getConnection());
        
        // Build WHERE clause for filters
        std::string whereClause = "";
        if (!projectName.isEmpty()) {
            whereClause += " AND p.name = " + txn.quote(projectName.toStdString());
        }
        if (!docType.isEmpty()) {
            whereClause += " AND d.type = " + txn.quote(docType.toStdString());
        }
        
        // Load Documentation with filters
        std::string sqlDocs = R"(
            SELECT d.doc_id, p.name AS project_name, d.type AS doc_type,
                   e.last_name || ' ' || e.first_name AS author_name,
                   TO_CHAR(d.creation_date, 'YYYY-MM-DD') AS creation_date,
                   TO_CHAR(d.last_update, 'YYYY-MM-DD') AS last_update,
                   COALESCE(d.storage_path, '') AS storage_path
            FROM Documentation d
            JOIN Project p ON d.project_id = p.project_id
            JOIN Employee e ON d.author_id = e.employee_id
            WHERE 1=1 )" + whereClause + R"(
            ORDER BY d.creation_date DESC
        )";

        pqxx::result resDocs = txn.exec(sqlDocs);
        for (auto &&row : resDocs) {
            DocItem item;
            item.doc_id = row["doc_id"].as<int>();
            item.project_name = row["project_name"].as<std::string>();
            item.doc_type = row["doc_type"].as<std::string>();
            item.author_name = row["author_name"].as<std::string>();
            item.creation_date = row["creation_date"].as<std::string>();
            item.last_update = row["last_update"].as<std::string>();
            item.storage_path = row["storage_path"].as<std::string>();
            item.document_text = "";
            docs.push_back(item);
        }

        // Build WHERE clause for specifications
        std::string whereClauseSpecs = "";
        if (!projectName.isEmpty()) {
            whereClauseSpecs += " AND p.name = " + txn.quote(projectName.toStdString());
        }
        if (!docType.isEmpty() && docType.toStdString() == "спецификация") {
            // Include specifications only if docType is "спецификация" or empty
        } else if (!docType.isEmpty()) {
            // Skip specifications if a different docType is selected
            txn.commit();
            endResetModel();
            return;
        }

        // Load ProjectSpecification as documentation with filters
        std::string sqlSpecs = R"(
            SELECT s.spec_id, p.name AS project_name, 'спецификация' AS doc_type,
                   COALESCE(e.last_name || ' ' || e.first_name, 'Система') AS author_name,
                   TO_CHAR(s.created_at, 'YYYY-MM-DD') AS creation_date,
                   TO_CHAR(s.created_at, 'YYYY-MM-DD') AS last_update,
                   'Версия ' || s.version AS storage_path,
                   s.document_text
            FROM ProjectSpecification s
            JOIN Project p ON s.project_id = p.project_id
            LEFT JOIN Employee e ON s.updated_by = e.employee_id
            WHERE 1=1 )" + whereClauseSpecs + R"(
            ORDER BY s.created_at DESC
        )";

        pqxx::result resSpecs = txn.exec(sqlSpecs);
        for (auto &&row : resSpecs) {
            DocItem item;
            item.doc_id = row["spec_id"].as<int>();
            item.project_name = row["project_name"].as<std::string>();
            item.doc_type = row["doc_type"].as<std::string>();
            item.author_name = row["author_name"].as<std::string>();
            item.creation_date = row["creation_date"].as<std::string>();
            item.last_update = row["last_update"].as<std::string>();
            item.storage_path = row["storage_path"].as<std::string>();
            item.document_text = row["document_text"].as<std::string>();
            docs.push_back(item);
        }

        txn.commit();
    } catch (const std::exception &e) {
        std::println(std::cerr, "Error applying filters: {}", e.what());
    }

    endResetModel();
}

QHash<int, QByteArray> DocumentationTableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[DocId] = "docId";
    roles[ProjectName] = "projectName";
    roles[DocType] = "docType";
    roles[AuthorName] = "authorName";
    roles[CreationDate] = "creationDate";
    roles[LastUpdate] = "lastUpdate";
    roles[StoragePath] = "storagePath";
    roles[DocumentText] = "documentText";
    return roles;
}
