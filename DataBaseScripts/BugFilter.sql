create or replace function get_bugs_by_filter(
    p_project_id int DEFAULT NULL,
    p_bug_status varchar(50)[] default null,
    p_bug_severity varchar(50)[] default null
)
returns table (
    bug_id int,
    project_name varchar(200),
    title varchar(300),
    description text,
    status varchar(50),
    severity varchar(50),
    found_by integer,
    fixed_by integer,
    created_at TIMESTAMP,
    found_date date,
    fixed_date date
) LANGUAGE plpgsql
AS $$
begin
    return query
    select
        b.bug_id,
        p.name as project_name,
        b.title,
        b.description,
        b.status,
        b.severity,
        b.found_by,
        b.fixed_by,
        b.created_at,
        b.found_date,
        b.fixed_date
    from bug b
    join project p on b.project_id = p.project_id
    where
        (p_project_id is null or b.project_id = p_project_id)
    and (p_bug_status is null or b.status = any(p_bug_status))
    and (p_bug_severity is null or b.severity = any(p_bug_severity));
end;
$$