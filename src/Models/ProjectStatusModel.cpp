#include "ProjectStatusModel.hpp"
#include <future>

ProjectStatusModel::ProjectStatusModel(QObject *parent)
    : QObject(parent)
    , database(std::make_shared<DataBase>("host=127.0.0.1 port=5432 dbname=db user=user password=pass"))
{
}

void ProjectStatusModel::loadStatus(int projectId, const QString &startDate, const QString &endDate) {
    if (projectId <= 0 || startDate.isEmpty() || endDate.isEmpty()) {
        m_hasData = false;
        emit statusChanged();
        return;
    }

    auto res = std::async([this, projectId, startDate, endDate] {
        return database->getProjectStatusReport(projectId, startDate.toStdString(), endDate.toStdString());
    });

    auto results = res.get();
    if (!results.empty()) {
        const auto &item = results[0];
        m_projectName = QString::fromStdString(item.project_name);
        m_currentPhase = QString::fromStdString(item.current_phase);
        m_tasksLogged = item.tasks_logged;
        m_teamHoursSpent = item.team_hours_spent;
        m_criticalRisksCount = item.critical_risks_count;
        m_reportPeriod = QString::fromStdString(item.report_period);
        m_hasData = true;
    } else {
        m_hasData = false;
    }

    emit statusChanged();
}
