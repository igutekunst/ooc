## Process this file with automake to produce Makefile.in

TESTS = check_string
check_PROGRAMS = check_string
check_string_SOURCES = check_string.c $(top_builddir)/src/object.h $(top_buildir)/src/string.h
check_string_CFLAGS = @CHECK_CFLAGS@
check_string_LDADD = $(top_builddir)/src/libooc.a @CHECK_LIBS@
