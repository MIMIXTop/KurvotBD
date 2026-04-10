#pragma once

#include <QAbstractTableModel>
#include <QStringList>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class ProjectTeamTableModel : public QAbstractTableModel {
    Q_OBJECT

    Q_PROPERTY(QStringList projectList READ projectList NOTIFY projectListChanged)
    Q_PROPERTY(QStringList roleList READ roleList NOTIFY roleListChanged)
    Q_PROPERTY(QStringList positionList READ positionList NOTIFY positionListChanged)

public:
    enum ProjectTeamRoles {
        AssignmentId = Qt::UserRole + 1,
        EmployeeId,
        ProjectId,
        PhaseId,
        FullName,
        PositionTitle,
        ProjectRole,
        ProjectName,
        PhaseName,
        StartDate,
        EndDate,
        HoursAllocated
    };
    Q_ENUM(ProjectTeamRoles)

    explicit ProjectTeamTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QStringList projectList() const { return m_projectList; }
    QStringList roleList() const { return m_roleList; }
    QStringList positionList() const { return m_positionList; }

    Q_INVOKABLE void loadAll();
    Q_INVOKABLE void applyFilters(int projectIndex, const QVariantList &roles, const QVariantList &positions, bool isActive, int sortIndex = 0);
    Q_INVOKABLE int getProjectId(int index) const;

signals:
    void projectListChanged();
    void roleListChanged();
    void positionListChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<lib::SqlFunctions::ProjectTeamResult> teamData;
    std::shared_ptr<DataBase> database;
    
    QStringList m_projectList;
    QStringList m_roleList;
    QStringList m_positionList;
    std::vector<int> m_projectIds;

    void updateProjectList();
    void updateRoleList();
    void updatePositionList();
};
