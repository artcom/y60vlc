cmake_minimum_required(VERSION 2.6.2)

project(Y60VLC)

find_package(Y60)

add_definitions(-DAC_BUILT_WITH_CMAKE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(PkgConfig)
pkg_check_modules(PC_LIBVLC vlc)
set(LIBVLC_DEFINITIONS ${PC_LIBVLC_CFLAGS_OTHER})


find_path(LIBVLC_INCLUDE_DIR vlc/vlc.h
    HINTS ${PC_LIBVLC_INCLUDEDIR} ${PC_LIBVLC_INCLUDE_DIRS}
          PATH_SUFFIXES libvlc )


find_library(LIBVLC_LIBRARY NAMES vlc
    HINTS ${PC_LIBVLC_LIBDIR} ${PC_LIBVLC_LIBRARY_DIRS} )

         set(LIBVLC_LIBRARIES ${LIBVLC_LIBRARY} )
         set(LIBVLC_INCLUDE_DIRS ${LIBVLC_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBVLC_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LIBVLC DEFAULT_MSG
    LIBVLC_LIBRARY LIBVLC_INCLUDE_DIR)

mark_as_advanced(LIBVLC_INCLUDE_DIR LIBVLC_LIBRARY )

ac_add_project(
    Y60VLC
	VERSION_MAJOR 1 VERSION_MINOR 2 VERSION_PATCH 0
    VENDOR "ART+COM AG"
	CONTACT "info@artcom.de"
	DESCRIPTION "y60 libvnc component"
)

add_subdirectory(src)

