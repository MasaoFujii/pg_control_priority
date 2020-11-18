MODULE_big = pg_control_priority
OBJS = pg_control_priority.o

EXTENSION = pg_control_priority
DATA = pg_control_priority--1.0.sql
PGFILEDESC = "pg_control_priority - controls the priority of PostgreSQL server process"

REGRESS = pg_control_priority

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/pg_control_priority
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
