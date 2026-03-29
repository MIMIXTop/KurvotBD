#pragma once
#include <string>
#include <tuple>
#include <string_view>

#include "../../Models.hpp"
#include "Converters.hpp"
#include "ToParams.hpp"

namespace lib::Meta {

    template <typename T>
    struct ModelMeta;

    // ─── Department ───────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::Department> {
        static constexpr std::string_view query =
            "INSERT INTO department (name, type) VALUES ($1, $2)";

        static auto extract(const Models::Department& m) {
            return std::make_tuple(
                Converters::toParams(m.name),
                Converters::toParams(m.type)
            );
        }
    };

    // ─── Position ─────────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::Position> {
        static constexpr std::string_view query =
            "INSERT INTO position (title, category, min_salary, max_salary) "
            "VALUES ($1, $2, $3, $4)";

        static auto extract(const Models::Position& m) {
            return std::make_tuple(
                Converters::toParams(m.title),
                Converters::toParams(m.category),
                Converters::toParams(m.min_salary),
                Converters::toParams(m.max_salary)
            );
        }
    };

    // ─── Client ───────────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::Client> {
        static constexpr std::string_view query =
            "INSERT INTO client (name, type, address, phone, email, registration_date) "
            "VALUES ($1, $2, $3, $4, $5, $6)";

        static auto extract(const Models::Client& m) {
            return std::make_tuple(
                Converters::toParams(m.name),
                Converters::toParams(m.type),
                m.address,
                m.phone,
                Converters::toParams(m.email),
                Converters::toParams(m.registration_date)
            );
        }
    };

    // ─── SoftwareLicense ──────────────────────────────────────────
    template <>
    struct ModelMeta<Models::SoftwareLicense> {
        static constexpr std::string_view query =
            "INSERT INTO softwarelicense (name, vendor, license_type, purchase_date, expiry_date, cost, seats) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7)";

        static auto extract(const Models::SoftwareLicense& m) {
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
    };

    // ─── Employee ─────────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::Employee> {
        static constexpr std::string_view query =
            "INSERT INTO employee (last_name, first_name, patronymic, birth_date, gender, "
            "phone, email, hire_date, salary, has_children, children_count, "
            "department_id, position_id, is_active) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14)";

        static auto extract(const Models::Employee& m) {
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
    };

    // ─── DeveloperSpecialization ──────────────────────────────────
    template <>
    struct ModelMeta<Models::DeveloperSpecialization> {
        static constexpr std::string_view query =
            "INSERT INTO developerspecialization (employee_id, programming_languages, "
            "frameworks, experience_years, backend_exp, frontend_exp, mobile_exp) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7)";

        static auto extract(const Models::DeveloperSpecialization& m) {
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
    };

    // ─── TesterSpecialization ─────────────────────────────────────
    template <>
    struct ModelMeta<Models::TesterSpecialization> {
        static constexpr std::string_view query =
            "INSERT INTO testerspecialization (employee_id, testing_types, "
            "automation_tools, certifications) "
            "VALUES ($1, $2, $3, $4)";

        static auto extract(const Models::TesterSpecialization& m) {
            return std::make_tuple(
                Converters::toParams(m.employee_id),
                Converters::toParams(m.testing_types),
                Converters::toParams(m.automation_tools),
                Converters::toParams(m.certifications)
            );
        }
    };

    // ─── ManagerCertification ─────────────────────────────────────
    template <>
    struct ModelMeta<Models::ManagerCertification> {
        static constexpr std::string_view query =
            "INSERT INTO managercertification (employee_id, cert_type, issue_date, "
            "expiry_date, project_complexity) "
            "VALUES ($1, $2, $3, $4, $5)";

        static auto extract(const Models::ManagerCertification& m) {
            return std::make_tuple(
                Converters::toParams(m.employee_id),
                Converters::toParams(m.cert_type),
                Converters::toParams(m.issue_date),
                m.expiry_date,
                m.project_complexity
            );
        }
    };

    // ─── Project ──────────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::Project> {
        static constexpr std::string_view query =
            "INSERT INTO project (name, description, client_id, start_date, "
            "planned_end_date, actual_end_date, budget, status, methodology, "
            "type, tech_stack, is_active) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12)";

        static auto extract(const Models::Project& m) {
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
    };

    // ─── ProjectPhase ─────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::ProjectPhase> {
        static constexpr std::string_view query =
            "INSERT INTO projectphase (project_id, name, start_date, end_date, order_number) "
            "VALUES ($1, $2, $3, $4, $5)";

        static auto extract(const Models::ProjectPhase& m) {
            return std::make_tuple(
                Converters::toParams(m.project_id),
                Converters::toParams(m.name),
                Converters::toParams(m.start_date),
                m.end_date,
                Converters::toParams(m.order_number)
            );
        }
    };

    // ─── ProjectAssignment ────────────────────────────────────────
    template <>
    struct ModelMeta<Models::ProjectAssignment> {
        static constexpr std::string_view query =
            "INSERT INTO projectassignment (employee_id, project_id, phase_id, "
            "role, start_date, end_date, hours_allocated) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7)";

        static auto extract(const Models::ProjectAssignment& m) {
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
    };

    // ─── LicenseAllocation ────────────────────────────────────────
    template <>
    struct ModelMeta<Models::LicenseAllocation> {
        static constexpr std::string_view query =
            "INSERT INTO licenseallocation (license_id, project_id, allocated_seats, allocation_date) "
            "VALUES ($1, $2, $3, $4)";

        static auto extract(const Models::LicenseAllocation& m) {
            return std::make_tuple(
                Converters::toParams(m.license_id),
                m.project_id,
                Converters::toParams(m.allocated_seats),
                Converters::toParams(m.allocation_date)
            );
        }
    };

    // ─── CloudResource ────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::CloudResource> {
        static constexpr std::string_view query =
            "INSERT INTO cloudresource (project_id, provider, type, configuration, cost_per_hour) "
            "VALUES ($1, $2, $3, $4, $5)";

        static auto extract(const Models::CloudResource& m) {
            return std::make_tuple(
                Converters::toParams(m.project_id),
                Converters::toParams(m.provider),
                Converters::toParams(m.type),
                m.configuration,
                Converters::toParams(m.cost_per_hour)
            );
        }
    };

    // ─── WorkLog ──────────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::WorkLog> {
        static constexpr std::string_view query =
            "INSERT INTO worklog (employee_id, project_id, task_description, work_date, hours_spent) "
            "VALUES ($1, $2, $3, $4, $5)";

        static auto extract(const Models::WorkLog& m) {
            return std::make_tuple(
                Converters::toParams(m.employee_id),
                Converters::toParams(m.project_id),
                Converters::toParams(m.task_description),
                Converters::toParams(m.work_date),
                Converters::toParams(m.hours_spent)
            );
        }
    };

    // ─── Bug ──────────────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::Bug> {
        static constexpr std::string_view query =
            "INSERT INTO bug (project_id, title, description, severity, status, "
            "found_date, fixed_date, found_by, fixed_by) "
            "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9)";

        static auto extract(const Models::Bug& m) {
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
    };

    // ─── Release ──────────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::Release> {
        static constexpr std::string_view query =
            "INSERT INTO release (project_id, version, release_date, changelog, fixed_bugs_count) "
            "VALUES ($1, $2, $3, $4, $5)";

        static auto extract(const Models::Release& m) {
            return std::make_tuple(
                Converters::toParams(m.project_id),
                Converters::toParams(m.version),
                Converters::toParams(m.release_date),
                m.changelog,
                Converters::toParams(m.fixed_bugs_count)
            );
        }
    };

    // ─── Documentation ────────────────────────────────────────────
    template <>
    struct ModelMeta<Models::Documentation> {
        static constexpr std::string_view query =
            "INSERT INTO documentation (project_id, type, author_id, creation_date, "
            "last_update, storage_path) "
            "VALUES ($1, $2, $3, $4, $5, $6)";

        static auto extract(const Models::Documentation& m) {
            return std::make_tuple(
                Converters::toParams(m.project_id),
                Converters::toParams(m.type),
                Converters::toParams(m.author_id),
                Converters::toParams(m.creation_date),
                m.last_update,
                m.storage_path
            );
        }
    };

}
