bin_PROGRAMS = musicmove

# Source files.
musicmove_SOURCES = \
	musicmove.cpp \
	context.hpp \
	move.cpp move.hpp

# Compiler flags.
musicmove_CPPFLAGS = \
	-Werror -Wall \
	$(TAGLIB_CFLAGS)

# Additional libraries for the linker.
musicmove_LDADD = \
	$(BOOST_SYSTEM_LDADD) \
	$(BOOST_PROGRAM_OPTIONS_LDADD) \
	$(BOOST_FILESYSTEM_LDADD) \
	$(BOOST_LOCALE_LDADD) \
	$(TAGLIB_LDADD)

