#include <iostream>
#include <memory>
#include <print>

#include <DataBase.hpp>
#include <Models.hpp>

int main() {
    try {
        DataBase db("host=127.0.0.1 port=5432 dbname=db user=user password=pass");
        std::println("Tables: {}", db.getTablesNames());

        // 1. Department — enum
        std::println("Inserting Department...");
        db.appendModel(lib::Models::Department{
            .id = 0,
            .name = "Разработка",
            .type = lib::Models::DepartmentType::Development
        });
        std::println("OK");

        // 2. Position — enum + doubles
        std::println("Inserting Position...");
        db.appendModel(lib::Models::Position{
            .id = 0,
            .title = "Senior Developer",
            .category = lib::Models::PositionCategory::Technical_staff,
            .min_salary = 150000.0,
            .max_salary = 300000.0
        });
        std::println("OK");

        // 3. Client — optional fields
        std::println("Inserting Client...");
        db.appendModel(lib::Models::Client{
            .id = 0,
            .name = "ООО Ромашка",
            .type = lib::Models::ClientType::Corporate,
            .address = std::nullopt,           // NULL
            .phone = std::string("+79991234567"),
            .email = "info@romashka.ru",
            .registration_date = "2024-01-15"
        });
        std::println("OK");

        // 4. Employee — many fields + optional
        std::println("Inserting Employee...");
        db.appendModel(lib::Models::Employee{
            .id = 0,
            .last_name = "Иванов",
            .first_name = "Иван",
            .patronymic = std::string("Иванович"),
            .birth_date = "1990-05-20",
            .gender = std::string("мужской"),
            .phone = std::nullopt,
            .email = "ivanov@test.ru",
            .hire_date = "2024-03-01",
            .salary = 200000.0,
            .has_children = true,
            .children_count = 2,
            .department_id = 1,
            .position_id = 1,
            .is_active = true
        });
        std::println("OK");

        // 5. DeveloperSpecialization — arrays
        std::println("Inserting DeveloperSpecialization...");
        db.appendModel(lib::Models::DeveloperSpecialization{
            .employee_id = 1,
            .programming_languages = {"C++", "Python", "Rust"},
            .frameworks = {"Qt", "Boost"},
            .experience_years = 5,
            .backend_exp = true,
            .frontend_exp = false,
            .mobile_exp = false
        });
        std::println("OK");

        std::println("All models inserted successfully!");

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
