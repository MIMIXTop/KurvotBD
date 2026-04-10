#include "DataBase.hpp"

#include <iostream>
#include <format>

#include "DataBaseSourse/SqlRequests/MetaData.hpp"
#include "DataBaseSourse/SqlRequests/ModelMeta.hpp"
#include "util/util.hpp"

DataBase::DataBase(const std::string &connectionString)
    : connection(connectionString) {
}

std::vector<std::string> DataBase::getTableField(const std::string &tableName) {
    std::vector<std::string> tableField;

    try {
        pqxx::work txn(connection);

        pqxx::params params;

        lib::Sql::appendParam(params, tableName);

        auto res = txn.exec(lib::Constants::getTablesField, params);

        for (auto&& row : res) {
            tableField.push_back(row[0].as<std::string>());
        }

        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getting table name: {}", e.what());
    }

    return tableField;
}

DataBase::~DataBase() = default;

std::vector<std::string> DataBase::getTablesNames() {
    std::vector<std::string> tables;

    try {
        pqxx::work work(connection);

        pqxx::result res = work.exec(lib::Constants::getListTables);

        for (auto&& row : res) {
            tables.push_back(row[0].as<std::string>());
        }

        work.commit();
    } catch (std::exception &e) {
        std::println(std::cerr, "Error getting table list: {}", e.what());
    }

    return tables;
}

void DataBase::appendModel(lib::Models::Model &&model) {
    try {
        pqxx::work txn(connection);

        std::visit([&](auto&& m) {
            insertModel(txn, m);
        }, std::move(model));

        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error appending model: {}", e.what());
    }
}

std::optional<lib::Models::Model> DataBase::getModelById(lib::Models::Model modelType, const std::string &id) {
    try {
        pqxx::work txn(connection);

        auto res = std::visit([&](auto&& m) {
            return selectModelById(txn, m, id);
        }, std::move(modelType));

        txn.commit();
        return res;
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getting model id: {}", e.what());
    }
    return std::nullopt;
}

void DataBase::deleteById(lib::Models::Model model, const std::string &id) {
    try {
        pqxx::work txn(connection);
        std::visit([&](auto&& m) { deleteModel(txn, m, id); }, std::move(model));
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error deleting model id: {}", e.what());
    }
}

std::vector<lib::Models::Model> DataBase::selectAll(lib::Models::Model modelType) {
    try {
        pqxx::work txn(connection);

        auto res = std::visit([&](auto&& m) -> std::vector<lib::Models::Model> {
            auto vec = selectAllModels(txn, m);
            std::vector<lib::Models::Model> result;
            result.reserve(vec.size());
            for (auto& item : vec) {
                result.push_back(std::move(item));
            }
            return result;
        }, std::move(modelType));

        txn.commit();
        return res;
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error selecting all models: {}", e.what());
    }
    return {};
}

void DataBase::updateById(lib::Models::Model model, const std::string &id) {
    try {
        pqxx::work txn(connection);
        std::visit([&](auto&& m) { updateModel(txn, m, id); }, std::move(model));
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error updating model id: {}", e.what());
    }
}

std::vector<lib::SqlFunctions::ProjectFilterResult> DataBase::getProjectsByFilters() {
    return getProjectsByFilters({}, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::ProjectFilterResult> DataBase::getProjectsByFilters(
    const std::vector<std::string>& statuses,
    const std::optional<std::string>& completedFrom,
    const std::optional<std::string>& completedTo,
    const std::optional<int>& clientId,
    const std::optional<std::string>& methodologyFilter,
    const std::optional<std::vector<std::string>>& clientTypes,
    const std::optional<double>& minBudget,
    const std::optional<double>& maxBudget,
    const std::optional<bool>& isActive
) {
    std::vector<lib::SqlFunctions::ProjectFilterResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_projects_by_filters(";
        
        bool first = true;
        
        if (!statuses.empty()) {
            sql += "p_statuses := ARRAY[";
            for (size_t i = 0; i < statuses.size(); ++i) {
                if (i > 0) sql += ", ";
                sql += txn.quote(statuses[i]);
            }
            sql += "]";
            first = false;
        }
        if (completedFrom) {
            if (!first) sql += ", ";
            sql += "p_completed_from := " + txn.quote(*completedFrom);
            first = false;
        }
        if (completedTo) {
            if (!first) sql += ", ";
            sql += "p_completed_to := " + txn.quote(*completedTo);
            first = false;
        }
        if (clientId) {
            if (!first) sql += ", ";
            sql += "p_client_id := " + std::to_string(*clientId);
            first = false;
        }
        if (methodologyFilter) {
            if (!first) sql += ", ";
            sql += "p_methodology_filter := " + txn.quote(*methodologyFilter);
            first = false;
        }
        if (clientTypes && !clientTypes->empty()) {
            if (!first) sql += ", ";
            sql += "p_client_types := ARRAY[";
            for (size_t i = 0; i < clientTypes->size(); ++i) {
                if (i > 0) sql += ", ";
                sql += txn.quote((*clientTypes)[i]);
            }
            sql += "]";
            first = false;
        }
        if (minBudget) {
            if (!first) sql += ", ";
            sql += "p_min_budget := " + std::to_string(*minBudget);
            first = false;
        }
        if (maxBudget) {
            if (!first) sql += ", ";
            sql += "p_max_budget := " + std::to_string(*maxBudget);
            first = false;
        }
        if (isActive) {
            if (!first) sql += ", ";
            sql += "p_is_active := " + std::string(*isActive ? "TRUE" : "FALSE");
        }
        
        sql += ")";
        
        std::println(std::cout, "{}", sql);
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::ProjectFilterResult r;
            r.project_id = row["project_id"].as<int>();
            r.client_id = row["client_id"].as<int>();
            r.project_name = row["project_name"].as<std::string>();
            r.client_name = row["client_name"].as<std::string>();
            r.client_type = row["client_type"].as<std::string>();
            r.status = row["status"].as<std::string>();
            r.type = row["type"].as<std::string>();
            r.methodology = row["methodology"].as<std::string>();
            r.start_date = row["start_date"].as<std::string>();
            r.planned_end_date = row["planned_end_date"].as<std::string>();
            r.actual_end_date = row["actual_end_date"].is_null() ? "" : row["actual_end_date"].as<std::string>();
            r.budget = row["budget"].as<double>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getProjectsByFilters: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::ProjectProfitabilityResult> DataBase::getProjectProfitability() {
    return getProjectProfitability(std::nullopt, std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::ProjectProfitabilityResult> DataBase::getProjectProfitability(
    const std::optional<std::string>& from,
    const std::optional<std::string>& to,
    const std::optional<bool>& isActive
) {
    std::vector<lib::SqlFunctions::ProjectProfitabilityResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_project_profitability(";
        
        bool first = true;
        if (from) {
            sql += txn.quote(*from);
            first = false;
        }
        if (to) {
            if (!first) sql += ", ";
            sql += txn.quote(*to);
            first = false;
        }
        if (isActive) {
            if (!first) sql += ", ";
            sql += *isActive ? "TRUE" : "FALSE";
        }
        
        sql += ")";
        
        std::println(std::cout, "{}", sql);
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::ProjectProfitabilityResult r;
            r.project_id = row["project_id"].as<int>();
            r.project_name = row["project_name"].as<std::string>();
            r.total_costs = row["total_costs"].as<double>();
            r.budget = row["budget"].as<double>();
            r.efficiency_ratio = row["efficiency_ratio"].as<double>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getProjectProfitability: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::InfrastructureReportResult> DataBase::getInfrastructureReport() {
    return getInfrastructureReport(std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::InfrastructureReportResult> DataBase::getInfrastructureReport(
    const std::optional<bool>& isActive,
    const std::optional<int>& projectId
) {
    std::vector<lib::SqlFunctions::InfrastructureReportResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_infrastructure_report(";
        
        bool first = true;
        if (isActive) {
            sql += "p_is_active := " + std::string(*isActive ? "TRUE" : "FALSE");
            first = false;
        }
        if (projectId) {
            if (!first) sql += ", ";
            sql += "p_project_id := " + std::to_string(*projectId);
        }
        
        sql += ")";
        
        std::println(std::cout, "{}", sql);
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::InfrastructureReportResult r;
            r.project_id = row["project_id"].as<int>();
            r.project_name = row["project_name"].as<std::string>();
            r.license_name = row["license_name"].is_null() ? "" : row["license_name"].as<std::string>();
            r.expiry_date = row["expiry_date"].is_null() ? "" : row["expiry_date"].as<std::string>();
            r.cost = row["cost"].is_null() ? 0.0 : row["cost"].as<double>();
            r.resource_provider = row["resource_provider"].is_null() ? "" : row["resource_provider"].as<std::string>();
            r.resource_cost_per_hour = row["resource_cost_per_hour"].is_null() ? 0.0 : row["resource_cost_per_hour"].as<double>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getInfrastructureReport: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::ProjectTechnologyResult> DataBase::getProjectTechnologies() {
    return getProjectTechnologies(std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::ProjectTechnologyResult> DataBase::getProjectTechnologies(
    const std::optional<bool>& isActive,
    const std::optional<std::string>& techName
) {
    std::vector<lib::SqlFunctions::ProjectTechnologyResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_project_technologies(";
        
        bool first = true;
        if (isActive) {
            sql += "p_is_active := " + std::string(*isActive ? "TRUE" : "FALSE");
            first = false;
        }
        if (techName) {
            if (!first) sql += ", ";
            sql += "p_tech_name := " + txn.quote(*techName);
        }
        
        sql += ")";
        
        std::println(std::cout, "{}", sql);
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::ProjectTechnologyResult r;
            r.tech_name = row["tech_name"].as<std::string>();
            r.project_count = row["project_count"].as<int64_t>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getProjectTechnologies: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::EmployeeFilterResult> DataBase::getEmployeeByFilters() {
    return getEmployeeByFilters(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::EmployeeFilterResult> DataBase::getEmployeeByFilters(
    const std::optional<std::vector<int>>& departmentIds,
    const std::optional<std::vector<int>>& positionTitles,
    const std::optional<int>& minExperience,
    const std::optional<int>& maxExperience,
    const std::optional<int>& minAge,
    const std::optional<int>& maxAge,
    const std::optional<double>& minSalary,
    const std::optional<double>& maxSalary,
    const std::optional<std::string>& certType,
    const std::optional<bool>& isActive
) {
    std::vector<lib::SqlFunctions::EmployeeFilterResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_employee_by_filters(";
        
        bool first = true;
        
        if (departmentIds && !departmentIds->empty()) {
            if (!first) sql += ", ";
            std::string arr = "ARRAY[";
            for (size_t i = 0; i < departmentIds->size(); ++i) {
                if (i > 0) arr += ", ";
                arr += std::to_string((*departmentIds)[i]);
            }
            arr += "]";
            sql += "p_department_ids := " + arr;
            first = false;
        }
        if (positionTitles && !positionTitles->empty()) {
            if (!first) sql += ", ";
            std::string arr = "ARRAY[";
            for (size_t i = 0; i < positionTitles->size(); ++i) {
                if (i > 0) arr += ", ";
                arr += std::to_string((*positionTitles)[i]);
            }
            arr += "]";
            sql += "p_position_ids := " + arr;
            first = false;
        }
        if (minExperience) {
            if (!first) sql += ", ";
            sql += "p_min_experience := " + std::to_string(*minExperience);
            first = false;
        }
        if (maxExperience) {
            if (!first) sql += ", ";
            sql += "p_max_experience := " + std::to_string(*maxExperience);
            first = false;
        }
        if (minAge) {
            if (!first) sql += ", ";
            sql += "p_min_age := " + std::to_string(*minAge);
            first = false;
        }
        if (maxAge) {
            if (!first) sql += ", ";
            sql += "p_max_age := " + std::to_string(*maxAge);
            first = false;
        }
        if (minSalary) {
            if (!first) sql += ", ";
            sql += "p_min_salary := " + std::to_string(*minSalary);
            first = false;
        }
        if (maxSalary) {
            if (!first) sql += ", ";
            sql += "p_max_salary := " + std::to_string(*maxSalary);
            first = false;
        }
        if (certType) {
            if (!first) sql += ", ";
            sql += "p_cert_type := " + txn.quote(*certType);
            first = false;
        }
        if (first) {
            sql += "p_is_active := ";
        } else {
            sql += ", p_is_active := ";
        }
        sql += std::string((isActive && *isActive) ? "TRUE" : "FALSE");
        
        sql += ")";
        
        std::println(std::cout, "{}", sql);
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::EmployeeFilterResult r;
            r.employee_id = row["employee_id"].as<int>();
            r.department_id = row["department_id"].as<int>();
            r.position_id = row["position_id"].as<int>();
            r.full_name = row["full_name"].is_null() ? "" : row["full_name"].as<std::string>();
            r.department_name = row["department_name"].is_null() ? "" : row["department_name"].as<std::string>();
            r.position_title = row["position_title"].is_null() ? "" : row["position_title"].as<std::string>();
            r.age = row["age"].as<int>();
            r.experience_years = row["experience_years"].as<int>();
            r.salary = row["salary"].as<double>();
            r.email = row["email"].is_null() ? "" : row["email"].as<std::string>();
            r.phone = row["phone"].is_null() ? "" : row["phone"].as<std::string>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getEmployeeByFilters: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::ProjectTeamResult> DataBase::getProjectTeam() {
    return getProjectTeam(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::ProjectTeamResult> DataBase::getProjectTeam(
    const std::optional<int>& projectId,
    const std::optional<int>& phaseId,
    const std::optional<std::vector<std::string>>& roles,
    const std::optional<std::vector<std::string>>& positionTitles,
    const std::optional<bool>& isActive
) {
    std::vector<lib::SqlFunctions::ProjectTeamResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_project_team(";
        
        bool first = true;
        
        if (projectId) {
            sql += "p_project_id := " + std::to_string(*projectId);
            first = false;
        }
        if (phaseId) {
            if (!first) sql += ", ";
            sql += "p_phase_id := " + std::to_string(*phaseId);
            first = false;
        }
        if (roles && !roles->empty()) {
            if (!first) sql += ", ";
            sql += "p_roles := ARRAY[";
            for (size_t i = 0; i < roles->size(); ++i) {
                if (i > 0) sql += ", ";
                sql += txn.quote((*roles)[i]);
            }
            sql += "]::varchar[]";
            first = false;
        }
        if (positionTitles && !positionTitles->empty()) {
            if (!first) sql += ", ";
            sql += "p_position_titles := ARRAY[";
            for (size_t i = 0; i < positionTitles->size(); ++i) {
                if (i > 0) sql += ", ";
                sql += txn.quote((*positionTitles)[i]);
            }
            sql += "]::varchar[]";
            first = false;
        }
        if (isActive) {
            if (!first) sql += ", ";
            sql += "p_is_active := " + std::string(*isActive ? "TRUE" : "FALSE");
        }
        
        sql += ")";
        
        std::println(std::cout, "{}", sql);
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::ProjectTeamResult r;
            r.assignment_id = row["assignment_id"].as<int>();
            r.employee_id = row["employee_id"].as<int>();
            r.project_id = row["project_id"].as<int>();
            r.phase_id = row["phase_id"].is_null() ? 0 : row["phase_id"].as<int>();
            r.full_name = row["full_name"].as<std::string>();
            r.position_title = row["position_title"].as<std::string>();
            r.project_role = row["project_role"].as<std::string>();
            r.project_name = row["project_name"].as<std::string>();
            r.phase_name = row["phase_name"].is_null() ? "" : row["phase_name"].as<std::string>();
            r.start_date = row["start_date"].as<std::string>();
            r.end_date = row["end_date"].is_null() ? "" : row["end_date"].as<std::string>();
            r.hours_allocated = row["hours_allocated"].as<int>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getProjectTeam: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::BugFilterResult> DataBase::getBugsByFilter() {
    return getBugsByFilter(std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::BugFilterResult> DataBase::getBugsByFilter(
    const std::optional<int>& projectId,
    const std::optional<std::vector<std::string>>& bugStatus,
    const std::optional<std::vector<std::string>>& bugSeverity,
    const std::optional<std::string>& foundFrom,
    const std::optional<std::string>& foundTo
) {
    std::vector<lib::SqlFunctions::BugFilterResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_bugs_by_filter(";
        
        bool first = true;
        
        if (projectId) {
            sql += "p_project_id := " + std::to_string(*projectId);
            first = false;
        }
        if (bugStatus && !bugStatus->empty()) {
            if (!first) sql += ", ";
            sql += "p_bug_status := ARRAY[";
            for (size_t i = 0; i < bugStatus->size(); ++i) {
                if (i > 0) sql += ", ";
                sql += txn.quote((*bugStatus)[i]);
            }
            sql += "]";
            first = false;
        }
        if (bugSeverity && !bugSeverity->empty()) {
            if (!first) sql += ", ";
            sql += "p_bug_severity := ARRAY[";
            for (size_t i = 0; i < bugSeverity->size(); ++i) {
                if (i > 0) sql += ", ";
                sql += txn.quote((*bugSeverity)[i]);
            }
            sql += "]";
            first = false;
        }
        if (foundFrom) {
            if (!first) sql += ", ";
            sql += "p_found_from := " + txn.quote(*foundFrom);
            first = false;
        }
        if (foundTo) {
            if (!first) sql += ", ";
            sql += "p_found_to := " + txn.quote(*foundTo);
        }
        
        sql += ")";
        
        std::println(std::cout, "{}", sql);
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::BugFilterResult r;
            r.bug_id = row["bug_id"].as<int>();
            r.project_name = row["project_name"].as<std::string>();
            r.title = row["title"].as<std::string>();
            r.description = row["description"].as<std::string>();
            r.status = row["status"].as<std::string>();
            r.severity = row["severity"].as<std::string>();
            r.found_by_name = row["found_by_name"].as<std::string>();
            r.fixed_by_name = row["fixed_by_name"].as<std::string>();
            r.created_at = row["created_at"].as<std::string>();
            r.found_date = row["found_date"].as<std::string>();
            r.fixed_date = row["fixed_date"].as<std::string>();
            r.total_count = row["total_count"].as<int64_t>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getBugsByFilter: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::TestingEfficiencyResult> DataBase::getTestingEfficiency() {
    return getTestingEfficiency(std::nullopt, std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::TestingEfficiencyResult> DataBase::getTestingEfficiency(
    const std::optional<int>& projectId,
    const std::optional<std::string>& from,
    const std::optional<std::string>& to
) {
    std::vector<lib::SqlFunctions::TestingEfficiencyResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_testing_efficiency(";
        
        bool first = true;
        
        if (projectId) {
            sql += "p_project_id := " + std::to_string(*projectId);
            first = false;
        }
        if (from) {
            if (!first) sql += ", ";
            sql += "p_from := " + txn.quote(*from);
            first = false;
        }
        if (to) {
            if (!first) sql += ", ";
            sql += "p_to := " + txn.quote(*to);
        }
        
        sql += ")";
        
        std::println(std::cout, "{}", sql);
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::TestingEfficiencyResult r;
            r.project_name = row["project_name"].as<std::string>();
            r.bugs_in_test = row["bugs_in_test"].as<int64_t>();
            r.bugs_after_release = row["bugs_after_release"].as<int64_t>();
            r.efficiency_ratio = row["efficiency_ratio"].as<double>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getTestingEfficiency: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::EmployeeWorkloadResult> DataBase::getEmployeeWorkload() {
    return getEmployeeWorkload(std::nullopt, std::nullopt, std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::EmployeeWorkloadResult> DataBase::getEmployeeWorkload(
    const std::optional<std::vector<int>>& employeeIds,
    const std::optional<std::vector<int>>& projectIds,
    const std::optional<std::string>& from,
    const std::optional<std::string>& to
) {
    std::vector<lib::SqlFunctions::EmployeeWorkloadResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_employee_workload(";
        
        bool first = true;
        
        if (employeeIds && !employeeIds->empty()) {
            if (!first) sql += ", ";
            sql += "p_employee_ids := ARRAY[";
            for (size_t i = 0; i < employeeIds->size(); ++i) {
                if (i > 0) sql += ", ";
                sql += std::to_string((*employeeIds)[i] + 1);
            }
            sql += "]";
            first = false;
        }
        if (projectIds && !projectIds->empty()) {
            if (!first) sql += ", ";
            sql += "p_project_ids := ARRAY[";
            for (size_t i = 0; i < projectIds->size(); ++i) {
                if (i > 0) sql += ", ";
                sql += std::to_string((*projectIds)[i] + 1);
            }
            sql += "]";
            first = false;
        }
        if (from) {
            if (!first) sql += ", ";
            sql += "p_from := " + txn.quote(*from);
            first = false;
        }
        if (to) {
            if (!first) sql += ", ";
            sql += "p_to := " + txn.quote(*to);
        }
        
        sql += ")";

        std::println(std::cout, "{}",sql);
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::EmployeeWorkloadResult r;
            r.employee_id = row["employee_id"].as<int>();
            r.full_name = row["full_name"].as<std::string>();
            r.project_id = row["project_id"].as<int>();
            r.project_name = row["project_name"].as<std::string>();
            r.total_hours = row["total_hours"].as<double>();
            r.task_count = row["task_count"].as<int64_t>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getEmployeeWorkload: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::ClientActivityResult> DataBase::getClientActivity() {
    return getClientActivity(std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::ClientActivityResult> DataBase::getClientActivity(
    const std::optional<std::string>& from,
    const std::optional<std::string>& to
) {
    std::vector<lib::SqlFunctions::ClientActivityResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_client_activity(";
        
        bool first = true;
        if (from) {
            sql += "p_from := " + txn.quote(*from);
            first = false;
        }
        if (to) {
            if (!first) sql += ", ";
            sql += "p_to := " + txn.quote(*to);
        }
        
        sql += ")";
        
        std::println(std::cout, "{}", sql);
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::ClientActivityResult r;
            r.client_id = row["client_id"].as<int>();
            r.client_name = row["client_name"].as<std::string>();
            r.active_projects = row["active_projects"].as<int64_t>();
            r.total_spend = row["total_spend"].is_null() ? 0.0 : row["total_spend"].as<double>();
            r.earliest_project_date = row["earliest_project_date"].is_null() ? "" : row["earliest_project_date"].as<std::string>();
            r.latest_project_date = row["latest_project_date"].is_null() ? "" : row["latest_project_date"].as<std::string>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getClientActivity: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::ReleaseReportResult> DataBase::getReleaseReport() {
    return getReleaseReport(std::nullopt, std::nullopt);
}

std::vector<lib::SqlFunctions::ReleaseReportResult> DataBase::getReleaseReport(
    const std::optional<std::string>& from,
    const std::optional<std::string>& to
) {
    std::vector<lib::SqlFunctions::ReleaseReportResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_release_report(";
        
        bool first = true;
        if (from) {
            sql += "p_from := " + txn.quote(*from);
            first = false;
        }
        if (to) {
            if (!first) sql += ", ";
            sql += "p_to := " + txn.quote(*to);
        }
        
        sql += ")";
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::ReleaseReportResult r;
            r.project_name = row["project_name"].as<std::string>();
            r.version = row["version"].as<std::string>();
            r.release_date = row["release_date"].as<std::string>();
            r.changelog = row["changelog"].as<std::string>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getReleaseReport: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::MonthlyFinancialReportResult> DataBase::getMonthlyFinancialReport(int month, int year) {
    std::vector<lib::SqlFunctions::MonthlyFinancialReportResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_monthly_financial_report(p_month := " + std::to_string(month) + ", p_year := " + std::to_string(year) + ")";
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::MonthlyFinancialReportResult r;
            r.project_name = row["project_name"].as<std::string>();
            r.expenses_salary = row["expenses_salary"].as<double>();
            r.expenses_licenses = row["expenses_licenses"].as<double>();
            r.expenses_cloud = row["expenses_cloud"].as<double>();
            r.total_monthly_cost = row["total_monthly_cost"].as<double>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getMonthlyFinancialReport: {}", e.what());
    }
    return results;
}

std::vector<lib::SqlFunctions::ProjectStatusReportResult> DataBase::getProjectStatusReport(
    int projectId,
    const std::string& startDate,
    const std::string& endDate
) {
    std::vector<lib::SqlFunctions::ProjectStatusReportResult> results;
    try {
        pqxx::work txn(connection);
        std::string sql = "SELECT * FROM get_project_status_report(p_project_id := " + std::to_string(projectId) + 
                          ", p_start_date := " + txn.quote(startDate) + ", p_end_date := " + txn.quote(endDate) + ")";
        
        pqxx::result res = txn.exec(sql);
        for (auto&& row : res) {
            lib::SqlFunctions::ProjectStatusReportResult r;
            r.project_name = row["project_name"].as<std::string>();
            r.current_phase = row["current_phase"].as<std::string>();
            r.tasks_logged = row["tasks_logged"].as<int64_t>();
            r.team_hours_spent = row["team_hours_spent"].as<double>();
            r.critical_risks_count = row["critical_risks_count"].as<int64_t>();
            r.report_period = row["report_period"].as<std::string>();
            results.push_back(r);
        }
        txn.commit();
    } catch (const std::exception& e) {
        std::println(std::cerr, "Error getProjectStatusReport: {}", e.what());
    }
    return results;
}