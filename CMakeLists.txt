cmake_minimum_required(VERSION 2.6.2)

project(Y60VLCPlugin)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 2)
set(VERSION_PATCH 0)
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

find_package(Y60)

add_definitions(-DAC_BUILT_WITH_CMAKE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(PkgConfig)
pkg_check_modules(PC_LIBVLC vlc)
set(LIBVLC_DEFINITIONS ${PC_LIBVLC_CFLAGS_OTHER})


find_path(LIBVLC_INCLUDE_DIR vlc/vlc.h
    HINTS ${PC_LIBVLC_INCLUDEDIR} ${PC_LIBVLC_INCLUDE_DIRS} $ENV{VLC_DIR}/sdk/include
          PATH_SUFFIXES libvlc )


find_library(LIBVLC_LIBRARY NAMES vlc libvlc
    HINTS ${PC_LIBVLC_LIBDIR} ${PC_LIBVLC_LIBRARY_DIRS} $ENV{VLC_DIR}/sdk/lib )

         set(LIBVLC_LIBRARIES ${LIBVLC_LIBRARY} )
         set(LIBVLC_INCLUDE_DIRS ${LIBVLC_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBVLC_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LIBVLC DEFAULT_MSG
    LIBVLC_LIBRARY LIBVLC_INCLUDE_DIR)

mark_as_advanced(LIBVLC_INCLUDE_DIR LIBVLC_LIBRARY )

ac_begin_solution(
    Y60VLCPlugin
    VERSION_MAJOR ${VERSION_MAJOR} VERSION_MINOR ${VERSION_MINOR} VERSION_PATCH  ${VERSION_PATCH}
    VENDOR "ART+COM AG"
    CONTACT "info@artcom.de"
    DESCRIPTION "y60 libvlc component"

    DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/DESCRIPTION.txt"
    LICENSE_FILE     "${CMAKE_SOURCE_DIR}/LICENSE.txt"
    PACKAGE_ICON     "${CMAKE_SOURCE_DIR}/installer/windows/NSIS-Header.bmp"
    INSTALLER_ICON   "${CMAKE_SOURCE_DIR}/installer/windows/shopping_bag.ico"
    UNINSTALLER_ICON "${CMAKE_SOURCE_DIR}/installer/windows/shopping_bag_grey.ico"
    CREATE_SHORTCUTS FALSE
)

add_subdirectory(src)

ac_end_solution(Y60VLCPlugin)
