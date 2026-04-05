-- Requirement 13: Certificates report
CREATE OR REPLACE FUNCTION get_employees_with_certifications(
    p_cert_type VARCHAR DEFAULT NULL
)
    RETURNS TABLE (
        employee_id INT,
        full_name TEXT,
        cert_type VARCHAR(100),
        issue_date DATE
    )
    LANGUAGE plpgsql
AS $$
BEGIN
    RETURN QUERY
        SELECT
            e.employee_id,
            CONCAT_WS(' ', e.last_name, e.first_name, e.patronymic),
            mc.cert_type,
            mc.issue_date
        FROM Employee e
                 JOIN ManagerCertification mc ON e.employee_id = mc.employee_id
        WHERE (p_cert_type IS NULL OR mc.cert_type ILIKE '%' || p_cert_type || '%');
END;
$$;
