#include <gtest/gtest.h>

#include "DataBase.hpp"
#include "Models.hpp"

using namespace lib::Models;

static const std::string connStr =
    "host=127.0.0.1 port=5432 dbname=db user=user password=pass";

class DeleteModelTest : public ::testing::Test {
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

    bool rowExists(const std::string& table, const std::string& pkColumn, const std::string& id) {
        pqxx::work txn(conn);
        auto r = txn.exec("SELECT COUNT(*) FROM " + table + " WHERE " + pkColumn + " = " + id);
        return r[0][0].as<int>() > 0;
    }
};

TEST_F(DeleteModelTest, DeleteDepartmentById) {
    db->appendModel(Department{
        .id = 0,
        .name = "Тестовый отдел",
        .type = DepartmentType::Testing
    });

    EXPECT_EQ(countRows("department"), 1);
    EXPECT_TRUE(rowExists("department", "department_id", "1"));

    db->deleteById(Department{.id=0,.name="",.type=DepartmentType::Development,.created_at=""}, "1");

    EXPECT_EQ(countRows("department"), 0);
    EXPECT_FALSE(rowExists("department", "department_id", "1"));
}

TEST_F(DeleteModelTest, DeletePositionById) {
    db->appendModel(Position{
        .id = 0,
        .title = "Junior Dev",
        .category = PositionCategory::Technical_staff,
        .min_salary = 80000.0,
        .max_salary = 150000.0
    });

    EXPECT_EQ(countRows("position"), 1);

    db->deleteById(Position{.id=0,.title="",.category=PositionCategory::Technical_staff,.min_salary=0,.max_salary=0,.created_at=""}, "1");

    EXPECT_EQ(countRows("position"), 0);
}

TEST_F(DeleteModelTest, DeleteClientById) {
    db->appendModel(Client{
        .id = 0,
        .name = "ООО Тест",
        .type = ClientType::Corporate,
        .address = std::nullopt,
        .phone = std::nullopt,
        .email = "test@test.ru",
        .registration_date = "2024-06-15"
    });

    EXPECT_EQ(countRows("client"), 1);

    db->deleteById(Client{.id=0,.name="",.type=ClientType::Corporate,.created_at=""}, "1");

    EXPECT_EQ(countRows("client"), 0);
}

TEST_F(DeleteModelTest, DeleteEmployeeById) {
    db->appendModel(Department{.id=0,.name="IT",.type=DepartmentType::Development});
    db->appendModel(Position{
        .id=0,.title="Dev",.category=PositionCategory::Technical_staff,
        .min_salary=100000,.max_salary=200000
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

    db->deleteById(Employee{.id=0,.last_name="",.first_name="",.hire_date="",.salary=0,.has_children=false,.children_count=0,.department_id=0,.position_id=0,.is_active=false,.created_at="",.updated_at=""}, "1");

    EXPECT_EQ(countRows("employee"), 0);
}

TEST_F(DeleteModelTest, DeleteDeveloperSpecializationById) {
    db->appendModel(Department{.id=0,.name="IT",.type=DepartmentType::Development});
    db->appendModel(Position{
        .id=0,.title="Dev",.category=PositionCategory::Technical_staff,
        .min_salary=100000,.max_salary=200000
    });
    db->appendModel(Employee{
        .id=0,.last_name="Иванов",.first_name="Иван",.birth_date="1990-01-01",
        .email="dev@test.ru",.hire_date="2024-01-01",.salary=150000,
        .has_children=false,.children_count=0,.department_id=1,.position_id=1,.is_active=true
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

    db->deleteById(DeveloperSpecialization{.employee_id=0,.created_at=""}, "1");

    EXPECT_EQ(countRows("developerspecialization"), 0);
}

TEST_F(DeleteModelTest, DeleteNonExistentDepartment) {
    EXPECT_EQ(countRows("department"), 0);

    db->deleteById(Department{.id=0,.name="",.type=DepartmentType::Development,.created_at=""}, "999");

    EXPECT_EQ(countRows("department"), 0);
}

TEST_F(DeleteModelTest, DeleteMultipleRecords) {
    db->appendModel(Department{.id=0,.name="IT",.type=DepartmentType::Development});
    db->appendModel(Department{.id=0,.name="QA",.type=DepartmentType::Testing});
    db->appendModel(Department{.id=0,.name="HR",.type=DepartmentType::Administrative});

    EXPECT_EQ(countRows("department"), 3);

    db->deleteById(Department{.id=0,.name="",.type=DepartmentType::Development,.created_at=""}, "2");

    EXPECT_EQ(countRows("department"), 2);
    EXPECT_FALSE(rowExists("department", "department_id", "2"));
    EXPECT_TRUE(rowExists("department", "department_id", "1"));
    EXPECT_TRUE(rowExists("department", "department_id", "3"));
}

TEST_F(DeleteModelTest, DeleteClientWithOptionalFields) {
    db->appendModel(Client{
        .id = 0,
        .name = "ИП Иванов",
        .type = ClientType::Individual_entrepreneur,
        .address = std::string("ул. Пушкина, 1"),
        .phone = std::string("+79990001122"),
        .email = "ivanov@biz.ru",
        .registration_date = "2025-01-01"
    });

    EXPECT_EQ(countRows("client"), 1);

    db->deleteById(Client{.id=0,.name="",.type=ClientType::Corporate,.created_at=""}, "1");

    EXPECT_EQ(countRows("client"), 0);
}

TEST_F(DeleteModelTest, DeleteAndVerifyGone) {
    db->appendModel(Department{
        .id = 0,
        .name = "Тест",
        .type = DepartmentType::Testing
    });

    auto before = db->getModelById(Department{.id=0,.name="",.type=DepartmentType::Development,.created_at=""}, "1");
    ASSERT_TRUE(before.has_value());

    db->deleteById(Department{.id=0,.name="",.type=DepartmentType::Development,.created_at=""}, "1");

    auto after = db->getModelById(Department{.id=0,.name="",.type=DepartmentType::Development,.created_at=""}, "1");
    EXPECT_FALSE(after.has_value());
}
