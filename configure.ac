AC_INIT([musicmove bulk renamer for music files], [0.0.1], [bugs@xsco.org], [musicmove])
AM_SILENT_RULES([yes])
AM_INIT_AUTOMAKE([-Wall])

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_LANG([C++])
AC_PROG_RANLIB

AM_PROG_CC_C_O
AM_PROG_AR
AC_SUBST([ARFLAGS], [cr])

# Require C++14
AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])

# Check some mandatory dependencies.
AC_CHECK_HEADERS([boost/program_options.hpp], [],
    [AC_MSG_ERROR([couldn't find boost/program_options.hpp])])
AC_CHECK_HEADERS([boost/filesystem.hpp], [],
    [AC_MSG_ERROR([couldn't find boost/filesystem.hpp])])
AC_CHECK_HEADERS([boost/locale.hpp], [],
    [AC_MSG_ERROR([couldn't find boost/locale.hpp])])
AC_CHECK_HEADERS([boost/test/unit_test.hpp], [],
    [AC_MSG_ERROR([couldn't find boost/test/unit_test.hpp])])
PKG_CHECK_MODULES([TAGLIB], [taglib])
#PKG_CHECK_MODULES([CHAISCRIPT], [chaiscript])

# Library names.
AC_SUBST([BOOST_SYSTEM_LIBADD], [-lboost_system])
AC_SUBST([BOOST_PROGRAM_OPTIONS_LIBADD], [-lboost_program_options])
AC_SUBST([BOOST_FILESYSTEM_LIBADD], [-lboost_filesystem])
AC_SUBST([BOOST_LOCALE_LIBADD], [-lboost_locale])
AC_SUBST([TAGLIB_LIBADD], [$TAGLIB_LIBS])
AC_SUBST([BOOST_UTF_LDADD], [-lboost_unit_test_framework])

# Path conversion default, which may be OS-specific one day.
AC_DEFINE([PATH_CONVERSION_DEFAULT_VALUE], ["windows-ascii"],
          [Default path conversion command-line value])

# Use config.h
AC_CONFIG_HEADERS([config.h])

# Makefiles
AC_CONFIG_FILES([
	Makefile
	src/Makefile
])


AC_OUTPUT

