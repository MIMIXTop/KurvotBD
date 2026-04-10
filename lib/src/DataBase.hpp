#pragma once
#include <memory>
#include <pqxx/pqxx>

#include "Models.hpp"
#include "SqlFunctions.hpp"

#include "DataBaseSourse/SqlRequests/ModelMeta.hpp"
#include "DataBaseSourse/SqlRequests/AppendParam.hpp"


class DataBase {
public:
    DataBase(const std::string &connectinString);

    std::vector<std::string> getTablesNames();

    void appendModel(lib::Models::Model &&model);

    std::optional<lib::Models::Model> getModelById(lib::Models::Model modelType, const std::string &id);

    void deleteById(lib::Models::Model model, const std::string &id);

    std::vector<lib::Models::Model> selectAll(lib::Models::Model modelType);

    void updateById(lib::Models::Model model, const std::string &id);

    std::vector<lib::SqlFunctions::ProjectFilterResult> getProjectsByFilters();
    std::vector<lib::SqlFunctions::ProjectFilterResult> getProjectsByFilters(
        const std::vector<std::string>& statuses,
        const std::optional<std::string>& completedFrom,
        const std::optional<std::string>& completedTo,
        const std::optional<int>& clientId,
        const std::optional<std::string>& methodologyFilter,
        const std::optional<std::vector<std::string>>& clientTypes,
        const std::optional<double>& minBudget,
        const std::optional<double>& maxBudget,
        const std::optional<bool>& isActive
    );

    std::vector<lib::SqlFunctions::ProjectProfitabilityResult> getProjectProfitability();
    std::vector<lib::SqlFunctions::ProjectProfitabilityResult> getProjectProfitability(
        const std::optional<std::string>& from,
        const std::optional<std::string>& to,
        const std::optional<bool>& isActive
    );

    std::vector<lib::SqlFunctions::InfrastructureReportResult> getInfrastructureReport();
    std::vector<lib::SqlFunctions::InfrastructureReportResult> getInfrastructureReport(
        const std::optional<bool>& isActive,
        const std::optional<int>& projectId
    );

    std::vector<lib::SqlFunctions::ProjectTechnologyResult> getProjectTechnologies();
    std::vector<lib::SqlFunctions::ProjectTechnologyResult> getProjectTechnologies(
        const std::optional<bool>& isActive,
        const std::optional<std::string>& techName
    );

    std::vector<lib::SqlFunctions::EmployeeFilterResult> getEmployeeByFilters();
    std::vector<lib::SqlFunctions::EmployeeFilterResult> getEmployeeByFilters(
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
    );

    std::vector<lib::SqlFunctions::ProjectTeamResult> getProjectTeam();
    std::vector<lib::SqlFunctions::ProjectTeamResult> getProjectTeam(
        const std::optional<int>& projectId,
        const std::optional<int>& phaseId,
        const std::optional<std::vector<std::string>>& roles,
        const std::optional<std::vector<std::string>>& positionTitles,
        const std::optional<bool>& isActive
    );

    std::vector<lib::SqlFunctions::BugFilterResult> getBugsByFilter();
    std::vector<lib::SqlFunctions::BugFilterResult> getBugsByFilter(
        const std::optional<int>& projectId,
        const std::optional<std::vector<std::string>>& bugStatus,
        const std::optional<std::vector<std::string>>& bugSeverity,
        const std::optional<std::string>& foundFrom,
        const std::optional<std::string>& foundTo
    );

    std::vector<lib::SqlFunctions::TestingEfficiencyResult> getTestingEfficiency();
    std::vector<lib::SqlFunctions::TestingEfficiencyResult> getTestingEfficiency(
        const std::optional<int>& projectId,
        const std::optional<std::string>& from,
        const std::optional<std::string>& to
    );

    std::vector<lib::SqlFunctions::EmployeeWorkloadResult> getEmployeeWorkload();
    std::vector<lib::SqlFunctions::EmployeeWorkloadResult> getEmployeeWorkload(
        const std::optional<std::vector<int>>& employeeIds,
        const std::optional<std::vector<int>>& projectIds,
        const std::optional<std::string>& from,
        const std::optional<std::string>& to
    );

    std::vector<lib::SqlFunctions::ClientActivityResult> getClientActivity();
    std::vector<lib::SqlFunctions::ClientActivityResult> getClientActivity(
        const std::optional<std::string>& from,
        const std::optional<std::string>& to
    );

    std::vector<lib::SqlFunctions::ReleaseReportResult> getReleaseReport();
    std::vector<lib::SqlFunctions::ReleaseReportResult> getReleaseReport(
        const std::optional<std::string>& from,
        const std::optional<std::string>& to
    );

    std::vector<lib::SqlFunctions::MonthlyFinancialReportResult> getMonthlyFinancialReport(int month, int year);

    std::vector<lib::SqlFunctions::ProjectStatusReportResult> getProjectStatusReport(
        int projectId,
        const std::string& startDate,
        const std::string& endDate
    );

    pqxx::connection& getConnection() { return connection; }

    std::vector<std::string> getTableField(const std::string& ttableName);
    ~DataBase();

private:
    pqxx::connection connection;

    template <typename T>
    static void insertModel(pqxx::work& txn, const T& model) {
        auto values = lib::Meta::ModelMeta<T>::extract(model);
        std::apply([&](auto&&... args) {
            pqxx::params ps;
            (lib::Sql::appendParam(ps, args), ...);
            txn.exec(lib::Meta::ModelMeta<T>::Query::insert, ps);
        }, values);
    }

    template <typename T>
    static std::optional<lib::Models::Model> selectModelById(pqxx::work& txn, const T&, const std::string &id) {
        pqxx::params ps;
        ps.append(id);
        auto res = txn.exec(lib::Meta::ModelMeta<T>::Query::selectById, ps);

        if (res.empty()) {
            return std::nullopt;
        }

        return lib::Meta::ModelMeta<T>::fromRow(res[0]);
    }

    template <typename T>
    static void deleteModel(pqxx::work& txn, const T&, const std::string &id) {
        pqxx::params ps;
        ps.append(id);
        txn.exec(lib::Meta::ModelMeta<T>::Query::deleteById, ps);
    }

    template <typename T>
    static std::vector<T> selectAllModels(pqxx::work& txn, const T &) {
        auto res = txn.exec(lib::Meta::ModelMeta<T>::Query::selectAll);

        std::vector<T> models;
        models.reserve(res.size());

        for (const auto& row : res) {
            models.push_back(lib::Meta::ModelMeta<T>::fromRow(row));
        }

        return models;
    }

    template <typename T>
    static void updateModel(pqxx::work& txn, const T &model, const std::string &id) {
        auto values = lib::Meta::ModelMeta<T>::extract(model);
        std::apply([&](auto&&... args) {
            pqxx::params ps;
            (lib::Sql::appendParam(ps, args), ...);
            ps.append(id);
            txn.exec(lib::Meta::ModelMeta<T>::Query::updateById, ps);
        }, values);
    }
};
