AC_INIT([musicmove bulk renamer], [0.0.1], [bugs@xsco.org], [musicmove])
#AM_SILENT_RULES([yes])
AM_INIT_AUTOMAKE([-Wall])

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_LANG([C++])
AM_PROG_CC_C_O

# Require C++14
AX_CXX_COMPILE_STDCXX([14], [noext], [mandatory])

# Check some mandatory dependencies.
AC_CHECK_HEADERS([boost/program_options.hpp], [],
    [AC_MSG_ERROR([couldn't find boost/program_options.hpp])])
AC_CHECK_HEADERS([boost/filesystem.hpp], [],
    [AC_MSG_ERROR([couldn't find boost/filesystem.hpp])])
AC_CHECK_HEADERS([boost/locale.hpp], [],
    [AC_MSG_ERROR([couldn't find boost/locale.hpp])])
PKG_CHECK_MODULES([TAGLIB], [taglib])

# Library names.
AC_SUBST([BOOST_SYSTEM_LDADD], [-lboost_system])
AC_SUBST([BOOST_PROGRAM_OPTIONS_LDADD], [-lboost_program_options])
AC_SUBST([BOOST_FILESYSTEM_LDADD], [-lboost_filesystem])
AC_SUBST([BOOST_LOCALE_LDADD], [-lboost_locale])
AC_SUBST([TAGLIB_LDADD], [$TAGLIB_LIBS])

# Use config.h
AC_CONFIG_HEADERS([config.h])

# Makefiles
AC_CONFIG_FILES([
	Makefile
	src/Makefile
])


AC_OUTPUT

