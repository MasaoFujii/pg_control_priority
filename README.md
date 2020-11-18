# pg_control_priority
This extension provides the ability to control the scheduling priority
("nice") of PostgreSQL server process.

pg_control_priority is released under the
[PostgreSQL License](https://opensource.org/licenses/postgresql),
a liberal Open Source license, similar to the BSD or MIT licenses.

pg_control_priority requires PostgreSQL 10 or later.

## Test Status
[![Build Status](https://travis-ci.org/MasaoFujii/pg_control_priority.svg?branch=master)](https://travis-ci.org/MasaoFujii/pg_control_priority)

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
See getpriority(2) man page for details about a scheduling priority.
This function is restricted to superusers by default,
but other users can be granted EXECUTE to run the function.

## Configuration Parameters

Note that [shared_preload_libraries](http://www.postgresql.org/docs/devel/static/runtime-config-client.html#GUC-SHARED-PRELOAD-LIBRARIES)
or [session_preload_libraries](http://www.postgresql.org/docs/devel/static/runtime-config-client.html#GUC-SESSION-PRELOAD-LIBRARIES)
must be set to 'pg_control_priority' in postgresql.conf
if you want to use the configuration parameters which this extension provides.

### pg_control_priority.scheduling_priority (integer)
Specify the scheduling priority ("nice") of PostgreSQL server process.
Valid values are between -20 and 19.
Lower values cause more favorable scheduling.
The default value is zero.
Any users can change this setting.
See getpriority(2) man page for details about a scheduling priority.
