#pragma once

#include <QAbstractTableModel>
#include <QStringList>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class ProjectTechnologiesTableModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(QStringList techList READ techList NOTIFY techListChanged)

public:
    enum TechnologyRoles {
        TechName = Qt::UserRole + 1,
        ProjectCount
    };
    Q_ENUM(TechnologyRoles)

    explicit ProjectTechnologiesTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList techList() const { return m_techList; }

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void applyFilters(bool isActive, const QString &techName);

signals:
    void techListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::ProjectTechnologyResult> techData;
    std::shared_ptr<DataBase> database;
    QStringList m_techList;

    void updateTechList();
};
