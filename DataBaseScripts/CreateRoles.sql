-- ============================================
-- СОЗДАНИЕ РОЛЕЙ И НАЗНАЧЕНИЕ ПРАВ
-- ============================================

-- Удаление существующих ролей (если нужно пересоздать)
-- DROP ROLE IF EXISTS admin_role;
-- DROP ROLE IF EXISTS user_role;

-- Создание ролей
CREATE ROLE admin_role LOGIN PASSWORD 'admin123';
CREATE ROLE user_role LOGIN PASSWORD 'user123';

-- GRANT USAGE ON SCHEMA public TO admin_role;
-- GRANT USAGE ON SCHEMA public TO user_role;

-- Полные права для admin_role
GRANT ALL ON ALL TABLES IN SCHEMA public TO admin_role;
GRANT ALL ON ALL SEQUENCES IN SCHEMA public TO admin_role;
GRANT ALL ON ALL FUNCTIONS IN SCHEMA public TO admin_role;

-- Только чтение для user_role
GRANT SELECT ON ALL TABLES IN SCHEMA public TO user_role;
GRANT USAGE ON ALL SEQUENCES IN SCHEMA public TO user_role;

-- Установка прав по умолчанию для новых таблиц
ALTER DEFAULT PRIVILEGES FOR ROLE admin_role IN SCHEMA public GRANT ALL ON TABLES TO admin_role;
ALTER DEFAULT PRIVILEGES FOR ROLE admin_role IN SCHEMA public GRANT ALL ON SEQUENCES TO admin_role;
ALTER DEFAULT PRIVILEGES FOR ROLE user_role IN SCHEMA public GRANT SELECT ON TABLES TO user_role;

-- Проверка созданных ролей
SELECT rolname, rolsuper, rolcanlogin FROM pg_roles WHERE rolname IN ('admin_role', 'user_role');

SELECT 'Роли созданы успешно!' AS result;