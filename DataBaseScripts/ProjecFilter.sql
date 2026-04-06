create or replace function get_projects_by_filters(
    p_statuses varchar(50)[] DEFAULT NULL,
    p_completed_from DATE DEFAULT NULL,
    p_completed_to DATE DEFAULT NULL,
    p_client_id INT DEFAULT NULL,
    p_methodology_filter VARCHAR(50) DEFAULT NULL,
    p_client_types VARCHAR[] DEFAULT NULL,
    p_min_budget DECIMAL(15, 2) DEFAULT NULL,
    p_max_budget DECIMAL(15, 2) DEFAULT NULL,
    p_is_active BOOLEAN DEFAULT TRUE
)
    RETURNS TABLE (
        project_id int,
        client_id int,
        project_name varchar(200),
        client_name varchar(200),
        client_type varchar(50),
        status varchar(30),
        type varchar(100),
        methodology varchar(50),
        start_date DATE,
        planned_end_date DATE,
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
            c.type as client_type,
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
            AND (p_methodology_filter IS NULL OR p.methodology = p_methodology_filter)
            AND (p_client_types IS NULL OR c.type = ANY(p_client_types))
            AND (p_min_budget IS NULL OR p.budget >= p_min_budget)
            AND (p_max_budget IS NULL OR p.budget <= p_max_budget)
        ORDER BY p.status, p.start_date DESC;
    end;
    $$;

-- Requirement 8: Project profitability
CREATE OR REPLACE FUNCTION get_project_profitability(
    p_from DATE DEFAULT NULL,
    p_to DATE DEFAULT NULL,
    p_is_active BOOLEAN DEFAULT NULL
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
          AND (p_is_active IS NULL OR p.is_active = p_is_active)
        GROUP BY p.project_id, p.name, p.budget;
END;
$$;

-- Requirement 9: License and Cloud resource report
CREATE OR REPLACE FUNCTION get_infrastructure_report(
    p_is_active BOOLEAN DEFAULT TRUE,
    p_project_id INT DEFAULT NULL
)
    RETURNS TABLE (
        project_id INT,
        project_name VARCHAR(200),
        license_name VARCHAR(200),
        expiry_date DATE,
        cost DECIMAL(12, 2),
        resource_provider VARCHAR(50),
        resource_cost_per_hour DECIMAL(10, 4)
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            p.project_id,
            p.name AS project_name,
            sl.name AS license_name,
            sl.expiry_date,
            sl.cost,
            cr.provider AS resource_provider,
            cr.cost_per_hour
        FROM Project p
                 LEFT JOIN LicenseAllocation la ON p.project_id = la.project_id
                 LEFT JOIN SoftwareLicense sl ON la.license_id = sl.license_id
                 LEFT JOIN CloudResource cr ON p.project_id = cr.project_id
        WHERE (p_is_active IS NULL OR p.is_active = p_is_active)
          AND (p_project_id IS NULL OR p.project_id = p_project_id);
END;
$$;

-- Requirement 6: Technologies and tools usage
CREATE OR REPLACE FUNCTION get_project_technologies(
    p_is_active BOOLEAN DEFAULT TRUE,
    p_tech_name TEXT DEFAULT NULL
)
    RETURNS TABLE (
        tech_name TEXT,
        project_count BIGINT
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            tech.t AS tech_name,
            COUNT(*) AS project_count
        FROM Project p,
             unnest(p.tech_stack) AS tech(t)
        WHERE (p_is_active IS NULL OR p.is_active = p_is_active)
          AND (p_tech_name IS NULL OR tech.t ILIKE '%' || p_tech_name || '%')
        GROUP BY tech.t
        ORDER BY project_count DESC;
END;
$$;