bin_PROGRAMS = musicmove

# Unit test programs
check_PROGRAMS = metadata_test format_test
TESTS = $(check_PROGRAMS)

# Source files for "library" (i.e. excluding main)
# TODO - make this a static library so `make check' doesn't rebuild it all
libmusicmove_SOURCES = \
	context.hpp \
	metadata.cpp metadata.hpp metadata_base.hpp \
	metadata_flac.hpp metadata_mp4.hpp \
	metadata_mpeg.hpp metadata_ogg_vorbis.hpp \
	format.cpp format_easytag.cpp format.hpp \
	move.cpp move.hpp

# Source files for CLI application
musicmove_SOURCES = \
	musicmove.cpp \
	$(libmusicmove_SOURCES)

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
metadata_test_SOURCES = $(libmusicmove_SOURCES) metadata_test.cpp
metadata_test_CPPFLAGS = \
	$(musicmove_CPPFLAGS) \
	-DTESTDATA_DIR="../testdata"
metadata_test_LDADD = \
	$(musicmove_LDADD) \
	$(BOOST_UTF_LDADD)

# Format test
format_test_SOURCES = $(libmusicmove_SOURCES) format_test.cpp
format_test_CPPFLAGS = \
	$(musicmove_CPPFLAGS) \
	-DTESTDATA_DIR="../testdata"
format_test_LDADD = \
	$(musicmove_LDADD) \
	$(BOOST_UTF_LDADD)

