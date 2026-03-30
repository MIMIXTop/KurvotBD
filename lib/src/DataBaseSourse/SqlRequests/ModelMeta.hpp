#pragma once
#include <string>
#include <tuple>
#include <string_view>

#include <pqxx/pqxx>

#include "../../Models.hpp"
#include "Converters.hpp"
#include "ToParams.hpp"

namespace lib::Meta {
    template<typename T>
    struct ModelMeta;

    // ─── Department ───────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::Department> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO department (name, type) VALUES ($1, $2)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM department WHERE department_id = $1";
        };

        static auto extract(const Models::Department &m) {
            return std::make_tuple(
                Converters::toParams(m.name),
                Converters::toParams(m.type)
            );
        }

        static Models::Department fromRow(const pqxx::row &row) {
            return Models::Department{
                .id = row["department_id"].as<int>(),
                .name = row["name"].as<std::string>(),
                .type = Converters::parseDepartmentType(row["type"].as<std::string>()),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── Position ─────────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::Position> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO position (title, category, min_salary, max_salary) "
                    "VALUES ($1, $2, $3, $4)";

            static constexpr std::string_view selectById =
                    "SELECT * from position WHERE position_id = $1";
        };

        static auto extract(const Models::Position &m) {
            return std::make_tuple(
                Converters::toParams(m.title),
                Converters::toParams(m.category),
                Converters::toParams(m.min_salary),
                Converters::toParams(m.max_salary)
            );
        }

        static Models::Position fromRow(const pqxx::row &row) {
            return Models::Position{
                .id = row["position_id"].as<int>(),
                .title = row["title"].as<std::string>(),
                .category = Converters::parsePositionCategory(row["category"].as<std::string>()),
                .min_salary = row["min_salary"].as<double>(),
                .max_salary = row["max_salary"].as<double>(),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── Client ───────────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::Client> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO client (name, type, address, phone, email, registration_date) "
                    "VALUES ($1, $2, $3, $4, $5, $6)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM client WHERE client_id = $1";
        };

        static auto extract(const Models::Client &m) {
            return std::make_tuple(
                Converters::toParams(m.name),
                Converters::toParams(m.type),
                m.address,
                m.phone,
                Converters::toParams(m.email),
                Converters::toParams(m.registration_date)
            );
        }

        static Models::Client fromRow(const pqxx::row &row) {
            return Models::Client{
                .id = row["client_id"].as<int>(),
                .name = row["name"].as<std::string>(),
                .type = Converters::parseClientType(row["type"].as<std::string>()),
                .address = row["address"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["address"].as<std::string>()),
                .phone = row["phone"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["phone"].as<std::string>()),
                .email = row["email"].as<std::string>(),
                .registration_date = row["registration_date"].as<std::string>(),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── SoftwareLicense ──────────────────────────────────────────
    template<>
    struct ModelMeta<Models::SoftwareLicense> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO softwarelicense (name, vendor, license_type, purchase_date, expiry_date, cost, seats) "
                    "VALUES ($1, $2, $3, $4, $5, $6, $7)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM softwarelicense WHERE license_id = $1";
        };

        static auto extract(const Models::SoftwareLicense &m) {
            return std::make_tuple(
                Converters::toParams(m.name),
                Converters::toParams(m.vendor),
                Converters::toParams(m.type),
                Converters::toParams(m.purchase_date),
                m.expiry_date,
                Converters::toParams(m.cost),
                Converters::toParams(m.seats)
            );
        }

        static Models::SoftwareLicense fromRow(const pqxx::row &row) {
            return Models::SoftwareLicense{
                .id = row["license_id"].as<int>(),
                .name = row["name"].as<std::string>(),
                .vendor = row["vendor"].as<std::string>(),
                .type = Converters::parseLicenseType(row["license_type"].as<std::string>()),
                .purchase_date = row["purchase_date"].as<std::string>(),
                .expiry_date = row["expiry_date"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["expiry_date"].as<std::string>()),
                .cost = row["cost"].as<double>(),
                .seats = row["seats"].as<int>(),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── Employee ─────────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::Employee> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO employee (last_name, first_name, patronymic, birth_date, gender, "
                    "phone, email, hire_date, salary, has_children, children_count, "
                    "department_id, position_id, is_active) "
                    "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM employee WHERE employee_id = $1";
        };

        static auto extract(const Models::Employee &m) {
            return std::make_tuple(
                Converters::toParams(m.last_name),
                Converters::toParams(m.first_name),
                m.patronymic,
                Converters::toParams(m.birth_date),
                m.gender,
                m.phone,
                Converters::toParams(m.email),
                Converters::toParams(m.hire_date),
                Converters::toParams(m.salary),
                Converters::toParams(m.has_children),
                Converters::toParams(m.children_count),
                Converters::toParams(m.department_id),
                Converters::toParams(m.position_id),
                Converters::toParams(m.is_active)
            );
        }

        static Models::Employee fromRow(const pqxx::row &row) {
            return Models::Employee{
                .id = row["employee_id"].as<int>(),
                .last_name = row["last_name"].as<std::string>(),
                .first_name = row["first_name"].as<std::string>(),
                .patronymic = row["patronymic"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["patronymic"].as<std::string>()),
                .birth_date = row["birth_date"].as<std::string>(),
                .gender = row["gender"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["gender"].as<std::string>()),
                .phone = row["phone"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["phone"].as<std::string>()),
                .email = row["email"].as<std::string>(),
                .hire_date = row["hire_date"].as<std::string>(),
                .salary = row["salary"].as<double>(),
                .has_children = row["has_children"].as<bool>(),
                .children_count = row["children_count"].as<int>(),
                .department_id = row["department_id"].as<int>(),
                .position_id = row["position_id"].as<int>(),
                .is_active = row["is_active"].as<bool>(),
                .created_at = row["created_at"].as<std::string>(),
                .updated_at = row["updated_at"].as<std::string>()
            };
        }
    };

    // ─── DeveloperSpecialization ──────────────────────────────────
    template<>
    struct ModelMeta<Models::DeveloperSpecialization> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO developerspecialization (employee_id, programming_languages, "
                    "frameworks, experience_years, backend_exp, frontend_exp, mobile_exp) "
                    "VALUES ($1, $2, $3, $4, $5, $6, $7)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM developerspecialization WHERE employee_id = $1";
        };

        static auto extract(const Models::DeveloperSpecialization &m) {
            return std::make_tuple(
                Converters::toParams(m.employee_id),
                Converters::toParams(m.programming_languages),
                Converters::toParams(m.frameworks),
                Converters::toParams(m.experience_years),
                Converters::toParams(m.backend_exp),
                Converters::toParams(m.frontend_exp),
                Converters::toParams(m.mobile_exp)
            );
        }

        static Models::DeveloperSpecialization fromRow(const pqxx::row &row) {
            return Models::DeveloperSpecialization{
                .employee_id = row["employee_id"].as<int>(),
                .programming_languages = Converters::parsePgArray(
                    row["programming_languages"].as<std::string>()),
                .frameworks = Converters::parsePgArray(
                    row["frameworks"].as<std::string>()),
                .experience_years = row["experience_years"].as<int>(),
                .backend_exp = row["backend_exp"].as<bool>(),
                .frontend_exp = row["frontend_exp"].as<bool>(),
                .mobile_exp = row["mobile_exp"].as<bool>(),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── TesterSpecialization ─────────────────────────────────────
    template<>
    struct ModelMeta<Models::TesterSpecialization> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO testerspecialization (employee_id, testing_types, "
                    "automation_tools, certifications) "
                    "VALUES ($1, $2, $3, $4)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM testerspecialization WHERE employee_id = $1";
        };

        static auto extract(const Models::TesterSpecialization &m) {
            return std::make_tuple(
                Converters::toParams(m.employee_id),
                Converters::toParams(m.testing_types),
                Converters::toParams(m.automation_tools),
                Converters::toParams(m.certifications)
            );
        }

        static Models::TesterSpecialization fromRow(const pqxx::row &row) {
            return Models::TesterSpecialization{
                .employee_id = row["employee_id"].as<int>(),
                .testing_types = Converters::parsePgArray(
                    row["testing_types"].as<std::string>()),
                .automation_tools = Converters::parsePgArray(
                    row["automation_tools"].as<std::string>()),
                .certifications = Converters::parsePgArray(
                    row["certifications"].as<std::string>()),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── ManagerCertification ─────────────────────────────────────
    template<>
    struct ModelMeta<Models::ManagerCertification> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO managercertification (employee_id, cert_type, issue_date, "
                    "expiry_date, project_complexity) "
                    "VALUES ($1, $2, $3, $4, $5)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM managercertification WHERE certification_id = $1";
        };

        static auto extract(const Models::ManagerCertification &m) {
            return std::make_tuple(
                Converters::toParams(m.employee_id),
                Converters::toParams(m.cert_type),
                Converters::toParams(m.issue_date),
                m.expiry_date,
                m.project_complexity
            );
        }

        static Models::ManagerCertification fromRow(const pqxx::row &row) {
            return Models::ManagerCertification{
                .id = row["certification_id"].as<int>(),
                .employee_id = row["employee_id"].as<int>(),
                .cert_type = row["cert_type"].as<std::string>(),
                .issue_date = row["issue_date"].as<std::string>(),
                .expiry_date = row["expiry_date"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["expiry_date"].as<std::string>()),
                .project_complexity = row["project_complexity"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["project_complexity"].as<std::string>()),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── Project ──────────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::Project> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO project (name, description, client_id, start_date, "
                    "planned_end_date, actual_end_date, budget, status, methodology, "
                    "type, tech_stack, is_active) "
                    "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM project WHERE project_id = $1";
        };

        static auto extract(const Models::Project &m) {
            std::optional<std::string> methodology;
            if (m.methodology.has_value())
                methodology = Converters::toString(m.methodology.value());

            return std::make_tuple(
                Converters::toParams(m.name),
                m.description,
                Converters::toParams(m.client_id),
                Converters::toParams(m.start_date),
                Converters::toParams(m.planned_end_date),
                m.actual_end_date,
                Converters::toParams(m.budget),
                Converters::toParams(m.status),
                methodology,
                Converters::toParams(m.type),
                Converters::toParams(m.tech_stack),
                Converters::toParams(m.is_active)
            );
        }

        static Models::Project fromRow(const pqxx::row &row) {
            std::optional<Models::ProjectMethodology> methodology;
            if (!row["methodology"].is_null())
                methodology = Converters::parseProjectMethodology(
                    row["methodology"].as<std::string>());

            return Models::Project{
                .id = row["project_id"].as<int>(),
                .name = row["name"].as<std::string>(),
                .description = row["description"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["description"].as<std::string>()),
                .client_id = row["client_id"].as<int>(),
                .start_date = row["start_date"].as<std::string>(),
                .planned_end_date = row["planned_end_date"].as<std::string>(),
                .actual_end_date = row["actual_end_date"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["actual_end_date"].as<std::string>()),
                .budget = row["budget"].as<double>(),
                .status = Converters::parseProjectStatus(row["status"].as<std::string>()),
                .methodology = methodology,
                .type = Converters::parseProjectType(row["type"].as<std::string>()),
                .tech_stack = Converters::parsePgArray(
                    row["tech_stack"].as<std::string>()),
                .is_active = row["is_active"].as<bool>(),
                .created_at = row["created_at"].as<std::string>(),
                .updated_at = row["updated_at"].as<std::string>()
            };
        }
    };

    // ─── ProjectPhase ─────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::ProjectPhase> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO projectphase (project_id, name, start_date, end_date, order_number) "
                    "VALUES ($1, $2, $3, $4, $5)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM projectphase WHERE phase_id = $1";
        };

        static auto extract(const Models::ProjectPhase &m) {
            return std::make_tuple(
                Converters::toParams(m.project_id),
                Converters::toParams(m.name),
                Converters::toParams(m.start_date),
                m.end_date,
                Converters::toParams(m.order_number)
            );
        }

        static Models::ProjectPhase fromRow(const pqxx::row &row) {
            return Models::ProjectPhase{
                .id = row["phase_id"].as<int>(),
                .project_id = row["project_id"].as<int>(),
                .name = row["name"].as<std::string>(),
                .start_date = row["start_date"].as<std::string>(),
                .end_date = row["end_date"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["end_date"].as<std::string>()),
                .order_number = row["order_number"].as<int>(),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── ProjectAssignment ────────────────────────────────────────
    template<>
    struct ModelMeta<Models::ProjectAssignment> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO projectassignment (employee_id, project_id, phase_id, "
                    "role, start_date, end_date, hours_allocated) "
                    "VALUES ($1, $2, $3, $4, $5, $6, $7)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM projectassignment WHERE assignment_id = $1";
        };

        static auto extract(const Models::ProjectAssignment &m) {
            return std::make_tuple(
                Converters::toParams(m.employee_id),
                Converters::toParams(m.project_id),
                m.phase_id,
                Converters::toParams(m.role),
                Converters::toParams(m.start_date),
                m.end_date,
                m.hours_allocated
            );
        }

        static Models::ProjectAssignment fromRow(const pqxx::row &row) {
            return Models::ProjectAssignment{
                .id = row["assignment_id"].as<int>(),
                .employee_id = row["employee_id"].as<int>(),
                .project_id = row["project_id"].as<int>(),
                .phase_id = row["phase_id"].is_null()
                    ? std::nullopt
                    : std::optional<int>(row["phase_id"].as<int>()),
                .role = row["role"].as<std::string>(),
                .start_date = row["start_date"].as<std::string>(),
                .end_date = row["end_date"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["end_date"].as<std::string>()),
                .hours_allocated = row["hours_allocated"].is_null()
                    ? std::nullopt
                    : std::optional<int>(row["hours_allocated"].as<int>()),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── LicenseAllocation ────────────────────────────────────────
    template<>
    struct ModelMeta<Models::LicenseAllocation> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO licenseallocation (license_id, project_id, allocated_seats, allocation_date) "
                    "VALUES ($1, $2, $3, $4)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM licenseallocation WHERE allocation_id = $1";
        };

        static auto extract(const Models::LicenseAllocation &m) {
            return std::make_tuple(
                Converters::toParams(m.license_id),
                m.project_id,
                Converters::toParams(m.allocated_seats),
                Converters::toParams(m.allocation_date)
            );
        }

        static Models::LicenseAllocation fromRow(const pqxx::row &row) {
            return Models::LicenseAllocation{
                .id = row["allocation_id"].as<int>(),
                .license_id = row["license_id"].as<int>(),
                .project_id = row["project_id"].is_null()
                    ? std::nullopt
                    : std::optional<int>(row["project_id"].as<int>()),
                .allocated_seats = row["allocated_seats"].as<int>(),
                .allocation_date = row["allocation_date"].as<std::string>(),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── CloudResource ────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::CloudResource> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO cloudresource (project_id, provider, type, configuration, cost_per_hour) "
                    "VALUES ($1, $2, $3, $4, $5)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM cloudresource WHERE resource_id = $1";
        };

        static auto extract(const Models::CloudResource &m) {
            return std::make_tuple(
                Converters::toParams(m.project_id),
                Converters::toParams(m.provider),
                Converters::toParams(m.type),
                m.configuration,
                Converters::toParams(m.cost_per_hour)
            );
        }

        static Models::CloudResource fromRow(const pqxx::row &row) {
            return Models::CloudResource{
                .id = row["resource_id"].as<int>(),
                .project_id = row["project_id"].as<int>(),
                .provider = Converters::parseCloudProvider(row["provider"].as<std::string>()),
                .type = row["type"].as<std::string>(),
                .configuration = row["configuration"].as<std::string>(),
                .cost_per_hour = row["cost_per_hour"].as<double>(),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── WorkLog ──────────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::WorkLog> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO worklog (employee_id, project_id, task_description, work_date, hours_spent) "
                    "VALUES ($1, $2, $3, $4, $5)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM worklog WHERE log_id = $1";
        };

        static auto extract(const Models::WorkLog &m) {
            return std::make_tuple(
                Converters::toParams(m.employee_id),
                Converters::toParams(m.project_id),
                Converters::toParams(m.task_description),
                Converters::toParams(m.work_date),
                Converters::toParams(m.hours_spent)
            );
        }

        static Models::WorkLog fromRow(const pqxx::row &row) {
            return Models::WorkLog{
                .id = row["log_id"].as<int>(),
                .employee_id = row["employee_id"].as<int>(),
                .project_id = row["project_id"].as<int>(),
                .task_description = row["task_description"].as<std::string>(),
                .work_date = row["work_date"].as<std::string>(),
                .hours_spent = row["hours_spent"].as<double>(),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── Bug ──────────────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::Bug> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO bug (project_id, title, description, severity, status, "
                    "found_date, fixed_date, found_by, fixed_by) "
                    "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)";

            static constexpr std::string_view selectById =
                    "SELECT * FROM bug WHERE bug_id = $1";
        };

        static auto extract(const Models::Bug &m) {
            return std::make_tuple(
                Converters::toParams(m.project_id),
                Converters::toParams(m.title),
                Converters::toParams(m.description),
                Converters::toParams(m.severity),
                Converters::toParams(m.status),
                Converters::toParams(m.found_date),
                m.fixed_date,
                m.found_by,
                m.fixed_by
            );
        }

        static Models::Bug fromRow(const pqxx::row &row) {
            return Models::Bug{
                .id = row["bug_id"].as<int>(),
                .project_id = row["project_id"].as<int>(),
                .title = row["title"].as<std::string>(),
                .description = row["description"].as<std::string>(),
                .severity = Converters::parseBugSeverity(row["severity"].as<std::string>()),
                .status = Converters::parseBugStatus(row["status"].as<std::string>()),
                .found_date = row["found_date"].as<std::string>(),
                .fixed_date = row["fixed_date"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["fixed_date"].as<std::string>()),
                .found_by = row["found_by"].is_null()
                    ? std::nullopt
                    : std::optional<int>(row["found_by"].as<int>()),
                .fixed_by = row["fixed_by"].is_null()
                    ? std::nullopt
                    : std::optional<int>(row["fixed_by"].as<int>()),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── Release ──────────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::Release> {
        struct Query {
            static constexpr std::string_view insert =
                    "INSERT INTO release (project_id, version, release_date, changelog, fixed_bugs_count) "
                    "VALUES ($1, $2, $3, $4, $5)";

            static constexpr std::string_view selectById =
                "SELECT * FROM release WHERE release_id = $1";
        };

        static auto extract(const Models::Release &m) {
            return std::make_tuple(
                Converters::toParams(m.project_id),
                Converters::toParams(m.version),
                Converters::toParams(m.release_date),
                m.changelog,
                Converters::toParams(m.fixed_bugs_count)
            );
        }

        static Models::Release fromRow(const pqxx::row &row) {
            return Models::Release{
                .id = row["release_id"].as<int>(),
                .project_id = row["project_id"].as<int>(),
                .version = row["version"].as<std::string>(),
                .release_date = row["release_date"].as<std::string>(),
                .changelog = row["changelog"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["changelog"].as<std::string>()),
                .fixed_bugs_count = row["fixed_bugs_count"].as<int>(),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };

    // ─── Documentation ────────────────────────────────────────────
    template<>
    struct ModelMeta<Models::Documentation> {
        struct Query {
            static constexpr std::string_view insert =
                "INSERT INTO documentation (project_id, type, author_id, creation_date, "
                "last_update, storage_path) "
                "VALUES ($1, $2, $3, $4, $5, $6)";

            static constexpr std::string_view selectById =
                "SELECT * FROM documentation WHERE doc_id = $1";
        };

        static auto extract(const Models::Documentation &m) {
            return std::make_tuple(
                Converters::toParams(m.project_id),
                Converters::toParams(m.type),
                Converters::toParams(m.author_id),
                Converters::toParams(m.creation_date),
                m.last_update,
                m.storage_path
            );
        }

        static Models::Documentation fromRow(const pqxx::row &row) {
            return Models::Documentation{
                .id = row["doc_id"].as<int>(),
                .project_id = row["project_id"].as<int>(),
                .type = Converters::parseDocType(row["type"].as<std::string>()),
                .author_id = row["author_id"].as<int>(),
                .creation_date = row["creation_date"].as<std::string>(),
                .last_update = row["last_update"].as<std::string>(),
                .storage_path = row["storage_path"].is_null()
                    ? std::nullopt
                    : std::optional<std::string>(row["storage_path"].as<std::string>()),
                .created_at = row["created_at"].as<std::string>()
            };
        }
    };
}
