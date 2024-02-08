cmake_minimum_required(VERSION 3.16)

project(
        musicmove
        VERSION 1.1.0
        DESCRIPTION "Rename and move music files into a structure that aligns with their metadata tags"
)
set(PROJECT_HOMEPAGE_URL "https://github.com/xsco/musicmove")
set(PROJECT_BUGREPORT_URL "https://github.com/xsco/musicmove/issues")

# Include additional modules.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Require C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
if (MSVC)
    add_compile_options("/W4" "/wd4251" "/wd4275" "$<$<CONFIG:RELEASE>:/O2>")

    # Ask MSVC to populate the __cplusplus macro properly.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus")
else ()
    add_compile_options("-Wall" "$<$<CONFIG:RELEASE>:-O3>")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options("-stdlib=libc++")
    endif ()
endif ()

# Require boost program_options, filesystem, locale
find_package(
        Boost 1.71
        REQUIRED
        COMPONENTS program_options system filesystem locale unit_test_framework)

# Require taglib 1.x
find_package(Taglib 1.11.1 REQUIRED)

# Require chaiscript
include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(chaiscript/chaiscript.hpp ChaiScript_FOUND)
if (!ChaiScript_FOUND)
    message(FATAL_ERROR "Cannot find <chaiscript/chaiscript.hpp>, which is a required dependency")
endif ()

set(PACKAGE ${CMAKE_PROJECT_NAME})
set(PACKAGE_STRING ${CMAKE_PROJECT_NAME})
set(PACKAGE_BUGREPORT ${PROJECT_BUGREPORT_URL})
configure_file(src/config.hpp.in src/config.hpp)

add_compile_definitions(PATH_CONVERSION_DEFAULT_VALUE="windows-ascii")

add_library(
        libmusicmove
        STATIC
        src/context.hpp
        src/format.cpp
        src/format.hpp
        src/format_easytag.cpp
        src/metadata.cpp
        src/metadata.hpp
        src/metadata_base.hpp
        src/metadata_flac.hpp
        src/metadata_mp4.hpp
        src/metadata_mpeg.hpp
        src/metadata_ogg_vorbis.hpp
        src/move.cpp
        src/move.hpp
        src/script_runner.cpp
        src/script_runner.hpp
)
target_include_directories(libmusicmove PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/src")
target_link_libraries(
        libmusicmove
        Boost::program_options Boost::system Boost::filesystem Boost::locale
        Taglib::Taglib
)

add_executable(
        musicmove
        src/musicmove.cpp
)
target_link_libraries(
        musicmove
        libmusicmove
)

# man page generation
add_custom_command(
        TARGET musicmove
        POST_BUILD
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/musicmove.1
        COMMAND help2man ${CMAKE_CURRENT_BINARY_DIR}/musicmove > ${CMAKE_CURRENT_BINARY_DIR}/musicmove.1
)

include(CTest)

add_executable(
        test_metadata
        src/metadata_test.cpp
        src/metadata.cpp
)
target_compile_definitions(test_metadata PUBLIC -DTESTDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR}/testdata)
target_link_libraries(test_metadata PUBLIC libmusicmove Boost::unit_test_framework)
add_test(NAME test_metadata COMMAND test_metadata)

add_executable(
        test_format
        src/format_test.cpp
        src/metadata_mock.cpp
        src/format.cpp
        src/format_easytag.cpp
        src/format.hpp)
target_compile_definitions(test_format PUBLIC -DTESTDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR}/testdata)
target_link_libraries(test_format PUBLIC libmusicmove Boost::unit_test_framework)
add_test(NAME test_format COMMAND test_format)

add_executable(
        test_script_runner
        src/script_runner_test.cpp
        src/metadata_mock.cpp)
target_compile_definitions(test_script_runner PUBLIC -DTESTDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR}/testdata)
target_link_libraries(test_script_runner PUBLIC libmusicmove Boost::unit_test_framework)
add_test(NAME test_script_runner COMMAND test_script_runner)

add_executable(
        test_move
        src/move_test.cpp
        src/format_mock.cpp
        src/metadata_mock.cpp)
target_compile_definitions(test_move PUBLIC -DTESTDATA_DIR=${CMAKE_CURRENT_SOURCE_DIR}/testdata)
target_link_libraries(test_move PUBLIC libmusicmove Boost::unit_test_framework)
add_test(NAME test_move COMMAND test_move)

# Install stage
install(TARGETS musicmove)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/musicmove.1 DESTINATION ${CMAKE_INSTALL_PREFIX}/man/man1)
