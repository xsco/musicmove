/*
    musicmove - Bulk renamer for music files
    Copyright (C) 2016-2017  Adam Szmigin (adam.szmigin@xsco.net)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "format.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE format_test
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <string>

using namespace std;

BOOST_AUTO_TEST_CASE (path_conversion_posix)
{
    mm::context ctx;
    ctx.path_conversion = mm::path_conversion_t::posix;
    
    // Non-printable characters should be converted to underscore
    // Trailing underscores should be removed
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        u8"abcd"
        "\u0001\u0002\u0003\u0004\u0005\u0006\u0007\u0008\u0009"
        "\u000a\u000b\u000c\u000d\u000e\u000f\u0010\u0011\u0012\u0013"
        "\u0014\u0015\u0016\u0017\u0018\u0019\u001a\u001b\u001c\u001d"
        "\u001e\u001f",
        ctx),
        "abcd");

    // Forbidden ASCII characters should be converted to underscore
    // Trailing underscores should be removed
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "1ascii_fragment-str", ctx),
        "1ascii_fragment-str");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "spaces get converted", ctx),
        "spaces_get_converted");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "foo (foo) \"foo\" <foo>", ctx),
        "foo__foo___foo___foo");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "some title (name of remix)", ctx),
        "some_title__name_of_remix");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "This / That", ctx),
        "This_-_That");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "That & This", ctx),
        "That___This");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "xyzw<>:\"|", ctx),
        "xyzw");

    // Accented characters (in UTF-8) should be converted to non-accented
    // 7-bit equivalents; anything else to underscore
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        u8"S\u00f5m\u00ea \u00e5\u00e7c\u00e9\u00f1ted characters here", ctx),
        "Some_accented_characters_here");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "Basic Latin punctuation:"
        "\u00a0\u00a1\u00a2\u00a3\u00a4\u00a5\u00a6\u00a7\u00a8"
        "\u00a9\u00aa\u00ab\u00ac\u00ad\u00ae\u00af\u00b0\u00b1\u00b2"
        "\u00b3\u00b4\u00b5\u00b6\u00b7\u00b8\u00b9\u00ba\u00bb\u00bc"
        "\u00bd\u00be\u00bf", ctx),
        "Basic_Latin_punctuation");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "Basic Latin letters: "
        "\u00c0\u00c1\u00c2\u00c3\u00c4\u00c5\u00c6\u00c7\u00c8\u00c9"
        "\u00ca\u00cb\u00cc\u00cd\u00ce\u00cf\u00d0\u00d1\u00d2\u00d3"
        "\u00d4\u00d5\u00d6\u00d7\u00d8\u00d9\u00da\u00db\u00dc\u00dd"
        "\u00de\u00df\u00e0\u00e1\u00e2\u00e3\u00e4\u00e5\u00e6\u00e7"
        "\u00e8\u00e9\u00ea\u00eb\u00ec\u00ed\u00ee\u00ef\u00f0\u00f1"
        "\u00f2\u00f3\u00f4\u00f5\u00f6\u00f7\u00f8\u00f9\u00fa\u00fb"
        "\u00fc\u00fd\u00fe\u00ff", ctx),
        "Basic_Latin_letters__"
        "AAAAAAECEE"
        "EEIIIIDNOO"
        "OOO_OUUUUY"
        "Psaaaaaaec"
        "eeeeiiiion"
        "ooooo_ouuu"
        "uypy");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "Latin-1 Supplement, some of: "
        "\u0179\u017a\u017b\u017c\u017d\u017e\u017f", ctx),
        "Latin-1_Supplement__some_of");
}

BOOST_AUTO_TEST_CASE(path_conversion_utf8)
{
    mm::context ctx;
    ctx.path_conversion = mm::path_conversion_t::utf8;
    
    // Non-printable characters should be converted to underscore
    // Trailing underscores are not removed
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        u8"abcd"
        "\u0001\u0002\u0003\u0004\u0005\u0006\u0007\u0008\u0009"
        "\u000a\u000b\u000c\u000d\u000e\u000f\u0010\u0011\u0012\u0013"
        "\u0014\u0015\u0016\u0017\u0018\u0019\u001a\u001b\u001c\u001d"
        "\u001e\u001f",
        ctx),
        "abcd_______________________________");

    // Forbidden ASCII characters should be converted to underscore
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "1ascii_fragment-str", ctx),
        "1ascii_fragment-str");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "spaces not converted", ctx),
        "spaces not converted");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "foo (foo) \"foo\" <foo>", ctx),
        "foo (foo) \"foo\" <foo>");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "some title (name of remix)", ctx),
        "some title (name of remix)");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "This / That", ctx),
        "This - That");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "That & This", ctx),
        "That & This");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "xyzw<>:\"|", ctx),
        "xyzw<>:\"|");

    // Accented characters (in UTF-8) should be be preserved
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        u8"S\u00f5m\u00ea \u00e5\u00e7c\u00e9\u00f1ted characters here", ctx),
        u8"S\u00f5m\u00ea \u00e5\u00e7c\u00e9\u00f1ted characters here");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "Basic Latin punctuation:"
        "\u00a0\u00a1\u00a2\u00a3\u00a4\u00a5\u00a6\u00a7\u00a8"
        "\u00a9\u00aa\u00ab\u00ac\u00ad\u00ae\u00af\u00b0\u00b1\u00b2"
        "\u00b3\u00b4\u00b5\u00b6\u00b7\u00b8\u00b9\u00ba\u00bb\u00bc"
        "\u00bd\u00be\u00bf", ctx),
        "Basic Latin punctuation:"
        "\u00a0\u00a1\u00a2\u00a3\u00a4\u00a5\u00a6\u00a7\u00a8"
        "\u00a9\u00aa\u00ab\u00ac\u00ad\u00ae\u00af\u00b0\u00b1\u00b2"
        "\u00b3\u00b4\u00b5\u00b6\u00b7\u00b8\u00b9\u00ba\u00bb\u00bc"
        "\u00bd\u00be\u00bf");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "Basic Latin letters: "
        "\u00c0\u00c1\u00c2\u00c3\u00c4\u00c5\u00c6\u00c7\u00c8\u00c9"
        "\u00ca\u00cb\u00cc\u00cd\u00ce\u00cf\u00d0\u00d1\u00d2\u00d3"
        "\u00d4\u00d5\u00d6\u00d7\u00d8\u00d9\u00da\u00db\u00dc\u00dd"
        "\u00de\u00df\u00e0\u00e1\u00e2\u00e3\u00e4\u00e5\u00e6\u00e7"
        "\u00e8\u00e9\u00ea\u00eb\u00ec\u00ed\u00ee\u00ef\u00f0\u00f1"
        "\u00f2\u00f3\u00f4\u00f5\u00f6\u00f7\u00f8\u00f9\u00fa\u00fb"
        "\u00fc\u00fd\u00fe\u00ff", ctx),
        "Basic Latin letters: "
        "\u00c0\u00c1\u00c2\u00c3\u00c4\u00c5\u00c6\u00c7\u00c8\u00c9"
        "\u00ca\u00cb\u00cc\u00cd\u00ce\u00cf\u00d0\u00d1\u00d2\u00d3"
        "\u00d4\u00d5\u00d6\u00d7\u00d8\u00d9\u00da\u00db\u00dc\u00dd"
        "\u00de\u00df\u00e0\u00e1\u00e2\u00e3\u00e4\u00e5\u00e6\u00e7"
        "\u00e8\u00e9\u00ea\u00eb\u00ec\u00ed\u00ee\u00ef\u00f0\u00f1"
        "\u00f2\u00f3\u00f4\u00f5\u00f6\u00f7\u00f8\u00f9\u00fa\u00fb"
        "\u00fc\u00fd\u00fe\u00ff");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "Latin-1 Supplement, some of: "
        "\u0179\u017a\u017b\u017c\u017d\u017e\u017f", ctx),
        "Latin-1 Supplement, some of: "
        "\u0179\u017a\u017b\u017c\u017d\u017e\u017f");
}

BOOST_AUTO_TEST_CASE(path_conversion_windows)
{
    mm::context ctx;
    ctx.path_conversion = mm::path_conversion_t::windows_ascii;
    
    // Non-printable characters should be converted to underscore
    // Trailing underscores should be removed
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        u8"abcd"
        "\u0001\u0002\u0003\u0004\u0005\u0006\u0007\u0008\u0009"
        "\u000a\u000b\u000c\u000d\u000e\u000f\u0010\u0011\u0012\u0013"
        "\u0014\u0015\u0016\u0017\u0018\u0019\u001a\u001b\u001c\u001d"
        "\u001e\u001f",
        ctx),
        "abcd");

    // Forbidden ASCII characters should be converted to underscore
    // Trailing underscores should be removed
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "1ascii_fragment-str", ctx),
        "1ascii_fragment-str");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "spaces not converted", ctx),
        "spaces not converted");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "foo (foo) \"foo\" <foo>", ctx),
        "foo (foo) _foo_ _foo");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "some title (name of remix)", ctx),
        "some title (name of remix)");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "This / That", ctx),
        "This - That");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "That & This", ctx),
        "That & This");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "xyzw<>:\"|", ctx),
        "xyzw");
    
    // Accented characters (in UTF-8) should be converted to non-accented
    // 7-bit equivalents; anything else to underscore
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        u8"S\u00f5m\u00ea \u00e5\u00e7c\u00e9\u00f1ted characters here", ctx),
        "Some accented characters here");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "Basic Latin punctuation:"
        "\u00a0\u00a1\u00a2\u00a3\u00a4\u00a5\u00a6\u00a7\u00a8"
        "\u00a9\u00aa\u00ab\u00ac\u00ad\u00ae\u00af\u00b0\u00b1\u00b2"
        "\u00b3\u00b4\u00b5\u00b6\u00b7\u00b8\u00b9\u00ba\u00bb\u00bc"
        "\u00bd\u00be\u00bf", ctx),
        "Basic Latin punctuation");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "Basic Latin letters: "
        "\u00c0\u00c1\u00c2\u00c3\u00c4\u00c5\u00c6\u00c7\u00c8\u00c9"
        "\u00ca\u00cb\u00cc\u00cd\u00ce\u00cf\u00d0\u00d1\u00d2\u00d3"
        "\u00d4\u00d5\u00d6\u00d7\u00d8\u00d9\u00da\u00db\u00dc\u00dd"
        "\u00de\u00df\u00e0\u00e1\u00e2\u00e3\u00e4\u00e5\u00e6\u00e7"
        "\u00e8\u00e9\u00ea\u00eb\u00ec\u00ed\u00ee\u00ef\u00f0\u00f1"
        "\u00f2\u00f3\u00f4\u00f5\u00f6\u00f7\u00f8\u00f9\u00fa\u00fb"
        "\u00fc\u00fd\u00fe\u00ff", ctx),
        "Basic Latin letters_ "
        "AAAAAAECEE"
        "EEIIIIDNOO"
        "OOO_OUUUUY"
        "Psaaaaaaec"
        "eeeeiiiion"
        "ooooo_ouuu"
        "uypy");
    BOOST_CHECK_EQUAL(convert_for_filesystem(
        "Latin-1 Supplement, some of: "
        "\u0179\u017a\u017b\u017c\u017d\u017e\u017f", ctx),
        "Latin-1 Supplement, some of_ ");
}
