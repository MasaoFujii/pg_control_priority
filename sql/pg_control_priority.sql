CREATE EXTENSION pg_control_priority;

\pset null '(null)'

SET pg_control_priority.scheduling_priority TO 1;
SELECT pg_get_priority(pg_backend_pid());
SELECT pg_set_priority(pg_backend_pid(), 2);
SHOW pg_control_priority.scheduling_priority;

DROP EXTENSION pg_control_priority;
