# src/test/modules/twitter_parser/Makefile

MODULE_big = twitter_parser
OBJS = twitter_parser.o $(WIN32RES)
PGFILEDESC = "twitter_parser - Adding hashtags and mentions parsing for full-text search"

EXTENSION = twitter_parser
DATA = twitter_parser--1.0.sql twitter_parser--unpackaged--1.0.sql

REGRESS = twitter_parser

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/twitter_parser
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
