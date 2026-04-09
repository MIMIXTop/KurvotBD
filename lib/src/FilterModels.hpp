#pragma once

#include <string>
#include <vector>
#include <optional>
#include <variant>
#include <cstdint>

namespace lib::FilterModels {

    struct ProjectsByFilters {
        std::optional<std::vector<std::string>> statuses;
        std::optional<std::string> completed_from;
        std::optional<std::string> completed_to;
        std::optional<int> client_id;
        std::optional<std::string> methodology_filter;
        std::optional<std::vector<std::string>> client_types;
        std::optional<double> min_budget;
        std::optional<double> max_budget;
        std::optional<bool> is_active = true;
    };

    struct ProjectProfitability {
        std::optional<std::string> from;
        std::optional<std::string> to;
        std::optional<bool> is_active;
    };

    struct InfrastructureReport {
        std::optional<bool> is_active = true;
        std::optional<int> project_id;
    };

    struct ProjectTechnologies {
        std::optional<bool> is_active = true;
        std::optional<std::string> tech_name;
    };

    struct BugsByFilter {
        std::optional<int> project_id;
        std::optional<std::vector<std::string>> bug_status;
        std::optional<std::vector<std::string>> bug_severity;
        std::optional<std::string> found_from;
        std::optional<std::string> found_to;
    };

    struct TestingEfficiency {
        std::optional<int> project_id;
        std::optional<std::string> from;
        std::optional<std::string> to;
    };

    struct EmployeeByFilters {
        std::optional<std::vector<int>> department_ids;
        std::optional<std::vector<std::string>> position_titles;
        std::optional<int> min_experience;
        std::optional<int> max_experience;
        std::optional<int> min_age;
        std::optional<int> max_age;
        std::optional<double> min_salary;
        std::optional<double> max_salary;
        std::optional<std::string> cert_type;
        std::optional<bool> is_active = true;
    };

    struct ProjectTeam {
        std::optional<int> project_id;
        std::optional<int> phase_id;
        std::optional<std::vector<std::string>> roles;
        std::optional<bool> is_active = true;
    };

    struct ClientActivity {
        std::optional<std::string> from;
        std::optional<std::string> to;
    };

    struct ReleaseReport {
        std::optional<std::string> from;
        std::optional<std::string> to;
    };

    struct EmployeeWorkload {
        std::optional<std::vector<int>> employee_ids;
        std::optional<std::vector<int>> project_ids;
        std::optional<std::string> from;
        std::optional<std::string> to;
    };

    using Filter = std::variant<
        ProjectsByFilters,
        ProjectProfitability,
        InfrastructureReport,
        ProjectTechnologies,
        BugsByFilter,
        TestingEfficiency,
        EmployeeByFilters,
        ProjectTeam,
        ClientActivity,
        ReleaseReport,
        EmployeeWorkload
    >;

}
