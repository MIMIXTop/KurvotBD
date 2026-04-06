CREATE OR REPLACE FUNCTION trg_generate_specification()
    RETURNS TRIGGER LANGUAGE plpgsql AS $$
DECLARE
    v_tz_text TEXT;
    v_client_name VARCHAR(200);
    v_client_type VARCHAR(50);
    v_next_version INTEGER;
BEGIN
    -- Получаем данные заказчика
    SELECT name, type INTO v_client_name, v_client_type
    FROM Client
    WHERE client_id = NEW.client_id;

    -- Определяем следующую версию ТЗ для этого проекта
    SELECT COALESCE(MAX(version), 0) + 1 INTO v_next_version
    FROM ProjectSpecification
    WHERE project_id = NEW.project_id;

    v_tz_text := '=========================================' || E'\n' ||
                 ' ТЕХНИЧЕСКОЕ ЗАДАНИЕ (Версия ' || v_next_version || ')' || E'\n' ||
                 '=========================================' || E'\n' ||
                 'Заказчик: ' || v_client_name || ' (' || v_client_type || ')' || E'\n' ||
                 'Проект: ' || NEW.name || E'\n' ||
                 'Тип программного обеспечения: ' || COALESCE(NEW.type, 'Не указан') || E'\n' ||
                 'Методология разработки: ' || COALESCE(NEW.methodology, 'Не указана') || E'\n' ||
                 'Технологический стек: ' || COALESCE(array_to_string(NEW.tech_stack, ', '), 'Не определен') || E'\n' ||
                 'Согласованный бюджет: ' || NEW.budget || ' руб.' || E'\n' ||
                 'Плановые сроки: ' || NEW.start_date || ' - ' || NEW.planned_end_date || E'\n' ||
                 '-----------------------------------------' || E'\n' ||
                 'Требования к системе: [Заполняется аналитиком]' || E'\n' ||
                 'Дата автоматической генерации: ' || CURRENT_TIMESTAMP::TEXT || E'\n' ||
                 '=========================================';

    -- Записываем результат в таблицу спецификаций
    INSERT INTO ProjectSpecification (project_id, document_text, version)
    VALUES (NEW.project_id, v_tz_text, v_next_version);

    RETURN NEW;
END;
$$;

CREATE TRIGGER trigger_auto_generate_spec
    AFTER INSERT OR UPDATE OF type, methodology, budget, tech_stack, planned_end_date
    ON Project
    FOR EACH ROW
EXECUTE FUNCTION trg_generate_specification();