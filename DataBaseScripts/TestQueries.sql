-- ============================================
-- ТЕСТОВЫЕ ЗАПРОСЫ ДЛЯ ПРОВЕРКИ СКРИПТОВ
-- ============================================

-- ============================================
-- 1. MethodologyReport.sql
-- ============================================

-- 1.1. Проекты по методологии Scrum (активные, по умолчанию)
SELECT * FROM get_projects_by_methodology('Scrum');

-- 1.2. Проекты по методологии Kanban
SELECT * FROM get_projects_by_methodology('Kanban');

-- 1.3. Проекты по методологии Waterfall (активные)
SELECT * FROM get_projects_by_methodology('Waterfall', TRUE);

-- 1.4. Проекты по методологии Waterfall (неактивные)
SELECT * FROM get_projects_by_methodology('Waterfall', FALSE);

-- 1.5. Все проекты по методологии Scrum (активные + неактивные)
SELECT * FROM get_projects_by_methodology('Scrum', NULL);

-- 1.6. Несуществующая методология (должно вернуть пустой результат)
SELECT * FROM get_projects_by_methodology('XP');


-- ============================================
-- 2. SpecializationReport.sql
-- ============================================

-- 2.1. Все сертификаты
SELECT * FROM get_employees_with_certifications();

-- 2.2. Сертификаты PMP
SELECT * FROM get_employees_with_certifications('PMP');

-- 2.3. Сертификаты Scrum
SELECT * FROM get_employees_with_certifications('Scrum');

-- 2.4. Несуществующий тип сертификата
SELECT * FROM get_employees_with_certifications('AWS');


-- ============================================
-- 3. InfrastructureReport.sql
-- ============================================

-- 3.1. Отчёт по инфраструктуре (активные проекты, по умолчанию)
SELECT * FROM get_infrastructure_report();

-- 3.2. Отчёт по инфраструктуре (неактивные проекты)
SELECT * FROM get_infrastructure_report(FALSE);

-- 3.3. Отчёт по инфраструктуре (все проекты)
SELECT * FROM get_infrastructure_report(NULL);


-- ============================================
-- 4. BusinessReports.sql
-- ============================================

-- 4.1. Рентабельность всех проектов
SELECT * FROM get_project_profitability();

-- 4.2. Рентабельность проектов за 2024 год
SELECT * FROM get_project_profitability('2024-01-01', '2024-12-31');

-- 4.3. Активность всех клиентов
SELECT * FROM get_client_activity();

-- 4.4. Активность клиентов за период
SELECT * FROM get_client_activity('2024-01-01', '2024-06-30');

-- 4.5. Все релизы
SELECT * FROM get_release_report();

-- 4.6. Релизы за 2024 год
SELECT * FROM get_release_report('2024-01-01', '2024-12-31');

-- 4.7. Эффективность тестирования проекта 1
SELECT * FROM get_testing_efficiency(1);

-- 4.8. Эффективность тестирования проекта 3
SELECT * FROM get_testing_efficiency(3);


-- ============================================
-- 5. TechnologyReport.sql
-- ============================================

-- 5.1. Использование технологий (активные проекты, по умолчанию)
SELECT * FROM get_project_technologies();

-- 5.2. Использование технологий (неактивные проекты)
SELECT * FROM get_project_technologies(FALSE);

-- 5.3. Использование технологий (все проекты)
SELECT * FROM get_project_technologies(NULL);


-- ============================================
-- 6. Workload.sql
-- ============================================

-- 6.1. Нагрузка всех сотрудников
SELECT * FROM get_employee_workload();

-- 6.2. Нагрузка конкретного сотрудника (employee_id = 6)
SELECT * FROM get_employee_workload(ARRAY[6]);

-- 6.3. Нагрузка нескольких сотрудников
SELECT * FROM get_employee_workload(ARRAY[6, 7, 8]);

-- 6.4. Нагрузка по проекту 1
SELECT * FROM get_employee_workload(NULL, ARRAY[1]);

-- 6.5. Нагрузка за период
SELECT * FROM get_employee_workload(NULL, NULL, '2024-01-01', '2024-03-31');

-- 6.6. Нагрузка сотрудника 6 по проекту 1
SELECT * FROM get_employee_workload(ARRAY[6], ARRAY[1]);


-- ============================================
-- 7. BugFilter.sql
-- ============================================

-- 7.1. Все баги без фильтрации
SELECT * FROM get_bugs_by_filter();

-- 7.2. Баги проекта 1
SELECT * FROM get_bugs_by_filter(1);

-- 7.3. Баги со статусом "исправлена"
SELECT * FROM get_bugs_by_filter(NULL, ARRAY['исправлена']);

-- 7.4. Баги со статусом "в_работе"
SELECT * FROM get_bugs_by_filter(NULL, ARRAY['в_работе']);

-- 7.5. Баги с severity "критическая"
SELECT * FROM get_bugs_by_filter(NULL, NULL, ARRAY['критическая']);

-- 7.6. Баги с severity "блокирующая"
SELECT * FROM get_bugs_by_filter(NULL, NULL, ARRAY['блокирующая']);

-- 7.7. Комбинированный фильтр: проект 1, статус "исправлена"
SELECT * FROM get_bugs_by_filter(1, ARRAY['исправлена']);

-- 7.8. Комбинированный фильтр: критические баги в работе
SELECT * FROM get_bugs_by_filter(NULL, ARRAY['в_работе'], ARRAY['критическая']);

-- 7.9. Несколько статусов
SELECT * FROM get_bugs_by_filter(p_bug_status :=  ARRAY['исправлена', 'в_работе']);


-- ============================================
-- 8. EmployeeFilter.sql
-- ============================================

-- 8.1. Все активные сотрудники (по умолчанию)
SELECT * FROM get_employee_by_filters();

-- 8.2. Все сотрудники (активные + неактивные)
SELECT * FROM get_employee_by_filters(p_is_active := NULL);

-- 8.3. Сотрудники отдела 1 (разработка)
SELECT * FROM get_employee_by_filters(ARRAY[1]);

-- 8.4. Сотрудники отделов 1 и 3
SELECT * FROM get_employee_by_filters(ARRAY[1, 3]);

-- 8.5. Сотрудники с зарплатой от 150000
SELECT * FROM get_employee_by_filters(NULL, NULL, NULL, NULL, NULL, NULL, 150000);

-- 8.6. Сотрудники с зарплатой от 100000 до 200000
SELECT * FROM get_employee_by_filters(NULL, NULL, NULL, NULL, NULL, NULL, 100000, 200000);

-- 8.7. Сотрудники по должности "Программист"
SELECT * FROM get_employee_by_filters(NULL, ARRAY['Программист']);

-- 8.8. Сотрудники с опытом от 5 лет
SELECT * FROM get_employee_by_filters(NULL, NULL, 5);

-- 8.9. Комбинированный фильтр: отдел 1, зарплата > 150000
SELECT * FROM get_employee_by_filters(ARRAY[1], NULL, NULL, NULL, NULL, NULL, 150000);

-- 8.10. Команда проекта 1 (активные сотрудники, по умолчанию)
SELECT * FROM get_project_team(1);

-- 8.11. Команда проекта 1, все сотрудники (активные + неактивные)
SELECT * FROM get_project_team(1, p_is_active := NULL);

-- 8.12. Команда проекта 1, только разработчики
SELECT * FROM get_project_team(1, NULL, ARRAY['разработчик']);

-- 8.13. Команда проекта 1, тестировщики
SELECT * FROM get_project_team(1, NULL, ARRAY['тестировщик']);

-- 8.14. Все менеджеры проектов
SELECT * FROM get_project_team(NULL, NULL, ARRAY['менеджер_проекта']);


-- ============================================
-- 9. ProjecFilter.sql
-- ============================================

-- 9.1. Все активные проекты (по умолчанию)
SELECT * FROM get_projects_by_filters();

-- 9.2. Все проекты (активные + неактивные)
SELECT * FROM get_projects_by_filters(p_is_active := NULL);

-- 9.3. Только неактивные проекты
SELECT * FROM get_projects_by_filters(p_is_active := FALSE);

-- 9.4. Проекты в статусе "в_разработке"
SELECT * FROM get_projects_by_filters(ARRAY['в_разработке']);

-- 9.5. Завершённые проекты (неактивные)
SELECT * FROM get_projects_by_filters(ARRAY['завершен'], p_is_active := FALSE);

-- 9.6. Проекты клиента 1 (ООО "ТехноКорп")
SELECT * FROM get_projects_by_filters(NULL, NULL, NULL, 1);

-- 9.7. Несколько статусов, все проекты
SELECT * FROM get_projects_by_filters(ARRAY['в_разработке', 'завершен'], p_is_active := NULL);

-- 9.8. Проекты по типу клиента (активные, по умолчанию)
SELECT * FROM get_projects_by_client_type(ARRAY['корпоративный']);

-- 9.9. Проекты по типу клиента (все)
SELECT * FROM get_projects_by_client_type(ARRAY['корпоративный'], p_is_active := NULL);

-- 9.10. Проекты государственных клиентов
SELECT * FROM get_projects_by_client_type(ARRAY['государственный']);

-- 9.11. Проекты с бюджетом от 5000000
SELECT * FROM get_projects_by_client_type(NULL, 5000000);

-- 9.12. Проекты с бюджетом от 2000000 до 6000000
SELECT * FROM get_projects_by_client_type(NULL, 2000000, 6000000);

-- 9.13. Комбинированный фильтр: корпоративные клиенты, бюджет > 4000000
SELECT * FROM get_projects_by_client_type(ARRAY['корпоративный'], 4000000);

-- 9.14. Все проекты с бюджетом > 3000000 (активные + неактивные)
SELECT * FROM get_projects_by_client_type(NULL, 3000000, p_is_active := NULL);
