AC_PREREQ(2.57)
AC_INIT([Raft Prototype], [0.0.1], [the.real.steve.mckay@aol.com])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SUBDIRS([glog-0.3.3 gflags-2.0])
AM_INIT_AUTOMAKE([foreign dist-xz no-dist-gzip subdir-objects])
AM_SILENT_RULES([yes])

AC_PROG_CXX
AC_LANG([C++])
ACX_PTHREAD
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"
LIBS="$LIBS $PTHREAD_LIBS"

AC_SYS_LARGEFILE

AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

