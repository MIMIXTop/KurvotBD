#pragma once
#include <string>
#include <vector>

#include "../../Models.hpp"

namespace lib::Converters {
    inline std::string toString(Models::DepartmentType t) {
        switch (t) {
            case Models::DepartmentType::Development: return "разработка";
            case Models::DepartmentType::Testing: return "тестирование";
            case Models::DepartmentType::Tech_support: return "техподдержка";
            case Models::DepartmentType::Project_management: return "управление_проектами";
            case Models::DepartmentType::Administrative: return "административный";
        }
        return "";
    }

    inline std::string toString(Models::PositionCategory t) {
        switch (t) {
            case Models::PositionCategory::Technical_staff: return "технический_персонал";
            case Models::PositionCategory::Management: return "руководящий_состав";
        }
        return "";
    }

    inline std::string toString(Models::ClientType t) {
        switch (t) {
            case Models::ClientType::Corporate: return "корпоративный";
            case Models::ClientType::Government: return "государственный";
            case Models::ClientType::Individual_entrepreneur: return "ИП";
        }
        return "";
    }

    inline std::string toString(Models::LicenseType t) {
        switch (t) {
            case Models::LicenseType::Perpetual: return "бессрочная";
            case Models::LicenseType::Subscription: return "подписка";
        }
        return "";
    }

    inline std::string toString(Models::ProjectStatus t) {
        switch (t) {
            case Models::ProjectStatus::In_development: return "в_разработке";
            case Models::ProjectStatus::Completed: return "завершен";
            case Models::ProjectStatus::Postponed: return "отложен";
            case Models::ProjectStatus::Closed_early: return "закрыт_досрочно";
        }
        return "";
    }

    inline std::string toString(Models::ProjectMethodology t) {
        switch (t) {
            case Models::ProjectMethodology::Waterfall: return "Waterfall";
            case Models::ProjectMethodology::Scrum: return "Scrum";
            case Models::ProjectMethodology::Kanban: return "Kanban";
            case Models::ProjectMethodology::Hybrid: return "гибридная";
        }
        return "";
    }

    inline std::string toString(Models::ProjectType t) {
        switch (t) {
            case Models::ProjectType::Web_application: return "веб-приложение";
            case Models::ProjectType::Mobile_application: return "мобильное_приложение";
            case Models::ProjectType::Corporate_system: return "корпоративная_система";
            case Models::ProjectType::Microservice: return "микросервис";
        }
        return "";
    }

    inline std::string toString(Models::BugSeverity t) {
        switch (t) {
            case Models::BugSeverity::Blocking: return "блокирующая";
            case Models::BugSeverity::Critical: return "критическая";
            case Models::BugSeverity::Medium: return "средняя";
            case Models::BugSeverity::Low: return "низкая";
        }
        return "";
    }

    inline std::string toString(Models::BugStatus t) {
        switch (t) {
            case Models::BugStatus::New: return "новая";
            case Models::BugStatus::In_progress: return "в_работе";
            case Models::BugStatus::Fixed: return "исправлена";
            case Models::BugStatus::Verified: return "проверена";
        }
        return "";
    }

    inline std::string toString(Models::DocType t) {
        switch (t) {
            case Models::DocType::Architecture: return "архитектура";
            case Models::DocType::API: return "API";
            case Models::DocType::User_guide: return "руководство_пользователя";
            case Models::DocType::Admin_guide: return "руководство_администратора";
            case Models::DocType::Specification: return "спецификация";
        }
        return "";
    }

    inline std::string toString(Models::CloudProvider t) {
        switch (t) {
            case Models::CloudProvider::AWS: return "AWS";
            case Models::CloudProvider::Azure: return "Azure";
            case Models::CloudProvider::GCP: return "GCP";
            case Models::CloudProvider::Yandex_Cloud: return "Yandex_Cloud";
            case Models::CloudProvider::Other: return "другой";
        }
        return "";
    }

    inline std::string toPgArray(const std::vector<std::string> &vec) {
        std::string result = "{";
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) result += ",";
            result += vec[i];
        }
        result += "}";
        return result;
    }

    inline Models::DepartmentType parseDepartmentType(const std::string& s) {
        if (s == "разработка")           return Models::DepartmentType::Development;
        if (s == "тестирование")         return Models::DepartmentType::Testing;
        if (s == "техподдержка")         return Models::DepartmentType::Tech_support;
        if (s == "управление_проектами")  return Models::DepartmentType::Project_management;
        return Models::DepartmentType::Administrative;
    }

    inline Models::PositionCategory parsePositionCategory(const std::string& s) {
        if (s == "руководящий_состав") return Models::PositionCategory::Management;
        return Models::PositionCategory::Technical_staff;
    }

    inline Models::ClientType parseClientType(const std::string& s) {
        if (s == "государственный") return Models::ClientType::Government;
        if (s == "ИП")             return Models::ClientType::Individual_entrepreneur;
        return Models::ClientType::Corporate;
    }

    inline Models::LicenseType parseLicenseType(const std::string& s) {
        if (s == "подписка") return Models::LicenseType::Subscription;
        return Models::LicenseType::Perpetual;
    }

    inline Models::ProjectStatus parseProjectStatus(const std::string& s) {
        if (s == "завершен")         return Models::ProjectStatus::Completed;
        if (s == "отложен")         return Models::ProjectStatus::Postponed;
        if (s == "закрыт_досрочно")  return Models::ProjectStatus::Closed_early;
        return Models::ProjectStatus::In_development;
    }

    inline Models::ProjectMethodology parseProjectMethodology(const std::string& s) {
        if (s == "Scrum")          return Models::ProjectMethodology::Scrum;
        if (s == "Kanban")         return Models::ProjectMethodology::Kanban;
        if (s == "гибридная")      return Models::ProjectMethodology::Hybrid;
        return Models::ProjectMethodology::Waterfall;
    }

    inline Models::ProjectType parseProjectType(const std::string& s) {
        if (s == "мобильное_приложение")    return Models::ProjectType::Mobile_application;
        if (s == "корпоративная_система")    return Models::ProjectType::Corporate_system;
        if (s == "микросервис")             return Models::ProjectType::Microservice;
        return Models::ProjectType::Web_application;
    }

    inline Models::BugSeverity parseBugSeverity(const std::string& s) {
        if (s == "блокирующая") return Models::BugSeverity::Blocking;
        if (s == "критическая") return Models::BugSeverity::Critical;
        if (s == "средняя")    return Models::BugSeverity::Medium;
        return Models::BugSeverity::Low;
    }

    inline Models::BugStatus parseBugStatus(const std::string& s) {
        if (s == "в_работе")   return Models::BugStatus::In_progress;
        if (s == "исправлена") return Models::BugStatus::Fixed;
        if (s == "проверена")  return Models::BugStatus::Verified;
        return Models::BugStatus::New;
    }

    inline Models::DocType parseDocType(const std::string& s) {
        if (s == "API")                       return Models::DocType::API;
        if (s == "руководство_пользователя")  return Models::DocType::User_guide;
        if (s == "руководство_администратора") return Models::DocType::Admin_guide;
        if (s == "спецификация")              return Models::DocType::Specification;
        return Models::DocType::Architecture;
    }

    inline Models::CloudProvider parseCloudProvider(const std::string& s) {
        if (s == "Azure")        return Models::CloudProvider::Azure;
        if (s == "GCP")          return Models::CloudProvider::GCP;
        if (s == "Yandex_Cloud") return Models::CloudProvider::Yandex_Cloud;
        if (s == "другой")       return Models::CloudProvider::Other;
        return Models::CloudProvider::AWS;
    }


    inline std::vector<std::string> parsePgArray(const std::string& s) {
        std::vector<std::string> result;
        if (s.size() < 2) return result;

        std::string inner = s.substr(1, s.size() - 2);
        if (inner.empty()) return result;

        std::string current;
        for (char c : inner) {
            if (c == ',') {
                result.push_back(current);
                current.clear();
            } else {
                current += c;
            }
        }
        result.push_back(current);
        return result;
    }

}
