CREATE OR REPLACE FUNCTION get_monthly_financial_report(
    p_month INT,
    p_year INT
)
    RETURNS TABLE
            (
                project_name       VARCHAR(200),
                expenses_salary    DECIMAL(15, 2),
                expenses_licenses  DECIMAL(15, 2),
                expenses_cloud     DECIMAL(15, 2),
                total_monthly_cost DECIMAL(15, 2)
            )
    LANGUAGE plpgsql
AS
$$
BEGIN
    RETURN QUERY
        SELECT p.name                                                                                              AS project_name,

               -- Расходы на ЗП: (Зарплата сотрудника / 160 часов) * количество потраченных часов в этом месяце
               COALESCE((SELECT SUM(wl.hours_spent * (e.salary / 160.0))
                         FROM WorkLog wl
                                  JOIN Employee e ON wl.employee_id = e.employee_id
                         WHERE wl.project_id = p.project_id
                           AND EXTRACT(MONTH FROM wl.work_date) = p_month
                           AND EXTRACT(YEAR FROM wl.work_date) = p_year),
                        0.00)                                                                                      AS expenses_salary,

               -- Расходы на лицензии (учитываем allocated_seats и делим годовую стоимость на 12 месяцев)
               COALESCE((SELECT SUM((sl.cost / 12.0) * la.allocated_seats)
                         FROM LicenseAllocation la
                                  JOIN SoftwareLicense sl ON la.license_id = sl.license_id
                         WHERE la.project_id = p.project_id
                           AND la.allocation_date <= MAKE_DATE(p_year, p_month, 28)
                           AND (sl.expiry_date IS NULL OR sl.expiry_date >= MAKE_DATE(p_year, p_month, 1))),
                        0.00)                                                                                      AS expenses_licenses,

               -- Расходы на облачные ресурсы (стоимость в час * 730 часов в месяце)
               COALESCE((SELECT SUM(cr.cost_per_hour * 730)
                         FROM CloudResource cr
                         WHERE cr.project_id = p.project_id),
                        0.00)                                                                                      AS expenses_cloud,

               -- Итоговые расходы проекта за месяц
               (
                   COALESCE((SELECT SUM(wl.hours_spent * (e.salary / 160.0))
                             FROM WorkLog wl
                                      JOIN Employee e ON wl.employee_id = e.employee_id
                             WHERE wl.project_id = p.project_id
                               AND EXTRACT(MONTH FROM wl.work_date) = p_month
                               AND EXTRACT(YEAR FROM wl.work_date) = p_year), 0) +
                   COALESCE((SELECT SUM((sl.cost / 12.0) * la.allocated_seats)
                             FROM LicenseAllocation la
                                      JOIN SoftwareLicense sl ON la.license_id = sl.license_id
                             WHERE la.project_id = p.project_id
                               AND la.allocation_date <= MAKE_DATE(p_year, p_month, 28)
                               AND (sl.expiry_date IS NULL OR sl.expiry_date >= MAKE_DATE(p_year, p_month, 1))), 0) +
                   COALESCE((SELECT SUM(cr.cost_per_hour * 730)
                             FROM CloudResource cr
                             WHERE cr.project_id = p.project_id), 0)
                   )                                                                                               AS total_monthly_cost

        FROM Project p
        WHERE p.is_active = TRUE;
END;
$$;