#pragma once

#include <QObject>
#include <QString>

#include "lib/src/SqlFunctions.hpp"
#include "lib/src/DataBase.hpp"

class ProjectStatusModel : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString projectName READ projectName NOTIFY statusChanged)
    Q_PROPERTY(QString currentPhase READ currentPhase NOTIFY statusChanged)
    Q_PROPERTY(qint64 tasksLogged READ tasksLogged NOTIFY statusChanged)
    Q_PROPERTY(double teamHoursSpent READ teamHoursSpent NOTIFY statusChanged)
    Q_PROPERTY(qint64 criticalRisksCount READ criticalRisksCount NOTIFY statusChanged)
    Q_PROPERTY(QString reportPeriod READ reportPeriod NOTIFY statusChanged)
    Q_PROPERTY(bool hasData READ hasData NOTIFY statusChanged)

public:
    explicit ProjectStatusModel(QObject *parent = nullptr);

    QString projectName() const { return m_projectName; }
    QString currentPhase() const { return m_currentPhase; }
    qint64 tasksLogged() const { return m_tasksLogged; }
    double teamHoursSpent() const { return m_teamHoursSpent; }
    qint64 criticalRisksCount() const { return m_criticalRisksCount; }
    QString reportPeriod() const { return m_reportPeriod; }
    bool hasData() const { return m_hasData; }

    Q_INVOKABLE void loadStatus(int projectId, const QString &startDate, const QString &endDate);

signals:
    void statusChanged();

private:
    std::shared_ptr<DataBase> database;

    QString m_projectName;
    QString m_currentPhase;
    qint64 m_tasksLogged = 0;
    double m_teamHoursSpent = 0;
    qint64 m_criticalRisksCount = 0;
    QString m_reportPeriod;
    bool m_hasData = false;
};
