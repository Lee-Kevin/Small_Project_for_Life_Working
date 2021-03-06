#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([IMI_UTIL], [1.0.0], [lijiankai@imilab.com])
AM_INIT_AUTOMAKE([-Wall subdir-objects])
AC_CONFIG_SRCDIR(src)
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
#AC_PROG_LIBTOOL

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS(stdlib.h)

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS(memset)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
