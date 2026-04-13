#pragma once
#include <string>
#include <vector>
#include <optional>
#include <variant>

namespace lib::Models {
    enum class DepartmentType {
        Development,
        Testing,
        Tech_support,
        Project_management,
        Administrative
    };

    enum class PositionCategory {
        Technical_staff,
        Management
    };

    enum class ClientType {
        Corporate,
        Government,
        Individual_entrepreneur
    };

    enum class LicenseType {
        Perpetual,
        Subscription
    };

    enum class ProjectStatus {
        In_development,
        Completed,
        Postponed,
        Closed_early
    };

    enum class ProjectMethodology {
        Waterfall,
        Scrum,
        Kanban,
        Hybrid
    };

    enum class ProjectType {
        Web_application,
        Mobile_application,
        Corporate_system,
        Microservice
    };

    enum class BugSeverity {
        Blocking,
        Critical,
        Medium,
        Low
    };

    enum class BugStatus {
        New,
        In_progress,
        Fixed,
        Verified
    };

    enum class DocType {
        Architecture,
        API,
        User_guide,
        Admin_guide,
        Specification
    };

    enum class CloudProvider {
        AWS,
        Azure,
        GCP,
        Yandex_Cloud,
        Other
    };

    struct Department {
        int department_id = 0;
        std::string name;
        DepartmentType type = DepartmentType::Development;
        std::string created_at;
    };

    struct Position {
        int position_id = 0;
        std::string title;
        PositionCategory category = PositionCategory::Technical_staff;
        double min_salary = 0.0;
        double max_salary = 0.0;
        std::string created_at;
    };

    struct Client {
        int client_id = 0;
        std::string name;
        ClientType type = ClientType::Corporate;
        std::optional<std::string> address;
        std::optional<std::string> phone;
        std::string email;
        std::string registration_date;
        std::string created_at;
    };

    struct SoftwareLicense {
        int license_id;
        std::string name;
        std::string vendor;
        LicenseType license_type;
        std::string purchase_date;
        std::optional<std::string> expiry_date;
        double cost;
        int seats;
        std::string created_at;
    };

    struct Employee {
        int employee_id = 0;
        std::string last_name;
        std::string first_name;
        std::optional<std::string> patronymic;
        std::string birth_date;
        std::optional<std::string> gender;
        std::optional<std::string> phone;
        std::string email;
        std::string hire_date;
        double salary = 0.0;
        bool has_children = false;
        int children_count = 0;
        int department_id = 0;
        int position_id = 0;
        bool is_active = false;
        std::string created_at;
        std::string updated_at;
    };

    struct DeveloperSpecialization {
        int employee_id = 0;
        std::vector<std::string> programming_languages;
        std::vector<std::string> frameworks;
        int experience_years = 0;
        bool backend_exp = false;
        bool frontend_exp = false;
        bool mobile_exp = false;
        std::string created_at;
    };

    struct TesterSpecialization {
        int employee_id;
        std::vector<std::string> testing_types;
        std::vector<std::string> automation_tools;
        std::vector<std::string> certifications;
        std::string created_at;
    };

    struct ManagerCertification {
        int certification_id;
        int employee_id;
        std::string cert_type;
        std::string issue_date;
        std::optional<std::string> expiry_date;
        std::optional<std::string> project_complexity;
        std::string created_at;
    };

    struct Project {
        int project_id;
        std::string name;
        std::optional<std::string> description;
        int client_id;
        std::string start_date;
        std::string planned_end_date;
        std::optional<std::string> actual_end_date;
        double budget;
        ProjectStatus status;
        std::optional<ProjectMethodology> methodology;
        ProjectType type;
        std::vector<std::string> tech_stack;
        bool is_active;
        std::string created_at;
        std::string updated_at;
    };

    struct ProjectPhase {
        int phase_id;
        int project_id;
        std::string name;
        std::string start_date;
        std::optional<std::string> end_date;
        int order_number;
        std::string created_at;
    };

    struct ProjectAssignment {
        int assignment_id;
        int employee_id;
        int project_id;
        std::optional<int> phase_id;
        std::string role;
        std::string start_date;
        std::optional<std::string> end_date;
        std::optional<int> hours_allocated;
        std::string created_at;
    };

    struct LicenseAllocation {
        int allocation_id;
        int license_id;
        std::optional<int> project_id;
        int allocated_seats;
        std::string allocation_date;
        std::string created_at;
    };

    struct CloudResource {
        int resource_id;
        int project_id;
        CloudProvider provider;
        std::string type;
        std::string configuration;
        double cost_per_hour;
        std::string created_at;
    };

    struct WorkLog {
        int log_id;
        int employee_id;
        int project_id;
        std::string task_description;
        std::string work_date;
        double hours_spent;
        std::string created_at;
    };

    struct Bug {
        int bug_id;
        int project_id;
        std::string title;
        std::string description;
        BugSeverity severity;
        BugStatus status;
        std::string found_date;
        std::optional<std::string> fixed_date;
        std::optional<int> found_by;
        std::optional<int> fixed_by;
        std::string created_at;
    };

    struct Release {
        int release_id;
        int project_id;
        std::string version;
        std::string release_date;
        std::optional<std::string> changelog;
        int fixed_bugs_count;
        std::string created_at;
    };

    struct Documentation {
        int doc_id;
        int project_id;
        DocType type;
        int author_id;
        std::string creation_date;
        std::string last_update;
        std::optional<std::string> storage_path;
        std::string created_at;
    };

    struct ProjectSpecification {
        int spec_id;
        int project_id;
        std::string document_text;
        int version;
        std::string created_at;
        int updated_by;
    };

    using Model = std::variant<Department, Position, Client, Employee, SoftwareLicense, DeveloperSpecialization, TesterSpecialization, ManagerCertification, Project, ProjectPhase, ProjectAssignment, LicenseAllocation, CloudResource, WorkLog, Bug, Release, Documentation, ProjectSpecification>;
}
