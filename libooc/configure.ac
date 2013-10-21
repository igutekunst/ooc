#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ooc], [0.0.1], [igutekunst@gmail.com])

AM_INIT_AUTOMAKE([0.0.1 foreign parallel-tests color-tests])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_HEADERS([config.h])

# Set some compiler settings
AC_PROG_CC_STDC

# Checks for programs.

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T

AC_PROG_CC
AM_PROG_AR
AC_PROG_RANLIB

# Tests
AM_PATH_CHECK() 

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 tests/Makefile])
AC_OUTPUT
