bin_PROGRAMS = musicmove

# Unit test programs
check_PROGRAMS = metadata_test
TESTS = $(check_PROGRAMS)

# Source files.
musicmove_SOURCES = \
	musicmove.cpp \
	context.hpp \
	metadata.cpp metadata.hpp metadata_impl.hpp\
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

# Metadata test
metadata_test_SOURCES = metadata.cpp metadata.hpp metadata_test.cpp
metadata_test_CPPFLAGS = \
	$(musicmove_CPPFLAGS) \
	-DTESTDATA_DIR="../testdata"
metadata_test_LDADD = \
	$(musicmove_LDADD) \
	$(BOOST_UTF_LDADD)

