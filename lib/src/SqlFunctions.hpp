#pragma once
#include <string>
#include <vector>
#include <optional>
#include <pqxx/pqxx>

namespace lib::SqlFunctions {

struct ProjectFilterResult {
    int project_id;
    int client_id;
    std::string project_name;
    std::string client_name;
    std::string client_type;
    std::string status;
    std::string type;
    std::string methodology;
    std::string start_date;
    std::string planned_end_date;
    std::string actual_end_date;
    double budget;
};

struct ProjectProfitabilityResult {
    int project_id;
    std::string project_name;
    double total_costs;
    double budget;
    double efficiency_ratio;
};

struct InfrastructureReportResult {
    int project_id;
    std::string project_name;
    std::string license_name;
    std::string expiry_date;
    double cost;
    std::string resource_provider;
    double resource_cost_per_hour;
};

struct ProjectTechnologyResult {
    std::string tech_name;
    int64_t project_count;
};

struct EmployeeFilterResult {
    int employee_id;
    int department_id;
    int position_id;
    std::string full_name;
    std::string department_name;
    std::string position_title;
    int age;
    int experience_years;
    float salary;
    std::string email;
    std::string phone;
    std::string cert_type;
    std::string cert_issue_date;

    std::vector<std::string> programming_languages;
    std::vector<std::string> frameworks;
    bool backend_exp;
    bool frontend_exp;
    bool mobile_exp;
    int dev_experience_years;

    std::vector<std::string> testing_types;
    std::vector<std::string> automation_tools;
    std::vector<std::string> certifications;

    std::string manager_cert_type;
    std::string manager_issue_date;
    std::optional<std::string> manager_expiry_date;
    std::optional<std::string> project_complexity;
};

struct ProjectTeamResult {
    int assignment_id;
    int employee_id;
    int project_id;
    int phase_id;
    std::string full_name;
    std::string position_title;
    std::string project_role;
    std::string project_name;
    std::string phase_name;
    std::string current_phase;
    std::string start_date;
    std::string end_date;
    int hours_allocated;
};

struct BugFilterResult {
    int bug_id;
    std::string project_name;
    std::string title;
    std::string description;
    std::string status;
    std::string severity;
    std::string found_by_name;
    std::string fixed_by_name;
    std::string created_at;
    std::string found_date;
    std::string fixed_date;
    int64_t total_count;
};

struct TestingEfficiencyResult {
    std::string project_name;
    int64_t bugs_in_test;
    int64_t bugs_after_release;
    double efficiency_ratio;
};

struct EmployeeWorkloadResult {
    int employee_id;
    std::string full_name;
    int project_id;
    std::string project_name;
    double total_hours;
    int64_t task_count;
};

struct ClientActivityResult {
    int client_id;
    std::string client_name;
    int64_t active_projects;
    double total_spend;
    std::string earliest_project_date;
    std::string latest_project_date;
};

struct ReleaseReportResult {
    std::string project_name;
    std::string version;
    std::string release_date;
    std::string changelog;
};

struct MonthlyFinancialReportResult {
    std::string project_name;
    double expenses_salary;
    double expenses_licenses;
    double expenses_cloud;
    double total_monthly_cost;
};

struct ProjectStatusReportResult {
    std::string project_name;
    std::string current_phase;
    int64_t tasks_logged;
    double team_hours_spent;
    int64_t critical_risks_count;
    std::string report_period;
};

}
