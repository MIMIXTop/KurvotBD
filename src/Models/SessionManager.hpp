#pragma once

#include <QObject>
#include <QString>
#include <QVariant>
#include <memory>
#include "lib/src/DataBase.hpp"

class SessionManager : public QObject {
    Q_OBJECT
    
    Q_PROPERTY(QString currentUser READ currentUser NOTIFY currentUserChanged)
    Q_PROPERTY(QString currentRole READ currentRole NOTIFY currentRoleChanged)
    Q_PROPERTY(bool isAdmin READ isAdmin NOTIFY currentRoleChanged)
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY loginStateChanged)
    
public:
    static SessionManager* instance(QObject* parent = nullptr);
    
    Q_INVOKABLE bool login(const QString& username, const QString& password);
    Q_INVOKABLE void logout();
    
    QString currentUser() const { return m_currentUser; }
    QString currentRole() const { return m_currentRole; }
    bool isAdmin() const { return m_currentRole == "admin_role"; }
    bool isLoggedIn() const { return m_isLoggedIn; }
    
    std::shared_ptr<DataBase> getDatabase() const { return m_database; }
    
signals:
    void currentUserChanged();
    void currentRoleChanged();
    void loginStateChanged();
    void loginSuccess();
    void loginFailed(const QString& error);
    
private:
    explicit SessionManager(QObject* parent = nullptr);
    
    QString m_currentUser;
    QString m_currentRole;
    bool m_isLoggedIn = false;
    std::shared_ptr<DataBase> m_database;
    
    static SessionManager* s_instance;
};