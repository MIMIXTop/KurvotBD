#pragma once

#include <QAbstractTableModel>
#include <QStringList>

#include "lib/src/DataBase.hpp"

class DocumentationTableModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(QStringList projectList READ projectList NOTIFY projectListChanged)
    Q_PROPERTY(QStringList docTypeList READ docTypeList NOTIFY docTypeListChanged)

public:
    enum DocumentationRoles {
        DocId = Qt::UserRole + 1,
        ProjectName,
        DocType,
        AuthorName,
        CreationDate,
        LastUpdate,
        StoragePath,
        DocumentText
    };
    Q_ENUM(DocumentationRoles)

    explicit DocumentationTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList projectList() const { return m_projectList; }
    QStringList docTypeList() const { return m_docTypeList; }

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void applyFilters(const QString &projectName, const QString &docType);

signals:
    void projectListChanged();
    void docTypeListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    struct DocItem {
        int doc_id;
        std::string project_name;
        std::string doc_type;
        std::string author_name;
        std::string creation_date;
        std::string last_update;
        std::string storage_path;
        std::string document_text;
    };

    std::vector<DocItem> docs;
    std::shared_ptr<DataBase> database;
    QStringList m_projectList;
    QStringList m_docTypeList;
};
