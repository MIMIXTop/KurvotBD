create or replace function get_bugs_by_filter(
    p_project_id int DEFAULT NULL,
    p_bug_status varchar(50)[] default null,
    p_bug_severity varchar(50)[] default null,
    p_found_from date default null,
    p_found_to date default null
)
returns table (
    bug_id int,
    project_name varchar(200),
    title varchar(300),
    description text,
    status varchar(50),
    severity varchar(50),
    found_by integer,
    fixed_by integer,
    created_at TIMESTAMP,
    found_date date,
    fixed_date date,
    total_count bigint
) LANGUAGE plpgsql
AS $$
begin
    return query
    select
        b.bug_id,
        p.name as project_name,
        b.title,
        b.description,
        b.status,
        b.severity,
        b.found_by,
        b.fixed_by,
        b.created_at,
        b.found_date,
        b.fixed_date,
        COUNT(*) OVER() as total_count
    from bug b
    join project p on b.project_id = p.project_id
    where
        (p_project_id is null or b.project_id = p_project_id)
    and (p_bug_status is null or b.status = any(p_bug_status))
    and (p_bug_severity is null or b.severity = any(p_bug_severity))
    and (p_found_from is null or b.found_date >= p_found_from)
    and (p_found_to is null or b.found_date <= p_found_to);
end;
$$;

-- Requirement 12: Testing efficiency
CREATE OR REPLACE FUNCTION get_testing_efficiency(
    p_project_id INT DEFAULT NULL,
    p_from DATE DEFAULT NULL,
    p_to DATE DEFAULT NULL
)
    RETURNS TABLE (
        project_name VARCHAR(200),
        bugs_in_test BIGINT,
        bugs_after_release BIGINT,
        efficiency_ratio DECIMAL(10, 2)
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            p.name,
            COUNT(*) FILTER (WHERE b.status IN ('в_работе', 'новая')) AS bugs_in_test,
            COUNT(*) FILTER (WHERE b.status = 'исправлена') AS bugs_after_release,
            CASE WHEN COUNT(*) FILTER (WHERE b.status = 'исправлена') > 0 
                THEN COUNT(*) FILTER (WHERE b.status IN ('в_работе', 'новая'))::DECIMAL / COUNT(*) FILTER (WHERE b.status = 'исправлена')
                ELSE 0 END AS efficiency_ratio
        FROM Bug b
        JOIN Project p ON b.project_id = p.project_id
        WHERE (p_project_id IS NULL OR b.project_id = p_project_id)
          AND (p_from IS NULL OR b.found_date >= p_from)
          AND (p_to IS NULL OR b.found_date <= p_to)
        GROUP BY p.name;
END;
$$;