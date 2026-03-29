#include <gtest/gtest.h>

#include "DataBase.hpp"
#include "Models.hpp"

using namespace lib::Models;

static const std::string connStr =
    "host=127.0.0.1 port=5432 dbname=db user=user password=pass";

// ═══════════════════════════════════════════════════════════════════
// Фикстура: подключение к БД + очистка таблиц после каждого теста
// ═══════════════════════════════════════════════════════════════════

class AppendModelTest : public ::testing::Test {
protected:
    std::unique_ptr<DataBase> db;
    pqxx::connection conn{connStr};

    void SetUp() override {
        db = std::make_unique<DataBase>(connStr);

        pqxx::work txn(conn);
        txn.exec(
            "TRUNCATE department, position, client, employee, "
            "developerspecialization, testerspecialization, "
            "softwarelicense, managercertification, project, "
            "projectphase, projectassignment, licenseallocation, "
            "cloudresource, worklog, bug, release, documentation "
            "RESTART IDENTITY CASCADE"
        );
        txn.commit();
    }

    int countRows(const std::string& table) {
        pqxx::work txn(conn);
        auto r = txn.exec("SELECT COUNT(*) FROM " + table);
        return r[0][0].as<int>();
    }
};

// ═══════════════════════════════════════════════════════════════════
// Department — enum конвертация
// ═══════════════════════════════════════════════════════════════════

TEST_F(AppendModelTest, InsertDepartment) {
    db->appendModel(Department{
        .id = 0,
        .name = "Тестовый отдел",
        .type = DepartmentType::Testing
    });

    EXPECT_EQ(countRows("department"), 1);

    pqxx::work txn(conn);
    auto r = txn.exec("SELECT name, type FROM department");
    EXPECT_EQ(r[0]["name"].as<std::string>(), "Тестовый отдел");
    EXPECT_EQ(r[0]["type"].as<std::string>(), "тестирование");
}

// ═══════════════════════════════════════════════════════════════════
// Position — enum + doubles
// ═══════════════════════════════════════════════════════════════════

TEST_F(AppendModelTest, InsertPosition) {
    db->appendModel(Position{
        .id = 0,
        .title = "Junior Dev",
        .category = PositionCategory::Technical_staff,
        .min_salary = 80000.0,
        .max_salary = 150000.0
    });

    EXPECT_EQ(countRows("position"), 1);

    pqxx::work txn(conn);
    auto r = txn.exec("SELECT title, category, min_salary, max_salary FROM position");
    EXPECT_EQ(r[0]["title"].as<std::string>(), "Junior Dev");
    EXPECT_EQ(r[0]["category"].as<std::string>(), "технический_персонал");
    EXPECT_DOUBLE_EQ(r[0]["min_salary"].as<double>(), 80000.0);
    EXPECT_DOUBLE_EQ(r[0]["max_salary"].as<double>(), 150000.0);
}

// ═══════════════════════════════════════════════════════════════════
// Client — optional поля (address = NULL)
// ═══════════════════════════════════════════════════════════════════

TEST_F(AppendModelTest, InsertClientWithNullOptional) {
    db->appendModel(Client{
        .id = 0,
        .name = "ООО Тест",
        .type = ClientType::Corporate,
        .address = std::nullopt,       // → NULL
        .phone = std::nullopt,         // → NULL
        .email = "test@test.ru",
        .registration_date = "2024-06-15"
    });

    EXPECT_EQ(countRows("client"), 1);

    pqxx::work txn(conn);
    auto r = txn.exec("SELECT name, type, address, phone, email FROM client");
    EXPECT_EQ(r[0]["name"].as<std::string>(), "ООО Тест");
    EXPECT_EQ(r[0]["type"].as<std::string>(), "корпоративный");
    EXPECT_TRUE(r[0]["address"].is_null());
    EXPECT_TRUE(r[0]["phone"].is_null());
    EXPECT_EQ(r[0]["email"].as<std::string>(), "test@test.ru");
}

// ═══════════════════════════════════════════════════════════════════
// Client — optional поля со значениями
// ═══════════════════════════════════════════════════════════════════

TEST_F(AppendModelTest, InsertClientWithValueOptional) {
    db->appendModel(Client{
        .id = 0,
        .name = "ИП Иванов",
        .type = ClientType::Individual_entrepreneur,
        .address = std::string("ул. Пушкина, 1"),
        .phone = std::string("+79990001122"),
        .email = "ivanov@biz.ru",
        .registration_date = "2025-01-01"
    });

    pqxx::work txn(conn);
    auto r = txn.exec("SELECT address, phone FROM client");
    EXPECT_EQ(r[0]["address"].as<std::string>(), "ул. Пушкина, 1");
    EXPECT_EQ(r[0]["phone"].as<std::string>(), "+79990001122");
}

// ═══════════════════════════════════════════════════════════════════
// Employee — много полей + optional
// ═══════════════════════════════════════════════════════════════════

TEST_F(AppendModelTest, InsertEmployee) {
    // Сначала нужен department и position
    db->appendModel(Department{.id = 0, .name = "IT", .type = DepartmentType::Development});
    db->appendModel(Position{
        .id = 0, .title = "Dev", .category = PositionCategory::Technical_staff,
        .min_salary = 100000, .max_salary = 200000
    });

    db->appendModel(Employee{
        .id = 0,
        .last_name = "Петров",
        .first_name = "Пётр",
        .patronymic = std::nullopt,
        .birth_date = "1995-03-10",
        .gender = std::string("мужской"),
        .phone = std::nullopt,
        .email = "petrov@test.ru",
        .hire_date = "2025-01-15",
        .salary = 180000.0,
        .has_children = false,
        .children_count = 0,
        .department_id = 1,
        .position_id = 1,
        .is_active = true
    });

    EXPECT_EQ(countRows("employee"), 1);

    pqxx::work txn(conn);
    auto r = txn.exec(
        "SELECT last_name, first_name, patronymic, gender, phone, "
        "salary, has_children, children_count, is_active FROM employee"
    );
    EXPECT_EQ(r[0]["last_name"].as<std::string>(), "Петров");
    EXPECT_TRUE(r[0]["patronymic"].is_null());
    EXPECT_EQ(r[0]["gender"].as<std::string>(), "мужской");
    EXPECT_TRUE(r[0]["phone"].is_null());
    EXPECT_DOUBLE_EQ(r[0]["salary"].as<double>(), 180000.0);
    EXPECT_EQ(r[0]["has_children"].as<bool>(), false);
    EXPECT_EQ(r[0]["children_count"].as<int>(), 0);
    EXPECT_EQ(r[0]["is_active"].as<bool>(), true);
}

// ═══════════════════════════════════════════════════════════════════
// DeveloperSpecialization — TEXT[] массивы
// ═══════════════════════════════════════════════════════════════════

TEST_F(AppendModelTest, InsertDeveloperSpecialization) {
    // Нужен employee
    db->appendModel(Department{.id = 0, .name = "IT", .type = DepartmentType::Development});
    db->appendModel(Position{
        .id = 0, .title = "Dev", .category = PositionCategory::Technical_staff,
        .min_salary = 100000, .max_salary = 200000
    });
    db->appendModel(Employee{
        .id = 0, .last_name = "Иванов", .first_name = "Иван",
        .birth_date = "1990-01-01", .email = "dev@test.ru",
        .hire_date = "2024-01-01", .salary = 150000,
        .has_children = false, .children_count = 0,
        .department_id = 1, .position_id = 1, .is_active = true
    });

    db->appendModel(DeveloperSpecialization{
        .employee_id = 1,
        .programming_languages = {"C++", "Rust", "Go"},
        .frameworks = {"Boost", "Qt"},
        .experience_years = 7,
        .backend_exp = true,
        .frontend_exp = false,
        .mobile_exp = false
    });

    EXPECT_EQ(countRows("developerspecialization"), 1);

    pqxx::work txn(conn);
    auto r = txn.exec(
        "SELECT programming_languages, frameworks, experience_years, "
        "backend_exp, frontend_exp FROM developerspecialization"
    );
    // PostgreSQL возвращает массивы в формате {C++,Rust,Go}
    EXPECT_EQ(r[0]["programming_languages"].as<std::string>(), "{C++,Rust,Go}");
    EXPECT_EQ(r[0]["frameworks"].as<std::string>(), "{Boost,Qt}");
    EXPECT_EQ(r[0]["experience_years"].as<int>(), 7);
    EXPECT_EQ(r[0]["backend_exp"].as<bool>(), true);
    EXPECT_EQ(r[0]["frontend_exp"].as<bool>(), false);
}
