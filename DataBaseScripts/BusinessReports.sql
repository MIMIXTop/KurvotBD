-- Requirement 10: Clients
CREATE OR REPLACE FUNCTION get_client_activity(
    p_from DATE DEFAULT NULL,
    p_to DATE DEFAULT NULL
)
    RETURNS TABLE (
        client_id INT,
        client_name VARCHAR(200),
        active_projects BIGINT,
        total_spend DECIMAL(15, 2),
        earliest_project_date DATE,
        latest_project_date DATE
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            c.client_id,
            c.name,
            COUNT(p.project_id) FILTER (WHERE p.status = 'в_разработке') AS active_projects,
            SUM(p.budget) AS total_spend,
            MIN(p.start_date) AS earliest_project_date,
            MAX(p.start_date) AS latest_project_date
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
