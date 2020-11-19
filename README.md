# pg_control_priority

This extension provides the ability to control the scheduling priorities
("nice") of PostgreSQL server processes.

This extension is released under the
[PostgreSQL License](https://opensource.org/licenses/postgresql),
a liberal Open Source license, similar to the BSD or MIT licenses.

This extension requires PostgreSQL 10 or later.

## Test Status

![build](https://github.com/MasaoFujii/pg_control_priority/workflows/build/badge.svg)

## Install

Download the source archive of pg_control_priority from
[here](https://github.com/MasaoFujii/pg_control_priority),
and then build and install it.

    $ cd pg_control_priority
    $ make USE_PGXS=1 PG_CONFIG=/opt/pgsql-X.Y.Z/bin/pg_config
    $ su
    # make USE_PGXS=1 PG_CONFIG=/opt/pgsql-X.Y.Z/bin/pg_config install
    # exit

USE_PGXS=1 must be always specified when building this extension.
The path to [pg_config](http://www.postgresql.org/docs/devel/static/app-pgconfig.html)
(which exists in the bin directory of PostgreSQL installation)
needs be specified in PG_CONFIG.
However, if the PATH environment variable contains the path to pg_config,
PG_CONFIG doesn't need to be specified.

## Functions

Note that **CREATE EXTENSION pg_control_priority** needs to be executed
in all the databases that you want to execute the functions that
this extension provides.

    =# CREATE EXTENSION pg_control_priority;

### integer pg_get_priority(pid int)

Return the scheduling priority ("nice") of the specified PostgreSQL
server process. This function can get the priority of postmaster,
backend, or auxiliary process.
See getpriority(2) man page for details about a scheduling priority.

This function is restricted to superusers by default,
but other users can be granted EXECUTE to run the function.

### void pg_set_priority(pid int, priority int)

Set the scheduling priority ("nice") of the specified PostgreSQL
server process to the specified value. This function can change
the priority of postmaster, backend, or auxiliary process.

Valid priority values are between 0 and 19 in Linux,
and between 0 and 20 in other systems like MacOS.
Lower value than the current priority cannot be specified.
See getpriority(2) man page for details about a scheduling priority.

This function is restricted to superusers by default,
but other users can be granted EXECUTE to run the function.

Note that this extension doesn't accept a priority value in range
between -20 and -1 while setpriority(2) does. Because the default
priority is zero, and only root may lower the priority but PostgreSQL
is not allowed to run as root.

## Configuration Parameters

Note that [shared_preload_libraries](http://www.postgresql.org/docs/devel/static/runtime-config-client.html#GUC-SHARED-PRELOAD-LIBRARIES)
or [session_preload_libraries](http://www.postgresql.org/docs/devel/static/runtime-config-client.html#GUC-SESSION-PRELOAD-LIBRARIES)
must be set to 'pg_control_priority' in postgresql.conf
if you want to use the configuration parameters which this extension provides.

### pg_control_priority.scheduling_priority (integer)

Specify the scheduling priorities ("nice") of PostgreSQL server processes.
Valid priority values are between 0 and 19 in Linux,
and between 0 and 20 in other systems like MacOS.
Lower value than the current priority cannot be specified.
The default value is zero.
See getpriority(2) man page for details about a scheduling priority.

Any users can change this setting.
