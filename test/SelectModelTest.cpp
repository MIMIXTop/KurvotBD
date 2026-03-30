#include <gtest/gtest.h>

#include "DataBase.hpp"
#include "Models.hpp"

using namespace lib::Models;

static const std::string connStr =
    "host=127.0.0.1 port=5432 dbname=db user=user password=pass";

class SelectModelTest : public ::testing::Test {
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
};

// ═══════════════════════════════════════════════════════════════════
// Department — enum
// ═══════════════════════════════════════════════════════════════════

TEST_F(SelectModelTest, SelectDepartmentById) {
    db->appendModel(Department{
        .id = 0, .name = "Тест", .type = DepartmentType::Testing
    });

    auto result = db->getModelById(Department{.id=0,.name="",.type=DepartmentType::Development,.created_at=""}, "1");

    ASSERT_TRUE(result.has_value());
    auto dept = std::get<Department>(*result);
    EXPECT_EQ(dept.id, 1);
    EXPECT_EQ(dept.name, "Тест");
    EXPECT_EQ(dept.type, DepartmentType::Testing);
}

// ═══════════════════════════════════════════════════════════════════
// Position — enum + doubles
// ═══════════════════════════════════════════════════════════════════

TEST_F(SelectModelTest, SelectPositionById) {
    db->appendModel(Position{
        .id = 0, .title = "Dev", .category = PositionCategory::Management,
        .min_salary = 100000.0, .max_salary = 250000.0
    });

    auto result = db->getModelById(Position{.id=0,.title="",.category=PositionCategory::Technical_staff,.min_salary=0,.max_salary=0,.created_at=""}, "1");

    ASSERT_TRUE(result.has_value());
    auto pos = std::get<Position>(*result);
    EXPECT_EQ(pos.title, "Dev");
    EXPECT_EQ(pos.category, PositionCategory::Management);
    EXPECT_DOUBLE_EQ(pos.min_salary, 100000.0);
    EXPECT_DOUBLE_EQ(pos.max_salary, 250000.0);
}

// ═══════════════════════════════════════════════════════════════════
// Client — optional NULL
// ═══════════════════════════════════════════════════════════════════

TEST_F(SelectModelTest, SelectClientWithNullOptional) {
    db->appendModel(Client{
        .id = 0, .name = "ООО Тест", .type = ClientType::Corporate,
        .address = std::nullopt, .phone = std::nullopt,
        .email = "test@test.ru", .registration_date = "2024-01-01"
    });

    auto result = db->getModelById(Client{.id=0,.name="",.type=ClientType::Corporate,.created_at=""}, "1");

    ASSERT_TRUE(result.has_value());
    auto c = std::get<Client>(*result);
    EXPECT_EQ(c.name, "ООО Тест");
    EXPECT_EQ(c.type, ClientType::Corporate);
    EXPECT_FALSE(c.address.has_value());
    EXPECT_FALSE(c.phone.has_value());
    EXPECT_EQ(c.email, "test@test.ru");
}

// ═══════════════════════════════════════════════════════════════════
// Client — optional со значениями
// ═══════════════════════════════════════════════════════════════════

TEST_F(SelectModelTest, SelectClientWithValueOptional) {
    db->appendModel(Client{
        .id = 0, .name = "ИП Иванов", .type = ClientType::Individual_entrepreneur,
        .address = std::string("ул. Пушкина, 1"),
        .phone = std::string("+79990001122"),
        .email = "ivanov@biz.ru", .registration_date = "2025-01-01"
    });

    auto result = db->getModelById(Client{.id=0,.name="",.type=ClientType::Corporate,.created_at=""}, "1");

    ASSERT_TRUE(result.has_value());
    auto c = std::get<Client>(*result);
    EXPECT_EQ(c.address.value(), "ул. Пушкина, 1");
    EXPECT_EQ(c.phone.value(), "+79990001122");
}

// ═══════════════════════════════════════════════════════════════════
// Employee — много полей + optional
// ═══════════════════════════════════════════════════════════════════

TEST_F(SelectModelTest, SelectEmployeeById) {
    db->appendModel(Department{.id=0,.name="IT",.type=DepartmentType::Development});
    db->appendModel(Position{.id=0,.title="Dev",.category=PositionCategory::Technical_staff,.min_salary=100000,.max_salary=200000});
    db->appendModel(Employee{
        .id = 0, .last_name = "Петров", .first_name = "Пётр",
        .patronymic = std::string("Иванович"),
        .birth_date = "1995-03-10", .gender = std::string("мужской"),
        .phone = std::nullopt, .email = "petrov@test.ru",
        .hire_date = "2025-01-15", .salary = 180000.0,
        .has_children = false, .children_count = 0,
        .department_id = 1, .position_id = 1, .is_active = true
    });

    auto result = db->getModelById(Employee{.id=0,.last_name="",.first_name="",.hire_date="",.salary=0,.has_children=false,.children_count=0,.department_id=0,.position_id=0,.is_active=false,.created_at="",.updated_at=""}, "1");

    ASSERT_TRUE(result.has_value());
    auto e = std::get<Employee>(*result);
    EXPECT_EQ(e.last_name, "Петров");
    EXPECT_EQ(e.first_name, "Пётр");
    EXPECT_EQ(e.patronymic.value(), "Иванович");
    EXPECT_EQ(e.gender.value(), "мужской");
    EXPECT_FALSE(e.phone.has_value());
    EXPECT_DOUBLE_EQ(e.salary, 180000.0);
    EXPECT_EQ(e.is_active, true);
}

// ═══════════════════════════════════════════════════════════════════
// DeveloperSpecialization — TEXT[] массивы
// ═══════════════════════════════════════════════════════════════════

TEST_F(SelectModelTest, SelectDeveloperSpecializationById) {
    db->appendModel(Department{.id=0,.name="IT",.type=DepartmentType::Development});
    db->appendModel(Position{.id=0,.title="Dev",.category=PositionCategory::Technical_staff,.min_salary=100000,.max_salary=200000});
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
        .backend_exp = true, .frontend_exp = false, .mobile_exp = false
    });

    auto result = db->getModelById(DeveloperSpecialization{.employee_id=0,.created_at=""}, "1");

    ASSERT_TRUE(result.has_value());
    auto d = std::get<DeveloperSpecialization>(*result);
    EXPECT_EQ(d.programming_languages, (std::vector<std::string>{"C++", "Rust", "Go"}));
    EXPECT_EQ(d.frameworks, (std::vector<std::string>{"Boost", "Qt"}));
    EXPECT_EQ(d.experience_years, 7);
    EXPECT_EQ(d.backend_exp, true);
    EXPECT_EQ(d.frontend_exp, false);
}

// ═══════════════════════════════════════════════════════════════════
// Non-existent ID — nullopt
// ═══════════════════════════════════════════════════════════════════

TEST_F(SelectModelTest, SelectNonExistentDepartment) {
    auto result = db->getModelById(Department{.id=0,.name="",.type=DepartmentType::Development,.created_at=""}, "999");
    EXPECT_FALSE(result.has_value());
}

TEST_F(SelectModelTest, SelectNonExistentClient) {
    auto result = db->getModelById(Client{.id=0,.name="",.type=ClientType::Corporate,.created_at=""}, "42");
    EXPECT_FALSE(result.has_value());
}
