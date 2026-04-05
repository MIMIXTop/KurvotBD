-- Requirement 5: Employee workload
CREATE OR REPLACE FUNCTION get_employee_workload(
    p_employee_ids INT[] DEFAULT NULL,
    p_project_ids INT[] DEFAULT NULL,
    p_from DATE DEFAULT NULL,
    p_to DATE DEFAULT NULL
)
    RETURNS TABLE (
        employee_id INT,
        full_name TEXT,
        project_id INT,
        project_name VARCHAR(200),
        total_hours DECIMAL(10, 2),
        task_count BIGINT
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            e.employee_id,
            CONCAT_WS(' ', e.last_name, e.first_name, e.patronymic) AS full_name,
            wl.project_id,
            p.name AS project_name,
            SUM(wl.hours_spent) AS total_hours,
            COUNT(wl.log_id) AS task_count
        FROM WorkLog wl
                 JOIN Employee e ON wl.employee_id = e.employee_id
                 JOIN Project p ON wl.project_id = p.project_id
        WHERE (p_employee_ids IS NULL OR wl.employee_id = ANY(p_employee_ids))
          AND (p_project_ids IS NULL OR wl.project_id = ANY(p_project_ids))
          AND (p_from IS NULL OR wl.work_date >= p_from)
          AND (p_to IS NULL OR wl.work_date <= p_to)
        GROUP BY e.employee_id, e.last_name, e.first_name, e.patronymic, wl.project_id, p.name
        ORDER BY full_name, total_hours DESC;
END;
$$;
