AC_INIT([musicmove], [1.0.0], [bugs@xsco.org], [musicmove])
AM_SILENT_RULES([yes])
AM_INIT_AUTOMAKE([-Wall])

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_LANG([C++])
AC_PROG_RANLIB

AM_PROG_CC_C_O
AM_PROG_AR
AC_SUBST([ARFLAGS], [cr])

AC_PATH_PROG(HELP2MAN, help2man, no)
if test x"${HELP2MAN}" == x"no"; then
    AC_MSG_ERROR([`help2man' not found in path.  Please install help2man \
before trying to build ${PACKAGE}.])
fi
AC_PATH_PROG(MAN2HTML, man2html, no)
if test x"${MAN2HTML}" == x"no"; then
	AC_MSG_ERROR([`man2html' not found in path.  Please install man2html \
before trying to build ${PACKAGE}.])
fi

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
AC_SUBST([BOOST_SYSTEM_LDADD], [-lboost_system])
AC_SUBST([BOOST_PROGRAM_OPTIONS_LDADD], [-lboost_program_options])
AC_SUBST([BOOST_FILESYSTEM_LDADD], [-lboost_filesystem])
AC_SUBST([BOOST_LOCALE_LDADD], [-lboost_locale])
AC_SUBST([TAGLIB_LDADD], [$TAGLIB_LIBS])
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

