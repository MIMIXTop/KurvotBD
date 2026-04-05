-- Requirement 6: Technology usage report
CREATE OR REPLACE FUNCTION get_project_technologies(
    p_is_active BOOLEAN DEFAULT TRUE
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
        WHERE p_is_active IS NULL OR p.is_active = p_is_active
        GROUP BY tech.t
        ORDER BY project_count DESC;
END;
$$;
