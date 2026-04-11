-- ============================================
-- Очистка всех таблиц от данных
-- ============================================
-- Внимание: порядок важен из-за foreign key!

-- Очищаем связанные таблицы
TRUNCATE TABLE LicenseAllocation CASCADE;
TRUNCATE TABLE WorkLog CASCADE;
TRUNCATE TABLE Bug CASCADE;
TRUNCATE TABLE Release CASCADE;
TRUNCATE TABLE Documentation CASCADE;
TRUNCATE TABLE ProjectSpecification CASCADE;
TRUNCATE TABLE ProjectAssignment CASCADE;
TRUNCATE TABLE ProjectPhase CASCADE;
TRUNCATE TABLE ManagerCertification CASCADE;
TRUNCATE TABLE TesterSpecialization CASCADE;
TRUNCATE TABLE DeveloperSpecialization CASCADE;
TRUNCATE TABLE CloudResource CASCADE;

-- Очищаем основные справочники и сущности
TRUNCATE TABLE Employee CASCADE;
TRUNCATE TABLE Project CASCADE;
TRUNCATE TABLE Client CASCADE;
TRUNCATE TABLE SoftwareLicense CASCADE;
TRUNCATE TABLE Position CASCADE;
TRUNCATE TABLE Department CASCADE;

-- Сбрасываем последовательности (автоинкремент) - устанавливаем в 1
SELECT setval('department_department_id_seq', 1, false);
SELECT setval('position_position_id_seq', 1, false);
SELECT setval('employee_employee_id_seq', 1, false);
SELECT setval('client_client_id_seq', 1, false);
SELECT setval('project_project_id_seq', 1, false);
SELECT setval('projectphase_phase_id_seq', 1, false);
SELECT setval('softwarelicense_license_id_seq', 1, false);
SELECT setval('projectassignment_assignment_id_seq', 1, false);
SELECT setval('worklog_log_id_seq', 1, false);
SELECT setval('bug_bug_id_seq', 1, false);
SELECT setval('release_release_id_seq', 1, false);
SELECT setval('documentation_doc_id_seq', 1, false);
SELECT setval('projectspecification_spec_id_seq', 1, false);
SELECT setval('managercertification_certification_id_seq', 1, false);
SELECT setval('testerspecialization_employee_id_seq', 1, false);
SELECT setval('developerspecialization_employee_id_seq', 1, false);
SELECT setval('licenseallocation_allocation_id_seq', 1, false);
SELECT setval('cloudresource_resource_id_seq', 1, false);

SELECT 'Очистка завершена!' AS result;