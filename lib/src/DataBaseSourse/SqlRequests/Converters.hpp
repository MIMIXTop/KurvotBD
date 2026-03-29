#pragma once
#include <string>
#include <vector>

#include "../../Models.hpp"

namespace lib::Converters {

    inline std::string toString(lib::Models::DepartmentType t) {
        switch (t) {
            case lib::Models::DepartmentType::Development:        return "разработка";
            case lib::Models::DepartmentType::Testing:            return "тестирование";
            case lib::Models::DepartmentType::Tech_support:       return "техподдержка";
            case lib::Models::DepartmentType::Project_management: return "управление_проектами";
            case lib::Models::DepartmentType::Administrative:     return "административный";
        }
        return "";
    }

    inline std::string toString(lib::Models::PositionCategory t) {
        switch (t) {
            case lib::Models::PositionCategory::Technical_staff: return "технический_персонал";
            case lib::Models::PositionCategory::Management:      return "руководящий_состав";
        }
        return "";
    }

    inline std::string toString(lib::Models::ClientType t) {
        switch (t) {
            case lib::Models::ClientType::Corporate:               return "корпоративный";
            case lib::Models::ClientType::Government:              return "государственный";
            case lib::Models::ClientType::Individual_entrepreneur: return "ИП";
        }
        return "";
    }

    inline std::string toString(lib::Models::LicenseType t) {
        switch (t) {
            case lib::Models::LicenseType::Perpetual:     return "бессрочная";
            case lib::Models::LicenseType::Subscription:  return "подписка";
        }
        return "";
    }

    inline std::string toString(lib::Models::ProjectStatus t) {
        switch (t) {
            case lib::Models::ProjectStatus::In_development: return "в_разработке";
            case lib::Models::ProjectStatus::Completed:      return "завершен";
            case lib::Models::ProjectStatus::Postponed:      return "отложен";
            case lib::Models::ProjectStatus::Closed_early:   return "закрыт_досрочно";
        }
        return "";
    }

    inline std::string toString(lib::Models::ProjectMethodology t) {
        switch (t) {
            case lib::Models::ProjectMethodology::Waterfall: return "Waterfall";
            case lib::Models::ProjectMethodology::Scrum:     return "Scrum";
            case lib::Models::ProjectMethodology::Kanban:    return "Kanban";
            case lib::Models::ProjectMethodology::Hybrid:    return "гибридная";
        }
        return "";
    }

    inline std::string toString(lib::Models::ProjectType t) {
        switch (t) {
            case lib::Models::ProjectType::Web_application:       return "веб-приложение";
            case lib::Models::ProjectType::Mobile_application:    return "мобильное_приложение";
            case lib::Models::ProjectType::Corporate_system:      return "корпоративная_система";
            case lib::Models::ProjectType::Microservice:          return "микросервис";
        }
        return "";
    }

    inline std::string toString(lib::Models::BugSeverity t) {
        switch (t) {
            case lib::Models::BugSeverity::Blocking: return "блокирующая";
            case lib::Models::BugSeverity::Critical: return "критическая";
            case lib::Models::BugSeverity::Medium:   return "средняя";
            case lib::Models::BugSeverity::Low:      return "низкая";
        }
        return "";
    }

    inline std::string toString(lib::Models::BugStatus t) {
        switch (t) {
            case lib::Models::BugStatus::New:         return "новая";
            case lib::Models::BugStatus::In_progress: return "в_работе";
            case lib::Models::BugStatus::Fixed:       return "исправлена";
            case lib::Models::BugStatus::Verified:    return "проверена";
        }
        return "";
    }

    inline std::string toString(lib::Models::DocType t) {
        switch (t) {
            case lib::Models::DocType::Architecture:  return "архитектура";
            case lib::Models::DocType::API:           return "API";
            case lib::Models::DocType::User_guide:    return "руководство_пользователя";
            case lib::Models::DocType::Admin_guide:   return "руководство_администратора";
            case lib::Models::DocType::Specification: return "спецификация";
        }
        return "";
    }

    inline std::string toString(lib::Models::CloudProvider t) {
        switch (t) {
            case lib::Models::CloudProvider::AWS:          return "AWS";
            case lib::Models::CloudProvider::Azure:        return "Azure";
            case lib::Models::CloudProvider::GCP:          return "GCP";
            case lib::Models::CloudProvider::Yandex_Cloud: return "Yandex_Cloud";
            case lib::Models::CloudProvider::Other:        return "другой";
        }
        return "";
    }

    inline std::string toPgArray(const std::vector<std::string>& vec) {
        std::string result = "{";
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) result += ",";
            result += vec[i];
        }
        result += "}";
        return result;
    }

}
