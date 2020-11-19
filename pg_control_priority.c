/*-------------------------------------------------------------------------
 *
 * pg_control_priority.c
 *   controls the priorities of PostgreSQL server processes
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <sys/resource.h>
#include <unistd.h>

#include "funcapi.h"
#include "miscadmin.h"
#include "storage/proc.h"
#include "storage/procarray.h"
#include "utils/guc.h"

PG_MODULE_MAGIC;

/* GUC variable */
static int		scheduling_priority = 0;

PG_FUNCTION_INFO_V1(pg_get_priority);
PG_FUNCTION_INFO_V1(pg_set_priority);

void		_PG_init(void);
void		_PG_fini(void);

static int GetProcessPriority(int pid, int elevel);
static void SetProcessPriority(int pid, int priority, int elevel);
static void CheckPostgresPid(int pid);
static bool	IsAuxiliaryPid(int pid);

static void assign_scheduling_priority(int newval, void *extra);
static const char *show_scheduling_priority(void);

/*
 * Module load callback
 */
void
_PG_init(void)
{
	/* Define custom GUC variable. */
	DefineCustomIntVariable("pg_control_priority.scheduling_priority",
							"Set the scheduling priorities of PostgreSQL server processes.",
							NULL,
							&scheduling_priority,
							0,
							0,
							19,
							PGC_USERSET,
							0,
							NULL,
							assign_scheduling_priority,
							show_scheduling_priority);

	EmitWarningsOnPlaceholders("pg_control_priority");
}

/*
 * Module unload callback
 */
void
_PG_fini(void)
{
}

/*
 * Get the scheduling priority of PostgreSQL server process.
 */
Datum
pg_get_priority(PG_FUNCTION_ARGS)
{
	int		pid = PG_GETARG_INT32(0);
	int		priority;

	CheckPostgresPid(pid);
	priority = GetProcessPriority(pid, ERROR);
	PG_RETURN_INT32(priority);
}

/*
 * Set the scheduling priority of PostgreSQL server process.
 */
Datum
pg_set_priority(PG_FUNCTION_ARGS)
{
	int		pid = PG_GETARG_INT32(0);
	int		priority = PG_GETARG_INT32(1);

	CheckPostgresPid(pid);
	SetProcessPriority(pid, priority, ERROR);
	PG_RETURN_VOID();
}

/*
 * Get the scheduling priority of the specified process.
 */
static int
GetProcessPriority(int pid, int elevel)
{
	int	priority = 0;
	int	save_errno = errno;

	errno = 0;
	priority = getpriority(PRIO_PROCESS, pid);
	if (errno != 0)
		ereport(elevel,
				(errmsg("could not get the scheduling priority of process %d: %m", pid)));
	errno = save_errno;

	return priority;
}

/*
 * Set the scheduling priority of the specified process.
 */
static void
SetProcessPriority(int pid, int priority, int elevel)
{
	int	save_errno = errno;

	if (setpriority(PRIO_PROCESS, pid, priority) != 0)
		ereport(elevel,
				(errmsg("could not set the scheduling priority of process %d to %d: %m", pid, priority)));
	errno = save_errno;
}

/*
 * Check to see if a given pid is a running postmaster, backend, or
 * auxiliary process.
 */
static void
CheckPostgresPid(int pid)
{
	if (PostmasterPid != pid && !IsBackendPid(pid) && !IsAuxiliaryPid(pid))
		ereport(ERROR,
				(errcode(ERRCODE_INVALID_PARAMETER_VALUE),
				 (errmsg("PID %d is not a PostgreSQL server process", pid))));
}

/*
 * Is a given pid a running auxiliary process?
 */
static bool
IsAuxiliaryPid(int pid)
{
	return (AuxiliaryPidGetProc(pid) != NULL);
}

static void
assign_scheduling_priority(int newval, void *extra)
{
	/* See comments in assign_tcp_keepalives_idle in PostgreSQL source */
	SetProcessPriority(getpid(), newval, WARNING);
}

static const char *
show_scheduling_priority(void)
{
	/* See comments in assign_tcp_keepalives_idle in PostgreSQL source */
	static char nbuf[16];

	snprintf(nbuf, sizeof(nbuf), "%d", GetProcessPriority(getpid(), WARNING));
	return nbuf;
}
