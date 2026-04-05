-- Requirement 9: License and Cloud resource report
CREATE OR REPLACE FUNCTION get_infrastructure_report(
    p_is_active BOOLEAN DEFAULT TRUE
)
    RETURNS TABLE (
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
        WHERE p_is_active IS NULL OR p.is_active = p_is_active;
END;
$$;
