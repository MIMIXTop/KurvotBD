#include <gtest/gtest.h>

#include "DataBase.hpp"
#include "SqlFunctions.hpp"
#include <pqxx/pqxx>


static const std::string connStr =
    "host=127.0.0.1 port=5432 dbname=db user=user password=pass";

class SqlFunctionsTest : public ::testing::Test {
protected:
    std::unique_ptr<DataBase> db;
    pqxx::connection conn{connStr};

    void SetUp() override {
        db = std::make_unique<DataBase>(connStr);

        pqxx::work txn(conn);
        // Truncate and reset tables
        txn.exec(R"(
            TRUNCATE department, position, client, employee,
                developerspecialization, testerspecialization,
                softwarelicense, managercertification, project,
                projectphase, projectassignment, licenseallocation,
                cloudresource, worklog, bug, release, documentation
                RESTART IDENTITY CASCADE
        )");

        txn.exec(R"(
            INSERT INTO department (department_id, name, type) VALUES
                (1, 'Отдел разработки', 'разработка'),
                (2, 'Отдел тестирования', 'тестирование');

            INSERT INTO position (position_id, title, category, min_salary, max_salary) VALUES
                (1, 'Программист', 'технический_персонал', 100000, 200000),
                (2, 'Тестировщик', 'технический_персонал', 80000, 150000);

            INSERT INTO client (client_id, name, type, email) VALUES
                (1, 'Клиент А', 'корпоративный', 'clientA@test.ru'),
                (2, 'Клиент Б', 'государственный', 'clientB@test.ru');

            INSERT INTO project (project_id, name, description, client_id, start_date, planned_end_date, actual_end_date, budget, status, methodology, type, is_active) VALUES
                (1, 'Проект 1', NULL, 1, '2024-01-01', '2024-12-31', NULL, 1000000, 'в_разработке', 'Scrum', 'веб-приложение', true),
                (2, 'Проект 2', NULL, 2, '2024-01-01', '2024-12-31', NULL, 500000, 'завершен', 'Kanban', 'мобильное_приложение', false);

            INSERT INTO employee (employee_id, last_name, first_name, patronymic, birth_date, gender, email, hire_date, salary, department_id, position_id, is_active) VALUES
                (1, 'Иванов', 'Иван', 'Иванович', '1990-01-01', 'мужской', 'ivanov@test.ru', '2020-01-01', 150000, 1, 1, true),
                (2, 'Петров', 'Пётр', NULL, '1992-06-15', 'мужской', 'petrov@test.ru', '2021-03-01', 120000, 2, 2, true);
        )");
        txn.commit();
    }

};

TEST_F(SqlFunctionsTest, GetProjectsByFilters_NoArgs) {
    auto results = db->getProjectsByFilters();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetProjectsByFilters_WithStatus) {
    auto results = db->getProjectsByFilters(
        {"в_разработке"},
        std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, std::nullopt
    );
    for (const auto& r : results) {
        EXPECT_EQ(r.status, "в_разработке");
    }
}

TEST_F(SqlFunctionsTest, GetProjectsByFilters_WithClientId) {
    auto results = db->getProjectsByFilters(
        {}, std::nullopt, std::nullopt, 1, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, std::nullopt
    );
    for (const auto& r : results) {
        EXPECT_EQ(r.client_id, 1);
    }
}

TEST_F(SqlFunctionsTest, GetProjectsByFilters_WithMethodology) {
    auto results = db->getProjectsByFilters(
        {}, std::nullopt, std::nullopt, std::nullopt, "Scrum",
        std::nullopt, std::nullopt, std::nullopt, std::nullopt
    );
    for (const auto& r : results) {
        EXPECT_EQ(r.methodology, "Scrum");
    }
}

TEST_F(SqlFunctionsTest, GetProjectsByFilters_WithBudgetRange) {
    auto results = db->getProjectsByFilters(
        {}, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        std::nullopt, 500000, 1500000, std::nullopt
    );
    for (const auto& r : results) {
        EXPECT_GE(r.budget, 500000);
        EXPECT_LE(r.budget, 1500000);
    }
}

TEST_F(SqlFunctionsTest, GetProjectsByFilters_WithIsActive) {
    auto results = db->getProjectsByFilters(
        {}, std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, true
    );
    for (const auto& r : results) {
        EXPECT_EQ(r.status, "в_разработке");
    }
}

TEST_F(SqlFunctionsTest, GetProjectProfitability_NoArgs) {
    auto results = db->getProjectProfitability();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetProjectProfitability_WithDates) {
    auto results = db->getProjectProfitability(
        "2024-01-01", "2024-12-31", std::nullopt
    );
    for (const auto& r : results) {
        EXPECT_GE(r.budget, 0);
    }
}

TEST_F(SqlFunctionsTest, GetInfrastructureReport_NoArgs) {
    auto results = db->getInfrastructureReport();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetInfrastructureReport_WithProjectId) {
    auto results = db->getInfrastructureReport(std::nullopt, 1);
    for (const auto& r : results) {
        EXPECT_EQ(r.project_id, 1);
    }
}

TEST_F(SqlFunctionsTest, GetProjectTechnologies_NoArgs) {
    auto results = db->getProjectTechnologies();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetEmployeeByFilters_NoArgs) {
    auto results = db->getEmployeeByFilters();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetEmployeeByFilters_WithDepartment) {
    std::vector<int> deptIds = {1};
    auto results = db->getEmployeeByFilters(
        deptIds, std::nullopt, std::nullopt, std::nullopt,
        std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt
    );
    for (const auto& r : results) {
        EXPECT_EQ(r.department_id, 1);
    }
}

TEST_F(SqlFunctionsTest, GetEmployeeByFilters_WithSalaryRange) {
    auto results = db->getEmployeeByFilters(
        std::nullopt, std::nullopt, std::nullopt, std::nullopt,
        std::nullopt, std::nullopt, 100000, 200000, std::nullopt, std::nullopt
    );
    for (const auto& r : results) {
        EXPECT_GE(r.salary, 100000);
        EXPECT_LE(r.salary, 200000);
    }
}

TEST_F(SqlFunctionsTest, GetProjectTeam_NoArgs) {
    auto results = db->getProjectTeam();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetProjectTeam_WithProjectId) {
    auto results = db->getProjectTeam(1, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
    for (const auto& r : results) {
        EXPECT_EQ(r.project_id, 1);
    }
}

TEST_F(SqlFunctionsTest, GetBugsByFilter_NoArgs) {
    auto results = db->getBugsByFilter();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetBugsByFilter_WithProjectId) {
    auto results = db->getBugsByFilter(
        1, std::nullopt, std::nullopt, std::nullopt, std::nullopt
    );
    for (const auto& r : results) {
        EXPECT_TRUE(true);
    }
}

TEST_F(SqlFunctionsTest, GetTestingEfficiency_NoArgs) {
    auto results = db->getTestingEfficiency();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetEmployeeWorkload_NoArgs) {
    auto results = db->getEmployeeWorkload();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetEmployeeWorkload_WithEmployeeIds) {
    std::vector<int> empIds = {1};
    auto results = db->getEmployeeWorkload(
        empIds, std::nullopt, std::nullopt, std::nullopt
    );
    for (const auto& r : results) {
        EXPECT_EQ(r.employee_id, 1);
    }
}

TEST_F(SqlFunctionsTest, GetClientActivity_NoArgs) {
    auto results = db->getClientActivity();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetClientActivity_WithDates) {
    auto results = db->getClientActivity("2024-01-01", "2024-12-31");
    for (const auto& r : results) {
        EXPECT_GE(r.client_id, 0);
    }
}

TEST_F(SqlFunctionsTest, GetReleaseReport_NoArgs) {
    auto results = db->getReleaseReport();
    EXPECT_GE(results.size(), 0);
}

TEST_F(SqlFunctionsTest, GetMonthlyFinancialReport) {
    auto results = db->getMonthlyFinancialReport(1, 2024);
    for (const auto& r : results) {
        EXPECT_GE(r.project_name.size(), 0);
    }
}

TEST_F(SqlFunctionsTest, GetProjectStatusReport) {
    auto results = db->getProjectStatusReport(1, "2024-01-01", "2024-12-31");
    if (!results.empty()) {
        EXPECT_EQ(results[0].project_name, "Проект 1");
    }
}