#pragma once

#include <QAbstractTableModel>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class TestingEfficiencyTableModel : public QAbstractTableModel {
    Q_OBJECT
    Q_PROPERTY(QStringList projectList READ projectList NOTIFY projectListChanged)

public:
    enum EfficiencyRoles {
        ProjectName = Qt::UserRole + 1,
        BugsInTest,
        BugsAfterRelease,
        EfficiencyRatio
    };

    explicit TestingEfficiencyTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void loadFiltered(int projectIndex, const QString &fromDate, const QString &toDate);

    QStringList projectList() const { return m_projectList; }

signals:
    void projectListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::TestingEfficiencyResult> efficiencyData;
    std::shared_ptr<DataBase> database;
    QStringList m_projectList;

    void updateProjectList();
};
