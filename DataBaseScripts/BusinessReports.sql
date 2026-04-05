-- Requirement 8: Project profitability
CREATE OR REPLACE FUNCTION get_project_profitability(
    p_from DATE DEFAULT NULL,
    p_to DATE DEFAULT NULL
)
    RETURNS TABLE (
        project_id INT,
        project_name VARCHAR(200),
        total_costs DECIMAL(15, 2),
        budget DECIMAL(15, 2),
        efficiency_ratio DECIMAL(10, 2)
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            p.project_id,
            p.name,
            COALESCE(SUM(wl.hours_spent * e.salary / 160), 0) + 
            COALESCE(SUM(cr.cost_per_hour * wl.hours_spent), 0) AS total_costs,
            p.budget,
            CASE WHEN p.budget > 0 THEN (p.budget - (COALESCE(SUM(wl.hours_spent * e.salary / 160), 0) + COALESCE(SUM(cr.cost_per_hour * wl.hours_spent), 0))) / p.budget ELSE 0 END AS efficiency_ratio
        FROM Project p
                 LEFT JOIN WorkLog wl ON p.project_id = wl.project_id
                 LEFT JOIN Employee e ON wl.employee_id = e.employee_id
                 LEFT JOIN CloudResource cr ON p.project_id = cr.project_id
        WHERE (p_from IS NULL OR p.start_date >= p_from)
          AND (p_to IS NULL OR p.planned_end_date <= p_to)
        GROUP BY p.project_id, p.name, p.budget;
END;
$$;

-- Requirement 10: Clients
CREATE OR REPLACE FUNCTION get_client_activity(
    p_from DATE DEFAULT NULL,
    p_to DATE DEFAULT NULL
)
    RETURNS TABLE (
        client_id INT,
        client_name VARCHAR(200),
        active_projects BIGINT,
        total_spend DECIMAL(15, 2)
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            c.client_id,
            c.name,
            COUNT(p.project_id) FILTER (WHERE p.status = 'в_разработке') AS active_projects,
            SUM(p.budget) AS total_spend
        FROM Client c
                 LEFT JOIN Project p ON c.client_id = p.client_id
        WHERE (p_from IS NULL OR p.start_date >= p_from)
          AND (p_to IS NULL OR p.start_date <= p_to)
        GROUP BY c.client_id, c.name;
END;
$$;

-- Requirement 11: Releases
CREATE OR REPLACE FUNCTION get_release_report(
    p_from DATE DEFAULT NULL,
    p_to DATE DEFAULT NULL
)
    RETURNS TABLE (
        project_name VARCHAR(200),
        version VARCHAR(50),
        release_date DATE,
        changelog TEXT
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            p.name,
            r.version,
            r.release_date,
            r.changelog
        FROM Release r
                 JOIN Project p ON r.project_id = p.project_id
        WHERE (p_from IS NULL OR r.release_date >= p_from)
          AND (p_to IS NULL OR r.release_date <= p_to)
        ORDER BY r.release_date DESC;
END;
$$;

-- Requirement 12: Testing efficiency
CREATE OR REPLACE FUNCTION get_testing_efficiency(
    p_project_id INT
)
    RETURNS TABLE (
        bugs_in_test BIGINT,
        bugs_after_release BIGINT,
        efficiency_ratio DECIMAL(10, 2)
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            COUNT(*) FILTER (WHERE b.found_date IS NOT NULL AND b.fixed_date IS NULL) AS bugs_in_test,
            COUNT(*) FILTER (WHERE b.found_date IS NOT NULL AND b.fixed_date IS NOT NULL) AS bugs_after_release,
            CASE WHEN COUNT(*) FILTER (WHERE b.found_date IS NOT NULL AND b.fixed_date IS NOT NULL) > 0 
                THEN COUNT(*) FILTER (WHERE b.found_date IS NOT NULL AND b.fixed_date IS NULL)::DECIMAL / COUNT(*) FILTER (WHERE b.found_date IS NOT NULL AND b.fixed_date IS NOT NULL)
                ELSE 0 END AS efficiency_ratio
        FROM Bug b
        WHERE b.project_id = p_project_id;
END;
$$;
