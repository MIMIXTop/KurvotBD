-- Requirement 14: Project by methodology
CREATE OR REPLACE FUNCTION get_projects_by_methodology(
    p_methodology VARCHAR(50),
    p_is_active BOOLEAN DEFAULT TRUE
)
    RETURNS TABLE (
        project_name VARCHAR(200),
        budget DECIMAL(15, 2),
        actual_end_date DATE,
        planned_end_date DATE
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            p.name,
            p.budget,
            p.actual_end_date,
            p.planned_end_date
        FROM Project p
        WHERE p.methodology = p_methodology
          AND (p_is_active IS NULL OR p.is_active = p_is_active);
END;
$$;
