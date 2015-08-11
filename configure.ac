AC_INIT([fastjudge], [0.0.1], [root@jackyyf.com])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_CANONICAL_BUILD

# echo $build_os
case $build_os in
	linux*)
		;;
	*)
		AC_MSG_ERROR([fastjudge is targeted for linux only.])
		break
		;;
esac

AC_PROG_CC
AC_PROG_CC_STDC
m4_include([libev/libev.m4])

PKG_CHECK_MODULES([libconfig],[libconfig],,AC_MSG_ERROR([libconfig not found]))
CFLAGS+=" $libconfig_CFLAGS"
LIBS+=" $libconfig_LIBS"

AC_CONFIG_SUBDIRS([libev])
AC_OUTPUT
