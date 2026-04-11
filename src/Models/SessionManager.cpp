#include "SessionManager.hpp"
#include <iostream>
#include <pqxx/pqxx>

SessionManager* SessionManager::s_instance = nullptr;

SessionManager::SessionManager(QObject* parent)
    : QObject(parent)
{}

SessionManager* SessionManager::instance(QObject* parent) {
    if (!s_instance) {
        s_instance = new SessionManager(parent);
    }
    return s_instance;
}

bool SessionManager::login(const QString& username, const QString& password) {
    try {
        std::string connectionString = 
            "host=127.0.0.1 port=5432 dbname=db user=" + username.toStdString() + 
            " password=" + password.toStdString();
        
        m_database = std::make_shared<DataBase>(connectionString);
        
        pqxx::connection testConn(connectionString);
        if (testConn.is_open()) {
            m_currentUser = username;
            
            if (username.contains("admin")) {
                m_currentRole = "admin_role";
                std::cout << "[AUDIT] User 'admin_role' logged in successfully" << std::endl;
            } else {
                m_currentRole = "user_role";
                std::cout << "[AUDIT] User 'user_role' logged in successfully" << std::endl;
            }
            
            m_isLoggedIn = true;
            emit currentUserChanged();
            emit currentRoleChanged();
            emit loginStateChanged();
            emit loginSuccess();
            
            return true;
        }
    } catch (const std::exception& e) {
        std::cerr << "[AUDIT] Login failed: " << e.what() << std::endl;
        QString error = QString("Ошибка входа: %1").arg(e.what());
        emit loginFailed(error);
    }
    
    m_isLoggedIn = false;
    emit loginFailed("Неверное имя пользователя или пароль");
    return false;
}

void SessionManager::logout() {
    if (!m_currentUser.isEmpty()) {
        std::cout << "[AUDIT] User '" << m_currentRole.toStdString() << "' logged out" << std::endl;
    }
    
    m_currentUser.clear();
    m_currentRole.clear();
    m_isLoggedIn = false;
    m_database.reset();
    
    emit currentUserChanged();
    emit currentRoleChanged();
    emit loginStateChanged();
}