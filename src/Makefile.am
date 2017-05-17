# Extra stuff to clean
MOSTLYCLEANFILES = musicmove.1 musicmove.html

# Static libraries, not installed
noinst_LIBRARIES = libscriptrunner.a libmusicmove.a

libscriptrunner_a_SOURCES = \
	context.hpp \
	metadata.hpp \
	script_runner.cpp script_runner.hpp
libscriptrunner_a_CPPFLAGS = \
	-Werror -Wall \
	$(TAGLIB_CFLAGS) \
	$(CHAISCRIPT_CFLAGS)

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
	$(TAGLIB_CFLAGS) \
	$(CHAISCRIPT_CFLAGS)


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
	libscriptrunner.a \
	$(BOOST_SYSTEM_LDADD) \
	$(BOOST_PROGRAM_OPTIONS_LDADD) \
	$(BOOST_FILESYSTEM_LDADD) \
	$(BOOST_LOCALE_LDADD) \
	$(TAGLIB_LDADD) \
	$(CHAISCRIPT_LDADD) -ldl

# Make the man page using help2man
man_MANS = musicmove.1
musicmove.1 : musicmove.cpp
	$(HELP2MAN) \
		--output=$@ \
		--name="Bulk renamer for music files" \
		--version-string="$(PACKAGE_VERSION)" \
	    --no-info \
	    ./musicmove
# Make html version of manual
.PHONY : html-local
html-local : musicmove.html
musicmove.html : musicmove.1
	$(MAN2HTML) -r $< | tail --lines=+3 > $@


# Unit test programs
check_PROGRAMS = metadata_test format_test script_runner_test move_test
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
	metadata_mock.cpp \
	format.cpp format_easytag.cpp format.hpp
format_test_CPPFLAGS = \
	-Werror -Wall \
	-DTESTDATA_DIR="../testdata"
format_test_LDADD = \
	$(BOOST_SYSTEM_LDADD) \
	$(BOOST_FILESYSTEM_LDADD) \
	$(BOOST_LOCALE_LDADD) \
	$(BOOST_UTF_LDADD)

# Script runner test
script_runner_test_SOURCES = \
	script_runner_test.cpp \
	metadata_mock.cpp
script_runner_test_CPPFLAGS = \
	-Werror -Wall \
	-DTESTDATA_DIR="../testdata"
script_runner_test_LDADD = \
	libscriptrunner.a \
	$(BOOST_SYSTEM_LDADD) \
	$(BOOST_FILESYSTEM_LDADD) \
	$(BOOST_LOCALE_LDADD) \
	$(BOOST_UTF_LDADD) \
	$(CHAISCRIPT_LDADD) -ldl

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
	libscriptrunner.a \
	$(BOOST_SYSTEM_LDADD) \
	$(BOOST_FILESYSTEM_LDADD) \
	$(BOOST_UTF_LDADD) \
	$(CHAISCRIPT_LDADD) -ldl

