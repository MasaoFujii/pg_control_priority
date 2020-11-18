-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pg_control_priority" to load this file. \quit

CREATE FUNCTION pg_get_priority(integer)
RETURNS integer
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT VOLATILE;
REVOKE ALL ON FUNCTION pg_get_priority(integer) FROM PUBLIC;

CREATE FUNCTION pg_set_priority(integer, integer)
RETURNS void
AS 'MODULE_PATHNAME'
LANGUAGE C STRICT VOLATILE;
REVOKE ALL ON FUNCTION pg_set_priority(integer, integer) FROM PUBLIC;
