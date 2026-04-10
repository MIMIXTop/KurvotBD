#pragma once

#include <QAbstractTableModel>
#include <QStringList>

#include "lib/src/DataBase.hpp"

class SpecificationTableModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(QStringList projectList READ projectList NOTIFY projectListChanged)

public:
    enum SpecificationRoles {
        SpecId = Qt::UserRole + 1,
        ProjectName,
        Version,
        AuthorName,
        CreationDate,
        LastUpdate,
        DocumentText
    };
    Q_ENUM(SpecificationRoles)

    explicit SpecificationTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList projectList() const { return m_projectList; }

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void applyFilters(const QString &projectName);

signals:
    void projectListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    struct SpecItem {
        int spec_id;
        std::string project_name;
        std::string version;
        std::string author_name;
        std::string creation_date;
        std::string last_update;
        std::string document_text;
    };

    std::vector<SpecItem> specs;
    std::shared_ptr<DataBase> database;
    QStringList m_projectList;
};
