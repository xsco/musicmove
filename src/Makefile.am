# Static libraries, not installed
noinst_LIBRARIES = libmusicmove.a

# Source files for the 'library'
libmusicmove_a_SOURCES = \
	context.hpp \
	metadata.cpp metadata.hpp metadata_base.hpp \
	metadata_flac.hpp metadata_mp4.hpp \
	metadata_mpeg.hpp metadata_ogg_vorbis.hpp \
	format.cpp format_easytag.cpp format.hpp \
	move.cpp move.hpp

# Compiler flags.
libmusicmove_a_CPPFLAGS = \
	-Werror -Wall \
	$(TAGLIB_CFLAGS)


# Programs
bin_PROGRAMS = musicmove

# Source files for CLI application
musicmove_SOURCES = \
	musicmove.cpp

# Compiler flags.
musicmove_CPPFLAGS = \
	-Werror -Wall

# Additional libraries for the linker.
musicmove_LDADD = \
	libmusicmove.a \
	$(BOOST_SYSTEM_LDADD) \
	$(BOOST_PROGRAM_OPTIONS_LDADD) \
	$(BOOST_FILESYSTEM_LDADD) \
	$(BOOST_LOCALE_LDADD) \
	$(TAGLIB_LDADD)


# Unit test programs
check_PROGRAMS = metadata_test format_test move_test
TESTS = $(check_PROGRAMS)

# Metadata test
metadata_test_SOURCES = \
	metadata_test.cpp \
	metadata.cpp
metadata_test_CPPFLAGS = \
	-Werror -Wall \
	$(TAGLIB_CFLAGS) \
	-DTESTDATA_DIR="../testdata"
metadata_test_LDADD = \
	$(BOOST_SYSTEM_LDADD) \
	$(BOOST_FILESYSTEM_LDADD) \
	$(TAGLIB_LDADD) \
	$(BOOST_UTF_LDADD)

# Format test
format_test_SOURCES = \
	format_test.cpp \
	format.cpp format.hpp
format_test_CPPFLAGS = \
	-Werror -Wall \
	-DTESTDATA_DIR="../testdata"
format_test_LDADD = \
	$(BOOST_SYSTEM_LDADD) \
	$(BOOST_FILESYSTEM_LDADD) \
	$(BOOST_LOCALE_LDADD) \
	$(BOOST_UTF_LDADD)

# Move test
move_test_SOURCES = \
	move_test.cpp \
	move.cpp \
	format_mock.cpp \
	metadata_mock.cpp
move_test_CPPFLAGS = \
	-Werror -Wall \
	-DTESTDATA_DIR="../testdata"
move_test_LDADD = \
	$(BOOST_SYSTEM_LDADD) \
	$(BOOST_FILESYSTEM_LDADD) \
	$(BOOST_UTF_LDADD)

