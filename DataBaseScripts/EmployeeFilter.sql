CREATE OR REPLACE FUNCTION get_employee_by_filters(
    p_department_ids INT[] DEFAULT NULL,
    p_position_ids int[] DEFAULT NULL,
    p_min_experience INT DEFAULT NULL,
    p_max_experience INT DEFAULT NULL,
    p_min_age INT DEFAULT NULL,
    p_max_age INT DEFAULT NULL,
    p_min_salary DECIMAL(10, 2) DEFAULT NULL,
    p_max_salary DECIMAL(10, 2) DEFAULT NULL,
    p_cert_type VARCHAR DEFAULT NULL,
    p_is_active BOOLEAN DEFAULT TRUE)
    RETURNS TABLE
            (
                employee_id      INT,
                department_id    INT,
                position_id      INT,

                full_name        TEXT,
                department_name  VARCHAR(100),
                position_title   VARCHAR(100),
                age              INT,
                experience_years INT,
                salary           DECIMAL(10, 2),
                email            VARCHAR(100),
                phone            VARCHAR(20),
                cert_type        VARCHAR(100),
                cert_issue_date  DATE,

                programming_languages TEXT[],
                frameworks            TEXT[],
                backend_exp           BOOLEAN,
                frontend_exp          BOOLEAN,
                mobile_exp            BOOLEAN,
                dev_experience_years  INT,

                testing_types     TEXT[],
                automation_tools  TEXT[],
                certifications    TEXT[],

                manager_cert_type     VARCHAR(100),
                manager_issue_date    DATE,
                manager_expiry_date   DATE,
                project_complexity    VARCHAR(50)
            )
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN QUERY
        SELECT e.employee_id,
               e.department_id,
               e.position_id,

               CONCAT_WS(' ', e.last_name, e.first_name, e.patronymic) AS full_name,
               d.name                                                  AS department_name,
               p.title                                                 AS position_title,
               EXTRACT(YEAR FROM AGE(CURRENT_DATE, e.birth_date))::INT AS age,
               EXTRACT(YEAR FROM AGE(CURRENT_DATE, e.hire_date))::INT  AS experience_years,
               e.salary,
               e.email,
               e.phone,
               mc.cert_type,
               mc.issue_date,

               ds.programming_languages,
               ds.frameworks,
               ds.backend_exp,
               ds.frontend_exp,
               ds.mobile_exp,
               ds.experience_years,

               ts.testing_types,
               ts.automation_tools,
               ts.certifications,

               mc2.cert_type,
               mc2.issue_date,
               mc2.expiry_date,
               mc2.project_complexity
        FROM Employee e
                 JOIN Department d ON e.department_id = d.department_id
                 JOIN Position p ON e.position_id = p.position_id
                 LEFT JOIN DeveloperSpecialization ds ON e.employee_id = ds.employee_id
                 LEFT JOIN TesterSpecialization ts ON e.employee_id = ts.employee_id
                 LEFT JOIN ManagerCertification mc ON e.employee_id = mc.employee_id
                     AND (p_cert_type IS NULL OR mc.cert_type ILIKE '%' || p_cert_type || '%')
                 LEFT JOIN ManagerCertification mc2 ON e.employee_id = mc2.employee_id
        WHERE (p_is_active IS NULL OR e.is_active = p_is_active)

          AND (p_department_ids IS NULL OR e.department_id = ANY (p_department_ids))
          AND (p_position_ids IS NULL OR e.position_id = ANY (p_position_ids))
          AND (p_min_experience IS NULL OR EXTRACT(YEAR FROM AGE(CURRENT_DATE, e.hire_date))::INT >= p_min_experience)
          AND (p_max_experience IS NULL OR EXTRACT(YEAR FROM AGE(CURRENT_DATE, e.hire_date))::INT <= p_max_experience)
          AND (p_min_age IS NULL OR EXTRACT(YEAR FROM AGE(CURRENT_DATE, e.birth_date))::INT >= p_min_age)
          AND (p_max_age IS NULL OR EXTRACT(YEAR FROM AGE(CURRENT_DATE, e.birth_date))::INT <= p_max_age)
          AND (p_min_salary IS NULL OR e.salary >= p_min_salary)
          AND (p_max_salary IS NULL OR e.salary <= p_max_salary)
          AND (p_cert_type IS NULL OR mc.employee_id IS NOT NULL)

        ORDER BY d.name, e.last_name, e.first_name;
END;
$$;

CREATE OR REPLACE FUNCTION get_project_team(
    p_project_id INT DEFAULT NULL,
    p_phase_id INT DEFAULT NULL,
    p_roles VARCHAR[] DEFAULT NULL,
    p_position_titles VARCHAR[] DEFAULT NULL,
    p_is_active BOOLEAN DEFAULT TRUE
)
    RETURNS TABLE
            (
                assignment_id   INT,
                employee_id     INT,
                project_id      INT,
                phase_id        INT,
                full_name       TEXT,
                position_title  VARCHAR(100),
                project_role    VARCHAR(100),
                project_name    VARCHAR(200),
                phase_name      VARCHAR(100),
                current_phase   VARCHAR(100),
                start_date      DATE,
                end_date        DATE,
                hours_allocated INT
            )
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN QUERY
        SELECT pa.assignment_id,
               pa.employee_id,
               pa.project_id,
               pa.phase_id,

               CONCAT_WS(' ', e.last_name, e.first_name, e.patronymic) AS full_name,
               pos.title                                               AS position_title,
               pa.role                                                 AS project_role,
               pr.name                                                 AS project_name,
               COALESCE(pp.name, 'Без фазы')                          AS phase_name,
               (
                   SELECT pp2.name
                   FROM ProjectPhase pp2
                   WHERE pp2.project_id = pr.project_id
                     AND pp2.start_date <= CURRENT_DATE
                     AND (pp2.end_date IS NULL OR pp2.end_date >= CURRENT_DATE)
                   ORDER BY pp2.order_number DESC
                   LIMIT 1
               )                                                       AS current_phase,
               pa.start_date,
               pa.end_date,
               pa.hours_allocated
        FROM ProjectAssignment pa
                 JOIN Employee e ON pa.employee_id = e.employee_id
                 JOIN Position pos ON e.position_id = pos.position_id
                 JOIN Project pr ON pa.project_id = pr.project_id
                 LEFT JOIN ProjectPhase pp ON pa.phase_id = pp.phase_id

        WHERE (p_is_active IS NULL OR e.is_active = p_is_active)

          AND (p_project_id IS NULL OR pa.project_id = p_project_id)
          AND (p_phase_id IS NULL OR pa.phase_id = p_phase_id)

          AND (p_roles IS NULL OR pa.role = ANY (p_roles))
          AND (p_position_titles IS NULL OR pos.title = ANY (p_position_titles))
        ORDER BY pa.role, e.last_name, e.first_name;
END;
$$;