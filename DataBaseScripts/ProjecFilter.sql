create or replace function get_projects_by_filters(
    p_statuses varchar(50)[] DEFAULT NULL,
    p_completed_from DATE DEFAULT NULL,
    p_completed_to DATE DEFAULT NULL,
    p_client_id INT DEFAULT NULL,
    p_is_active BOOLEAN DEFAULT TRUE
)
    RETURNS TABLE (
        project_id int,
        client_id int,
        project_name varchar(200),
        client_name varchar(200),
        status varchar(30),
        type varchar(100),
        methodology varchar(50),
        start_date DATE,
        planned_and_date DATE,
        actual_end_date DATE,
        budget DECIMAL(15,2)
    )
    LANGUAGE plpgsql AS $$
    begin
        RETURN QUERY
        select
            p.project_id,
            p.client_id,
            p.name as project_name,
            c.name as client_name,
            p.status,
            p.type,
            p.methodology,
            p.start_date,
            p.planned_end_date,
            p.actual_end_date,
            p.budget
        from project p
        join client c ON p.client_id = c.client_id
        WHERE (p_is_active IS NULL OR p.is_active = p_is_active)

            AND (p_client_id IS NULL OR p.client_id = p_client_id)
            AND (p_statuses IS NULL OR p.status = ANY(p_statuses))
            AND (p_completed_from IS NULL OR (p.actual_end_date IS NOT NULL AND p.actual_end_date >= p_completed_from))
            AND (p_completed_to IS NULL OR (p.actual_end_date IS NOT NULL AND p.actual_end_date <= p_completed_to))
        ORDER BY p.status, p.start_date DESC;
    end;
    $$;

CREATE OR REPLACE FUNCTION get_projects_by_client_type(
    p_client_types VARCHAR[] DEFAULT NULL,
    p_min_budget DECIMAL(15, 2) DEFAULT NULL,
    p_max_budget DECIMAL(15, 2) DEFAULT NULL,
    p_is_active BOOLEAN DEFAULT TRUE
)
    RETURNS TABLE (
                      total_count BIGINT,

                      project_id INT,
                      client_id INT,

                      project_name VARCHAR(200),
                      client_name VARCHAR(200),
                      client_type VARCHAR(50),
                      budget DECIMAL(15, 2),
                      start_date DATE,
                      planned_end_date DATE,
                      actual_end_date DATE,
                      status VARCHAR(50)
                  )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            COUNT(*) OVER() AS total_count,

            p.project_id,
            p.client_id,

            p.name AS project_name,
            c.name AS client_name,
            c.type AS client_type,
            p.budget,
            p.start_date,
            p.planned_end_date,
            p.actual_end_date,
            p.status
        FROM Project p
                 JOIN Client c ON p.client_id = c.client_id
        WHERE (p_is_active IS NULL OR p.is_active = p_is_active)

          AND (p_client_types IS NULL OR c.type = ANY(p_client_types))

          AND (p_min_budget IS NULL OR p.budget >= p_min_budget)
          AND (p_max_budget IS NULL OR p.budget <= p_max_budget)
        ORDER BY c.type, p.budget DESC;
END;
$$;