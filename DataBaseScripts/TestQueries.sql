-- ============================================
-- ТЕСТОВЫЕ ЗАПРОСЫ ДЛЯ ПРОВЕРКИ СКРИПТОВ
-- ============================================

-- ============================================
-- 1. ProjecFilter.sql (объединённые функции по таблице Project)
-- ============================================

-- 1.1. Все активные проекты (по умолчанию)
SELECT * FROM get_projects_by_filters();


-- 1.2. Все проекты (активные + неактивные)
SELECT * FROM get_projects_by_filters(p_is_active := NULL);

-- 1.3. Только неактивные проекты
SELECT * FROM get_projects_by_filters(p_is_active := FALSE);

-- 1.4. Проекты в статусе "в_разработке"
SELECT * FROM get_projects_by_filters(ARRAY['в_разработке']);

-- 1.5. Завершённые проекты (неактивные)
SELECT * FROM get_projects_by_filters(ARRAY['завершен'], p_is_active := FALSE);

-- 1.6. Проекты клиента 1 (ООО "ТехноКорп")
SELECT * FROM get_projects_by_filters(p_client_id := 1);

-- 1.7. Несколько статусов, все проекты
SELECT * FROM get_projects_by_filters(ARRAY['в_разработке', 'завершен'], p_is_active := NULL);

-- 1.8. Проекты по методологии Scrum (активные)
SELECT * FROM get_projects_by_filters(p_methodology_filter := 'Scrum');

-- 1.9. Проекты по методологии Kanban
SELECT * FROM get_projects_by_filters(p_methodology_filter := 'Kanban');

-- 1.10. Проекты по методологии Waterfall (неактивные)
SELECT * FROM get_projects_by_filters(p_methodology_filter := 'Waterfall', p_is_active := FALSE);

-- 1.11. Все проекты по методологии Scrum (активные + неактивные)
SELECT * FROM get_projects_by_filters(p_methodology_filter := 'Scrum', p_is_active := NULL);

-- 1.12. Проекты корпоративных клиентов
SELECT * FROM get_projects_by_filters(p_client_types := ARRAY['корпоративный']);

-- 1.13. Проекты государственных клиентов
SELECT * FROM get_projects_by_filters(p_client_types := ARRAY['государственный']);

-- 1.14. Проекты с бюджетом от 5000000
SELECT * FROM get_projects_by_filters(p_min_budget := 5000000);

-- 1.15. Проекты с бюджетом от 2000000 до 6000000
SELECT * FROM get_projects_by_filters(p_min_budget := 2000000, p_max_budget := 6000000);

-- 1.16. Комбинированный фильтр: корпоративные клиенты, бюджет > 4000000
SELECT * FROM get_projects_by_filters(p_client_types := ARRAY['корпоративный'], p_min_budget := 4000000);

-- 1.17. Комбинированный фильтр: методология Scrum, активные проекты
SELECT * FROM get_projects_by_filters(p_methodology_filter := 'Scrum', p_is_active := TRUE);

-- 1.18. Комбинированный фильтр: статус "в_разработке", бюджет от 3000000
SELECT * FROM get_projects_by_filters(ARRAY['в_разработке'], p_min_budget := 3000000);

-- 1.19. Рентабельность всех проектов
SELECT * FROM get_project_profitability();

-- 1.20. Рентабельность проектов за 2024 год
SELECT * FROM get_project_profitability('2024-01-01', '2024-12-31');

-- 1.21. Инфраструктура (активные проекты)
SELECT * FROM get_infrastructure_report();

-- 1.22. Инфраструктура (неактивные проекты)
SELECT * FROM get_infrastructure_report(FALSE);

-- 1.23. Инфраструктура конкретного проекта
SELECT * FROM get_infrastructure_report(p_project_id := 1);

-- 1.24. Использование технологий (активные проекты)
SELECT * FROM get_project_technologies();

-- 1.25. Использование технологий (неактивные проекты)
SELECT * FROM get_project_technologies(FALSE);

-- 1.26. Использование конкретной технологии
SELECT * FROM get_project_technologies(p_tech_name := 'Python');


-- ============================================
-- 2. BugFilter.sql (объединённые функции по таблице Bug)
-- ============================================

-- 2.1. Все баги без фильтрации
SELECT * FROM get_bugs_by_filter();

-- 2.2. Баги проекта 1
SELECT * FROM get_bugs_by_filter(1);

-- 2.3. Баги со статусом "исправлена"
SELECT * FROM get_bugs_by_filter(p_bug_status := ARRAY['исправлена']);

-- 2.4. Баги со статусом "в_работе"
SELECT * FROM get_bugs_by_filter(p_bug_status := ARRAY['в_работе']);

-- 2.5. Баги с severity "критическая"
SELECT * FROM get_bugs_by_filter(p_bug_severity := ARRAY['критическая']);

-- 2.6. Баги с severity "блокирующая"
SELECT * FROM get_bugs_by_filter(p_bug_severity := ARRAY['блокирующая']);

-- 2.7. Комбинированный фильтр: проект 1, статус "исправлена"
SELECT * FROM get_bugs_by_filter(1, ARRAY['исправлена']);

-- 2.8. Комбинированный фильтр: критические баги в работе
SELECT * FROM get_bugs_by_filter(NULL, ARRAY['в_работе'], ARRAY['критическая']);

-- 2.9. Несколько статусов
SELECT * FROM get_bugs_by_filter(p_bug_status := ARRAY['исправлена', 'в_работе']);

-- 2.10. Баги за период
SELECT * FROM get_bugs_by_filter(p_found_from := '2024-01-01', p_found_to := '2024-12-31');

-- 2.11. Эффективность тестирования проекта 1
SELECT * FROM get_testing_efficiency(1);

-- 2.12. Эффективность тестирования всех проектов
SELECT * FROM get_testing_efficiency();

-- 2.13. Эффективность тестирования за период
SELECT * FROM get_testing_efficiency(p_from := '2024-01-01', p_to := '2024-12-31');


-- ============================================
-- 3. EmployeeFilter.sql (объединённые функции по таблице Employee)
-- ============================================

-- 3.1. Все активные сотрудники (по умолчанию)
SELECT * FROM get_employee_by_filters();

-- 3.2. Все сотрудники (активные + неактивные)
SELECT * FROM get_employee_by_filters(p_is_active := NULL);

-- 3.3. Сотрудники отдела 1 (разработка)
SELECT * FROM get_employee_by_filters(ARRAY[1]);

-- 3.4. Сотрудники отделов 1 и 3
SELECT * FROM get_employee_by_filters(ARRAY[1, 3]);

-- 3.5. Сотрудники с зарплатой от 150000
SELECT * FROM get_employee_by_filters(p_min_salary := 150000);

-- 3.6. Сотрудники с зарплатой от 100000 до 200000
SELECT * FROM get_employee_by_filters(p_min_salary := 100000, p_max_salary := 200000);

-- 3.7. Сотрудники по должности "Программист"
SELECT * FROM get_employee_by_filters(p_position_ids := ARRAY[3]);

-- 3.8. Сотрудники с опытом от 5 лет
SELECT * FROM get_employee_by_filters(p_min_experience := 5);

-- 3.9. Комбинированный фильтр: отдел 1, зарплата > 150000
SELECT * FROM get_employee_by_filters(ARRAY[1], p_min_salary := 150000);

-- 3.10. Сотрудники с сертификатом PMP
SELECT * FROM get_employee_by_filters(p_cert_type := 'PMP');

-- 3.11. Сотрудники с сертификатом Scrum
SELECT * FROM get_employee_by_filters(p_cert_type := 'Scrum');

-- 3.12. Команда проекта 1 (активные сотрудники, по умолчанию)
SELECT * FROM get_project_team(1);

-- 3.13. Команда проекта 1, все сотрудники (активные + неактивные)
SELECT * FROM get_project_team(1, p_is_active := NULL);

-- 3.14. Команда проекта 1, только разработчики
SELECT * FROM get_project_team(1, p_roles := ARRAY['разработчик']);

-- 3.15. Команда проекта 1, тестировщики
SELECT * FROM get_project_team(1, p_roles := ARRAY['тестировщик']);

-- 3.16. Все менеджеры проектов
SELECT * FROM get_project_team(p_roles := ARRAY['менеджер_проекта']);


-- ============================================
-- 4. BusinessReports.sql (функции по таблице Client и Release)
-- ============================================

-- 4.1. Активность всех клиентов
SELECT * FROM get_client_activity();

-- 4.2. Активность клиентов за период
SELECT * FROM get_client_activity('2024-01-01', '2024-06-30');

-- 4.3. Все релизы
SELECT * FROM get_release_report();

-- 4.4. Релизы за 2024 год
SELECT * FROM get_release_report('2024-01-01', '2024-12-31');


-- ============================================
-- 5. Workload.sql (функция по таблице WorkLog)
-- ============================================

-- 5.1. Нагрузка всех сотрудников
SELECT * FROM get_employee_workload();

-- 5.2. Нагрузка конкретного сотрудника (employee_id = 6)
SELECT * FROM get_employee_workload(ARRAY[6]);

-- 5.3. Нагрузка нескольких сотрудников
SELECT * FROM get_employee_workload(ARRAY[6, 7, 8]);

-- 5.4. Нагрузка по проекту 1
SELECT * FROM get_employee_workload(p_project_ids := ARRAY[1]);

SELECT * FROM get_employee_workload(p_employee_ids := ARRAY[2]);

-- 5.5. Нагрузка за период
SELECT * FROM get_employee_workload(p_from := '2024-01-01', p_to := '2024-03-31');

-- 5.6. Нагрузка сотрудника 6 по проекту 1
SELECT * FROM get_employee_workload(ARRAY[6], ARRAY[1]);
