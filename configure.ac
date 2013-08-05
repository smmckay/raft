AC_PREREQ(2.57)
AC_INIT([Raft Prototype], [0.0.1], [the.real.steve.mckay@aol.com])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign dist-xz no-dist-gzip subdir-objects])

AC_PROG_CXX([clang++ g++])
AC_LANG([C++])
AX_BOOST_BASE([1.54.0])
ACX_PTHREAD
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS"
LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
LIBS="$LIBS $PTHREAD_LIBS"

AC_MSG_CHECKING([whether we are using clang])
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([], [[#ifndef __clang__
#error not clang
#endif
]])],
		  [ac_clang=yes],
		  [ac_clang=no])
AC_MSG_RESULT([$ac_clang])
AM_CONDITIONAL([CLANG], [test x$ac_clang = xyes])

### Copied from gflags configure.ac

AX_C___ATTRIBUTE__
# We only care about __attribute__ ((unused))
if test x"$ac_cv___attribute__" = x"yes"; then
  ac_cv___attribute__unused="__attribute__ ((unused))"
else
  ac_cv___attribute__unused=
fi

# Check whether some low-level functions/files are available
AC_HEADER_STDC

# These are tested for by AC_HEADER_STDC, but I check again to set the var
AC_CHECK_HEADER(stdint.h, ac_cv_have_stdint_h=1, ac_cv_have_stdint_h=0)
AC_CHECK_HEADER(sys/types.h, ac_cv_have_systypes_h=1, ac_cv_have_systypes_h=0)
AC_CHECK_HEADER(inttypes.h, ac_cv_have_inttypes_h=1, ac_cv_have_inttypes_h=0)
AC_CHECK_HEADERS([fnmatch.h])
AC_CHECK_HEADERS([sys/stat.h])
AC_CHECK_HEADERS([unistd.h])

# These are the types I need.  We look for them in either stdint.h,
# sys/types.h, or inttypes.h, all of which are part of the default-includes.
AC_CHECK_TYPE(uint16_t, ac_cv_have_uint16_t=1, ac_cv_have_uint16_t=0)
AC_CHECK_TYPE(u_int16_t, ac_cv_have_u_int16_t=1, ac_cv_have_u_int16_t=0)
AC_CHECK_TYPE(__int16, ac_cv_have___int16=1, ac_cv_have___int16=0)

AC_CHECK_FUNCS([strtoll strtoq])

# Find out what namespace 'normal' STL code lives in, and also what namespace
# the user wants our classes to be defined in
AC_CXX_STL_NAMESPACE
# TODO(csilvers): this should be renamed to gflags.
AC_DEFINE_GOOGLE_NAMESPACE(google)

# These are what's needed by gflags.h.in
AC_SUBST(ac_google_start_namespace)
AC_SUBST(ac_google_end_namespace)
AC_SUBST(ac_google_namespace)
AC_SUBST(ac_cv___attribute__unused)
AC_SUBST(ac_cv_have_stdint_h)
AC_SUBST(ac_cv_have_systypes_h)
AC_SUBST(ac_cv_have_inttypes_h)
AC_SUBST(ac_cv_have_uint16_t)
AC_SUBST(ac_cv_have_u_int16_t)
AC_SUBST(ac_cv_have___int16)

# For windows, this has a non-trivial value (__declspec(export)), but any
# system that uses configure wants this to be the empty string.
AC_DEFINE(GFLAGS_DLL_DECL,,
          [Always the empty-string on non-windows systems.
           On windows, should be "__declspec(dllexport)".
	   This way, when we compile the dll, we export our functions/classes.
	   It's safe to define this here because config.h is only used
	   internally, to compile the DLL, and every DLL source file
	   #includes "config.h" before anything else.])

# Write generated configuration file, and also .h files
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_FILES([Makefile
                 src/gflags/gflags.h src/gflags/gflags_declare.h
                 src/gflags/gflags_completions.h])
AC_OUTPUT
