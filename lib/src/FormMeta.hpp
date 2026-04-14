#pragma once
#include <string>
#include <vector>
#include <optional>
#include <variant>

namespace lib::FormMeta {

    enum class FieldType {
        Text,
        TextArea,
        Date,
        OptionalDate,
        Number,
        DoubleNumber,
        Boolean,
        Enum,
        ForeignKey,
        OptionalForeignKey,
        StringArray
    };

    struct EnumOption {
        std::string value;
        std::string display_name;
    };

    struct ForeignKeyInfo {
        std::string source_table;
        std::string id_column;
        std::string display_column;
    };

    struct FieldDefinition {
        std::string name;
        std::string display_name;
        FieldType type = FieldType::Text;
        std::vector<EnumOption> enum_options;
        std::optional<ForeignKeyInfo> fk_info;
        bool is_primary_key = false;
        bool is_auto_generated = false;
    };

    struct FormDefinition {
        std::string model_name;
        std::string table_name;
        std::vector<FieldDefinition> fields;
    };

    inline FormDefinition getFormDefinition(const std::string& modelName) {
        if (modelName == "Department") {
            return FormDefinition{
                "Department", "department", {
                    FieldDefinition{"department_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"name", "Название", FieldType::Text},
                    FieldDefinition{"type", "Тип", FieldType::Enum, {
                        {"разработка", "Разработка"},
                        {"тестирование", "Тестирование"},
                        {"техподдержка", "Техподдержка"},
                        {"управление_проектами", "Управление проектами"},
                        {"административный", "Административный"}
                    }},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "Position") {
            return FormDefinition{
                "Position", "position", {
                    FieldDefinition{"position_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"title", "Должность", FieldType::Text},
                    FieldDefinition{"category", "Категория", FieldType::Enum, {
                        {"технический_персонал", "Технический персонал"},
                        {"руководство", "Руководство"}
                    }},
                    FieldDefinition{"min_salary", "Мин. зарплата", FieldType::DoubleNumber},
                    FieldDefinition{"max_salary", "Макс. зарплата", FieldType::DoubleNumber},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "Client") {
            return FormDefinition{
                "Client", "client", {
                    FieldDefinition{"client_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"name", "Название", FieldType::Text},
                    FieldDefinition{"type", "Тип", FieldType::Enum, {
                        {"корпоративный", "Корпоративный"},
                        {"государственный", "Государственный"},
                        {"ИП", "ИП"}
                    }},
                    FieldDefinition{"address", "Адрес", FieldType::Text},
                    FieldDefinition{"phone", "Телефон", FieldType::Text},
                    FieldDefinition{"email", "Email", FieldType::Text},
                    FieldDefinition{"registration_date", "Дата регистрации", FieldType::Date},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "SoftwareLicense") {
            return FormDefinition{
                "SoftwareLicense", "softwarelicense", {
                    FieldDefinition{"license_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"name", "Название", FieldType::Text},
                    FieldDefinition{"vendor", "Поставщик", FieldType::Text},
                    FieldDefinition{"license_type", "Тип лицензии", FieldType::Enum, {
                        {"бессрочная", "Бессрочная"},
                        {"подписка", "Подписка"}
                    }},
                    FieldDefinition{"purchase_date", "Дата покупки", FieldType::Date},
                    FieldDefinition{"expiry_date", "Дата истечения", FieldType::OptionalDate},
                    FieldDefinition{"cost", "Стоимость", FieldType::DoubleNumber},
                    FieldDefinition{"seats", "Количество мест", FieldType::Number},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "Employee") {
            return FormDefinition{
                "Employee", "employee", {
                    FieldDefinition{"employee_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"last_name", "Фамилия", FieldType::Text},
                    FieldDefinition{"first_name", "Имя", FieldType::Text},
                    FieldDefinition{"patronymic", "Отчество", FieldType::Text},
                    FieldDefinition{"birth_date", "Дата рождения", FieldType::Date},
                    FieldDefinition{"gender", "Пол", FieldType::Enum, {
                        {"м", "Мужской"},
                        {"ж", "Женский"}
                    }},
                    FieldDefinition{"phone", "Телефон", FieldType::Text},
                    FieldDefinition{"email", "Email", FieldType::Text},
                    FieldDefinition{"hire_date", "Дата найма", FieldType::Date},
                    FieldDefinition{"salary", "Зарплата", FieldType::DoubleNumber},
                    FieldDefinition{"has_children", "Есть дети", FieldType::Boolean},
                    FieldDefinition{"children_count", "Количество детей", FieldType::Number},
                    FieldDefinition{"department_id", "Отдел", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"department", "department_id", "name"}},
                    FieldDefinition{"position_id", "Должность", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"position", "position_id", "title"}},
                    FieldDefinition{"is_active", "Активен", FieldType::Boolean},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true},
                    FieldDefinition{"updated_at", "Обновлено", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "Project") {
            return FormDefinition{
                "Project", "project", {
                    FieldDefinition{"project_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"name", "Название", FieldType::Text},
                    FieldDefinition{"description", "Описание", FieldType::TextArea},
                    FieldDefinition{"client_id", "Клиент", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"client", "client_id", "name"}},
                    FieldDefinition{"start_date", "Дата начала", FieldType::Date},
                    FieldDefinition{"planned_end_date", "Планируемая дата окончания", FieldType::Date},
                    FieldDefinition{"actual_end_date", "Фактическая дата окончания", FieldType::OptionalDate},
                    FieldDefinition{"budget", "Бюджет", FieldType::DoubleNumber},
                    FieldDefinition{"status", "Статус", FieldType::Enum, {
                        {"в_разработке", "В разработке"},
                        {"завершен", "Завершен"},
                        {"отложен", "Отложен"},
                        {"закрыт_досрочно", "Закрыт досрочно"}
                    }},
                    FieldDefinition{"methodology", "Методология", FieldType::Enum, {
                        {"Waterfall", "Waterfall"},
                        {"Scrum", "Scrum"},
                        {"Kanban", "Kanban"},
                        {"гибридная", "Гибридная"}
                    }},
                    FieldDefinition{"type", "Тип", FieldType::Enum, {
                        {"веб_приложение", "Веб-приложение"},
                        {"мобильное_приложение", "Мобильное приложение"},
                        {"корпоративная_система", "Корпоративная система"},
                        {"микросервис", "Микросервис"}
                    }},
                    FieldDefinition{"tech_stack", "Технологический стек", FieldType::StringArray},
                    FieldDefinition{"is_active", "Активен", FieldType::Boolean},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true},
                    FieldDefinition{"updated_at", "Обновлено", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "Bug") {
            return FormDefinition{
                "Bug", "bug", {
                    FieldDefinition{"bug_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"project_id", "Project", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"project", "project_id", "name"}},
                    FieldDefinition{"title", "Title", FieldType::Text},
                    FieldDefinition{"description", "Description", FieldType::TextArea},
                    FieldDefinition{"severity", "Severity", FieldType::Enum, {
                        {"блокирующая", "Блокирующая"},
                        {"критическая", "Критическая"},
                        {"средняя", "Средняя"},
                        {"низкая", "Низкая"}
                    }},
                    FieldDefinition{"status", "Status", FieldType::Enum, {
                        {"новая", "Новая"},
                        {"в_работе", "В работе"},
                        {"исправлена", "Исправлена"},
                        {"проверена", "Проверена"}
                    }},
                    FieldDefinition{"found_date", "Found Date", FieldType::Date},
                    FieldDefinition{"fixed_date", "Fixed Date", FieldType::OptionalDate},
                    FieldDefinition{"found_by", "Found By", FieldType::OptionalForeignKey, {}, 
                        ForeignKeyInfo{"employee", "employee_id", "last_name"}},
                    FieldDefinition{"fixed_by", "Fixed By", FieldType::OptionalForeignKey, {}, 
                        ForeignKeyInfo{"employee", "employee_id", "last_name"}},
                    FieldDefinition{"created_at", "Created At", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "CloudResource") {
            return FormDefinition{
                "CloudResource", "cloudresource", {
                    FieldDefinition{"resource_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"project_id", "Проект", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"project", "project_id", "name"}},
                    FieldDefinition{"provider", "Провайдер", FieldType::Enum, {
                        {"AWS", "AWS"},
                        {"Azure", "Azure"},
                        {"GCP", "GCP"},
                        {"Yandex_Cloud", "Yandex Cloud"},
                        {"Other", "Другой"}
                    }},
                    FieldDefinition{"type", "Тип ресурса", FieldType::Text},
                    FieldDefinition{"configuration", "Конфигурация (JSON)", FieldType::TextArea},
                    FieldDefinition{"cost_per_hour", "Стоимость в час", FieldType::DoubleNumber},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "WorkLog") {
            return FormDefinition{
                "WorkLog", "worklog", {
                    FieldDefinition{"log_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"employee_id", "Сотрудник", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"employee", "employee_id", "last_name"}},
                    FieldDefinition{"project_id", "Проект", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"project", "project_id", "name"}},
                    FieldDefinition{"task_description", "Описание задачи", FieldType::TextArea},
                    FieldDefinition{"work_date", "Дата работы", FieldType::Date},
                    FieldDefinition{"hours_spent", "Часов затрачено", FieldType::DoubleNumber},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "Release") {
            return FormDefinition{
                "Release", "release", {
                    FieldDefinition{"release_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"project_id", "Проект", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"project", "project_id", "name"}},
                    FieldDefinition{"version", "Версия", FieldType::Text},
                    FieldDefinition{"release_date", "Дата релиза", FieldType::Date},
                    FieldDefinition{"changelog", "Список изменений", FieldType::TextArea},
                    FieldDefinition{"fixed_bugs_count", "Исправлено ошибок", FieldType::Number},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "Documentation") {
            return FormDefinition{
                "Documentation", "documentation", {
                    FieldDefinition{"doc_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"project_id", "Проект", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"project", "project_id", "name"}},
                    FieldDefinition{"type", "Тип документации", FieldType::Enum, {
                        {"архитектура", "Архитектура"},
                        {"API", "API"},
                        {"руководство_пользователя", "Руководство пользователя"},
                        {"руководство_администратора", "Руководство администратора"},
                        {"спецификация", "Спецификация"}
                    }},
                    FieldDefinition{"author_id", "Автор", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"employee", "employee_id", "last_name"}},
                    FieldDefinition{"creation_date", "Дата создания", FieldType::Date},
                    FieldDefinition{"last_update", "Последнее обновление", FieldType::Date},
                    FieldDefinition{"storage_path", "Путь к файлу", FieldType::Text},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "ProjectSpecification") {
            return FormDefinition{
                "ProjectSpecification", "projectspecification", {
                    FieldDefinition{"spec_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"project_id", "Проект", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"project", "project_id", "name"}},
                    FieldDefinition{"document_text", "Текст документа", FieldType::TextArea},
                    FieldDefinition{"version", "Версия", FieldType::Number},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true},
                    FieldDefinition{"updated_by", "Обновил", FieldType::Number, {}, {}, false, true}
                }
            };
        }
        if (modelName == "DeveloperSpecialization") {
            return FormDefinition{
                "DeveloperSpecialization", "developer_specialization", {
                    FieldDefinition{"employee_id", "Сотрудник", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"employee", "employee_id", "last_name"}, true},
                    FieldDefinition{"programming_languages", "Языки программирования", FieldType::StringArray},
                    FieldDefinition{"frameworks", "Фреймворки", FieldType::StringArray},
                    FieldDefinition{"experience_years", "Лет опыта", FieldType::Number},
                    FieldDefinition{"backend_exp", "Опыт Backend", FieldType::Boolean},
                    FieldDefinition{"frontend_exp", "Опыт Frontend", FieldType::Boolean},
                    FieldDefinition{"mobile_exp", "Опыт Mobile", FieldType::Boolean},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "TesterSpecialization") {
            return FormDefinition{
                "TesterSpecialization", "tester_specialization", {
                    FieldDefinition{"employee_id", "Сотрудник", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"employee", "employee_id", "last_name"}, true},
                    FieldDefinition{"testing_types", "Типы тестирования", FieldType::StringArray},
                    FieldDefinition{"automation_tools", "Инструменты автоматизации", FieldType::StringArray},
                    FieldDefinition{"certifications", "Сертификаты", FieldType::StringArray},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "ManagerCertification") {
            return FormDefinition{
                "ManagerCertification", "manager_certification", {
                    FieldDefinition{"certification_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"employee_id", "Сотрудник", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"employee", "employee_id", "last_name"}},
                    FieldDefinition{"cert_type", "Тип сертификата", FieldType::Text},
                    FieldDefinition{"issue_date", "Дата выдачи", FieldType::Date},
                    FieldDefinition{"expiry_date", "Дата истечения", FieldType::OptionalDate},
                    FieldDefinition{"project_complexity", "Сложность проектов", FieldType::Text},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "ProjectPhase") {
            return FormDefinition{
                "ProjectPhase", "project_phase", {
                    FieldDefinition{"phase_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"project_id", "Проект", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"project", "project_id", "name"}},
                    FieldDefinition{"name", "Название фазы", FieldType::Text},
                    FieldDefinition{"start_date", "Дата начала", FieldType::Date},
                    FieldDefinition{"end_date", "Дата окончания", FieldType::OptionalDate},
                    FieldDefinition{"order_number", "Порядковый номер", FieldType::Number},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "ProjectAssignment") {
            return FormDefinition{
                "ProjectAssignment", "project_assignment", {
                    FieldDefinition{"assignment_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"employee_id", "Сотрудник", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"employee", "employee_id", "last_name"}},
                    FieldDefinition{"project_id", "Проект", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"project", "project_id", "name"}},
                    FieldDefinition{"phase_id", "Фаза проекта", FieldType::OptionalForeignKey, {}, 
                        ForeignKeyInfo{"project_phase", "phase_id", "name"}},
                    FieldDefinition{"role", "Роль", FieldType::Text},
                    FieldDefinition{"start_date", "Дата начала", FieldType::Date},
                    FieldDefinition{"end_date", "Дата окончания", FieldType::OptionalDate},
                    FieldDefinition{"hours_allocated", "Выделено часов", FieldType::Number},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }
        if (modelName == "LicenseAllocation") {
            return FormDefinition{
                "LicenseAllocation", "license_allocation", {
                    FieldDefinition{"allocation_id", "ID", FieldType::Number, {}, {}, true, true},
                    FieldDefinition{"license_id", "Лицензия", FieldType::ForeignKey, {}, 
                        ForeignKeyInfo{"softwarelicense", "license_id", "name"}},
                    FieldDefinition{"project_id", "Проект", FieldType::OptionalForeignKey, {}, 
                        ForeignKeyInfo{"project", "project_id", "name"}},
                    FieldDefinition{"allocated_seats", "Выделено мест", FieldType::Number},
                    FieldDefinition{"allocation_date", "Дата выделения", FieldType::Date},
                    FieldDefinition{"created_at", "Создано", FieldType::Text, {}, {}, false, true}
                }
            };
        }

        return FormDefinition{};
    }
}
