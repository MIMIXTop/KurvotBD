CREATE OR REPLACE FUNCTION get_project_status_report(
    p_project_id INT,
    p_start_date DATE,
    p_end_date DATE
)
    RETURNS TABLE
            (
                project_name         VARCHAR(200),
                current_phase        VARCHAR(100),
                tasks_logged         BIGINT,
                team_hours_spent     DECIMAL(10, 2),
                critical_risks_count BIGINT,
                report_period        TEXT
            )
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN QUERY
        SELECT p.name                                                                   AS project_name,
               COALESCE((SELECT pp.name
                         FROM ProjectPhase pp
                         WHERE pp.project_id = p.project_id
                           AND pp.start_date <= p_end_date
                         ORDER BY pp.start_date DESC
                         LIMIT 1), 'Этап не определен')::VARCHAR                        AS current_phase,

               (SELECT COUNT(*)
                FROM WorkLog wl
                WHERE wl.project_id = p.project_id
                  AND wl.work_date BETWEEN p_start_date AND p_end_date)                 AS tasks_logged,

               -- Потраченные часы команды
               COALESCE((SELECT SUM(wl.hours_spent)
                         FROM WorkLog wl
                         WHERE wl.project_id = p.project_id
                           AND wl.work_date BETWEEN p_start_date AND p_end_date), 0.00) AS team_hours_spent,

               (SELECT COUNT(*)
                FROM Bug b
                WHERE b.project_id = p.project_id
                  AND b.status IN ('новая', 'в_работе')
                  AND b.severity IN ('блокирующая', 'критическая'))                     AS critical_risks_count,

               (p_start_date::TEXT || ' - ' || p_end_date::TEXT)                        AS report_period
        FROM Project p
        WHERE p.project_id = p_project_id;
END;
$$;