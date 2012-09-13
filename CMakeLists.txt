cmake_minimum_required(VERSION 2.6.2)

project(Y60VLC)

find_package(Y60)

add_definitions(-DAC_BUILT_WITH_CMAKE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# define dependencies from local directory
#set(VLC_LIB_NAME "CCBlueBoxSDK.lib")
#set(CCBLUEBOX_DEFAULT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/Dependencies")

#if(NOT CCBLUEBOX_ROOT)
#    set(CCBLUEBOX_ROOT ${CCBLUEBOX_DEFAULT_ROOT})
#endif(NOT CCBLUEBOX_ROOT)

#FIND_PATH(CCBLUEBOX_INCLUDE_DIR 
#        ICCViewer.h 
#    PATHS
#        ${CCBLUEBOX_ROOT}/include
#    )

#set(CCBLUEBOX_INCLUDE_DIRS ${CCBLUEBOX_INCLUDE_DIR})

#mark_as_advanced(CCBLUEBOX_DEFINITIONS CCBLUEBOX_INCLUDE_DIR CCBLUEBOX_INCLUDE_DIRS)

#find_library(CCBLUEBOX_LIBRARIES NAMES ${CCBLUEBOX_LIB_NAME} 
#    PATH_SUFFIXES
#        libs
#    PATHS
#        ${CCBLUEBOX_ROOT}
#    )

#include(FindPackageHandleStandardArgs)
#find_package_handle_standard_args(
#        CCBLUEBOX DEFAULT_MSG
#        CCBLUEBOX_LIBRARIES CCBLUEBOX_INCLUDE_DIR
#)

#set(CCBB_DISPLAY_NAME "CanControls_BlueBox_Component")

ac_add_project(
    Y60VLC
	VERSION_MAJOR 1 VERSION_MINOR 2 VERSION_PATCH 0
    VENDOR "ART+COM AG"
	CONTACT "info@artcom.de"
	DESCRIPTION "y60 libvnc component"
)

add_subdirectory(src)

