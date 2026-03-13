CREATE TABLE Department
(
    department_id SERIAL PRIMARY KEY,
    name          VARCHAR(100) NOT NULL,
    type          VARCHAR(50)  NOT NULL CHECK (type IN
                                               ('разработка', 'тестирование', 'техподдержка', 'управление_проектами',
                                                'административный')),
    created_at    TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Position
(
    position_id SERIAL PRIMARY KEY,
    title       VARCHAR(100) NOT NULL,
    category    VARCHAR(50)  NOT NULL CHECK (category IN ('технический_персонал', 'руководящий_состав')),
    min_salary  DECIMAL(10, 2),
    max_salary  DECIMAL(10, 2),
    created_at  TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Client
(
    client_id         SERIAL PRIMARY KEY,
    name              VARCHAR(200) NOT NULL,
    type              VARCHAR(50)  NOT NULL CHECK (type IN ('корпоративный', 'государственный', 'ИП')),
    address           VARCHAR(300),
    phone             VARCHAR(20),
    email             VARCHAR(100),
    registration_date DATE         NOT NULL DEFAULT CURRENT_DATE,
    created_at        TIMESTAMP             DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE SoftwareLicense
(
    license_id    SERIAL PRIMARY KEY,
    name          VARCHAR(200)   NOT NULL,
    vendor        VARCHAR(100)   NOT NULL,
    license_type  VARCHAR(50)    NOT NULL CHECK (license_type IN ('бессрочная', 'подписка')),
    purchase_date DATE           NOT NULL,
    expiry_date   DATE,
    cost          DECIMAL(12, 2) NOT NULL,
    seats         INTEGER        NOT NULL DEFAULT 1,
    created_at    TIMESTAMP               DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Employee
(
    employee_id    SERIAL PRIMARY KEY,
    last_name      VARCHAR(100)        NOT NULL,
    first_name     VARCHAR(100)        NOT NULL,
    patronymic     VARCHAR(100),
    birth_date     DATE                NOT NULL,
    gender         VARCHAR(10) CHECK (gender IN ('мужской', 'женский')),
    phone          VARCHAR(20),
    email          VARCHAR(100) UNIQUE NOT NULL,
    hire_date      DATE                NOT NULL DEFAULT CURRENT_DATE,
    salary         DECIMAL(10, 2)      NOT NULL CHECK (salary >= 0),
    has_children   BOOLEAN                      DEFAULT FALSE,
    children_count INTEGER                      DEFAULT 0 CHECK (children_count >= 0),
    department_id  INTEGER             NOT NULL REFERENCES Department (department_id) ON DELETE RESTRICT,
    position_id    INTEGER             NOT NULL REFERENCES Position (position_id) ON DELETE RESTRICT,
    is_active      BOOLEAN                      DEFAULT TRUE,
    created_at     TIMESTAMP                    DEFAULT CURRENT_TIMESTAMP,
    updated_at     TIMESTAMP                    DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE DeveloperSpecialization
(
    employee_id           INTEGER PRIMARY KEY REFERENCES Employee (employee_id) ON DELETE CASCADE,
    programming_languages TEXT[]  NOT NULL DEFAULT '{}',
    frameworks            TEXT[]  NOT NULL DEFAULT '{}',
    experience_years      INTEGER NOT NULL CHECK (experience_years >= 0),
    backend_exp           BOOLEAN          DEFAULT FALSE,
    frontend_exp          BOOLEAN          DEFAULT FALSE,
    mobile_exp            BOOLEAN          DEFAULT FALSE,
    created_at            TIMESTAMP        DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE TesterSpecialization
(
    employee_id      INTEGER PRIMARY KEY REFERENCES Employee (employee_id) ON DELETE CASCADE,
    testing_types    TEXT[] NOT NULL DEFAULT '{}',
    automation_tools TEXT[] NOT NULL DEFAULT '{}',
    certifications   TEXT[] NOT NULL DEFAULT '{}',
    created_at       TIMESTAMP       DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE ManagerCertification
(
    certification_id   SERIAL PRIMARY KEY,
    employee_id        INTEGER      NOT NULL REFERENCES Employee (employee_id) ON DELETE CASCADE,
    cert_type          VARCHAR(100) NOT NULL,
    issue_date         DATE         NOT NULL,
    expiry_date        DATE,
    project_complexity VARCHAR(50),
    created_at         TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Project
(
    project_id       SERIAL PRIMARY KEY,
    name             VARCHAR(200)   NOT NULL,
    description      TEXT,
    client_id        INTEGER        NOT NULL REFERENCES Client (client_id) ON DELETE RESTRICT,
    start_date       DATE           NOT NULL,
    planned_end_date DATE           NOT NULL,
    actual_end_date  DATE,
    budget           DECIMAL(15, 2) NOT NULL CHECK (budget >= 0),
    status           VARCHAR(50)    NOT NULL DEFAULT 'в_разработке' CHECK (status IN ('в_разработке', 'завершен', 'отложен', 'закрыт_досрочно')),
    methodology      VARCHAR(50) CHECK (methodology IN ('Waterfall', 'Scrum', 'Kanban', 'гибридная')),
    type             VARCHAR(100)   NOT NULL CHECK (type IN
                                                    ('веб-приложение', 'мобильное_приложение', 'корпоративная_система',
                                                     'микросервис')),
    tech_stack       TEXT[]         NOT NULL DEFAULT '{}',
    is_active        BOOLEAN                 DEFAULT TRUE,
    created_at       TIMESTAMP               DEFAULT CURRENT_TIMESTAMP,
    updated_at       TIMESTAMP               DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE ProjectPhase
(
    phase_id     SERIAL PRIMARY KEY,
    project_id   INTEGER      NOT NULL REFERENCES Project (project_id) ON DELETE CASCADE,
    name         VARCHAR(100) NOT NULL CHECK (name IN
                                              ('анализ_требований', 'проектирование', 'разработка', 'тестирование',
                                               'внедрение', 'сопровождение')),
    start_date   DATE         NOT NULL,
    end_date     DATE,
    order_number INTEGER      NOT NULL,
    created_at   TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE ProjectAssignment
(
    assignment_id   SERIAL PRIMARY KEY,
    employee_id     INTEGER      NOT NULL REFERENCES Employee (employee_id) ON DELETE RESTRICT,
    project_id      INTEGER      NOT NULL REFERENCES Project (project_id) ON DELETE CASCADE,
    phase_id        INTEGER      REFERENCES ProjectPhase (phase_id) ON DELETE SET NULL,
    role            VARCHAR(100) NOT NULL CHECK (role IN ('разработчик', 'тестировщик', 'аналитик', 'менеджер_проекта',
                                                          'архитектор', 'технический_писатель')),
    start_date      DATE         NOT NULL DEFAULT CURRENT_DATE,
    end_date        DATE,
    hours_allocated INTEGER CHECK (hours_allocated > 0),
    created_at      TIMESTAMP             DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE LicenseAllocation
(
    allocation_id   SERIAL PRIMARY KEY,
    license_id      INTEGER NOT NULL REFERENCES SoftwareLicense (license_id) ON DELETE CASCADE,
    project_id      INTEGER REFERENCES Project (project_id) ON DELETE SET NULL,
    allocated_seats INTEGER NOT NULL CHECK (allocated_seats > 0),
    allocation_date DATE    NOT NULL DEFAULT CURRENT_DATE,
    created_at      TIMESTAMP        DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE CloudResource
(
    resource_id   SERIAL PRIMARY KEY,
    project_id    INTEGER        NOT NULL REFERENCES Project (project_id) ON DELETE CASCADE,
    provider      VARCHAR(50)    NOT NULL CHECK (provider IN ('AWS', 'Azure', 'GCP', 'Yandex_Cloud', 'другой')),
    type          VARCHAR(100)   NOT NULL,
    configuration JSONB,
    cost_per_hour DECIMAL(10, 4) NOT NULL CHECK (cost_per_hour >= 0),
    created_at    TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE WorkLog
(
    log_id           SERIAL PRIMARY KEY,
    employee_id      INTEGER       NOT NULL REFERENCES Employee (employee_id) ON DELETE RESTRICT,
    project_id       INTEGER       NOT NULL REFERENCES Project (project_id) ON DELETE CASCADE,
    task_description TEXT          NOT NULL,
    work_date        DATE          NOT NULL DEFAULT CURRENT_DATE,
    hours_spent      DECIMAL(5, 2) NOT NULL CHECK (hours_spent > 0 AND hours_spent <= 24),
    created_at       TIMESTAMP              DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Bug
(
    bug_id      SERIAL PRIMARY KEY,
    project_id  INTEGER      NOT NULL REFERENCES Project (project_id) ON DELETE CASCADE,
    title       VARCHAR(300) NOT NULL,
    description TEXT         NOT NULL,
    severity    VARCHAR(50)  NOT NULL CHECK (severity IN ('блокирующая', 'критическая', 'средняя', 'низкая')),
    status      VARCHAR(50)  NOT NULL DEFAULT 'новая' CHECK (status IN ('новая', 'в_работе', 'исправлена', 'проверена')),
    found_date  DATE         NOT NULL DEFAULT CURRENT_DATE,
    fixed_date  DATE,
    found_by    INTEGER      REFERENCES Employee (employee_id) ON DELETE SET NULL,
    fixed_by    INTEGER      REFERENCES Employee (employee_id) ON DELETE SET NULL,
    created_at  TIMESTAMP             DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Release
(
    release_id       SERIAL PRIMARY KEY,
    project_id       INTEGER     NOT NULL REFERENCES Project (project_id) ON DELETE CASCADE,
    version          VARCHAR(50) NOT NULL,
    release_date     DATE        NOT NULL DEFAULT CURRENT_DATE,
    changelog        TEXT,
    fixed_bugs_count INTEGER              DEFAULT 0 CHECK (fixed_bugs_count >= 0),
    created_at       TIMESTAMP            DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE Documentation
(
    doc_id        SERIAL PRIMARY KEY,
    project_id    INTEGER      NOT NULL REFERENCES Project (project_id) ON DELETE CASCADE,
    type          VARCHAR(100) NOT NULL CHECK (type IN ('архитектура', 'API', 'руководство_пользователя',
                                                        'руководство_администратора', 'спецификация')),
    author_id     INTEGER      NOT NULL REFERENCES Employee (employee_id) ON DELETE RESTRICT,
    creation_date DATE         NOT NULL DEFAULT CURRENT_DATE,
    last_update   DATE                  DEFAULT CURRENT_DATE,
    storage_path  VARCHAR(500),
    created_at    TIMESTAMP             DEFAULT CURRENT_TIMESTAMP
);