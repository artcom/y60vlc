# This file will be configured to contain variables for CPack. These variables
# should be set in the CMake list file of the project before CPack module is
# included. The list of available CPACK_xxx variables and their associated
# documentation may be obtained using
#  cpack --help-variable-list
#
# Some variables are common to all generators (e.g. CPACK_PACKAGE_NAME)
# and some are specific to a generator
# (e.g. CPACK_NSIS_EXTRA_INSTALL_COMMANDS). The generator specific variables
# usually begin with CPACK_<GENNAME>_xxxx.


SET(CPACK_BINARY_BUNDLE "OFF")
SET(CPACK_BINARY_CYGWIN "")
SET(CPACK_BINARY_DEB "OFF")
SET(CPACK_BINARY_DRAGNDROP "OFF")
SET(CPACK_BINARY_NSIS "OFF")
SET(CPACK_BINARY_OSXX11 "OFF")
SET(CPACK_BINARY_PACKAGEMAKER "ON")
SET(CPACK_BINARY_RPM "OFF")
SET(CPACK_BINARY_STGZ "ON")
SET(CPACK_BINARY_TBZ2 "OFF")
SET(CPACK_BINARY_TGZ "ON")
SET(CPACK_BINARY_TZ "")
SET(CPACK_BINARY_WIX "")
SET(CPACK_BINARY_ZIP "")
SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_COMPONENTS_ALL "Unspecified;Y60VLCPlugin_development")
SET(CPACK_COMPONENT_UNSPECIFIED_HIDDEN "TRUE")
SET(CPACK_COMPONENT_UNSPECIFIED_REQUIRED "TRUE")
SET(CPACK_GENERATOR "PackageMaker;STGZ;TGZ")
SET(CPACK_INSTALL_CMAKE_PROJECTS "/Users/mirko/Development/Samples/y60vlc;Y60VLCPlugin;ALL;/")
SET(CPACK_INSTALL_PREFIX "/usr/local")
SET(CPACK_MODULE_PATH "/usr/local/lib/spidermonkey/cmake;/usr/local/lib/acmake/cmake;/usr/local/lib/oscpack/cmake;/usr/local/lib/acmake/cmake;/usr/local/lib/acsdlttf/cmake;/usr/local/lib/acmake/cmake;/usr/local/lib/paintlib/cmake;/usr/local/lib/acmake/cmake;/usr/local/lib/asl/cmake;/usr/local/lib/acmake/cmake;/usr/local/lib/y60/cmake;/usr/local/lib/acmake/cmake;/usr/local/lib/acmake/cmake")
SET(CPACK_NSIS_DISPLAY_NAME "Y60VLCPlugin")
SET(CPACK_NSIS_INSTALLER_ICON_CODE "")
SET(CPACK_NSIS_INSTALLER_MUI_ICON_CODE "")
SET(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
SET(CPACK_NSIS_PACKAGE_NAME "Y60VLCPlugin")
SET(CPACK_OUTPUT_CONFIG_FILE "/Users/mirko/Development/Samples/y60vlc/CPackConfig.cmake")
SET(CPACK_PACKAGE_CONTACT "info@artcom.de")
SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "/Users/mirko/Development/Samples/y60vlc/DESCRIPTION.txt")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "y60 libvlc component")
SET(CPACK_PACKAGE_FILE_NAME "Y60VLCPlugin-1.2.0-Darwin")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "Y60VLCPlugin")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Y60VLCPlugin")
SET(CPACK_PACKAGE_NAME "Y60VLCPlugin")
SET(CPACK_PACKAGE_RELOCATABLE "true")
SET(CPACK_PACKAGE_VENDOR "ART+COM AG")
SET(CPACK_PACKAGE_VERSION "1.2.0")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "2")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_RESOURCE_FILE_LICENSE "/Users/mirko/Development/Samples/y60vlc/LICENSE.txt")
SET(CPACK_RESOURCE_FILE_README "/usr/local/Cellar/cmake/2.8.12/share/cmake/Templates/CPack.GenericDescription.txt")
SET(CPACK_RESOURCE_FILE_WELCOME "/usr/local/Cellar/cmake/2.8.12/share/cmake/Templates/CPack.GenericWelcome.txt")
SET(CPACK_SET_DESTDIR "OFF")
SET(CPACK_SOURCE_CYGWIN "")
SET(CPACK_SOURCE_GENERATOR "TGZ;TBZ2;TZ")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "/Users/mirko/Development/Samples/y60vlc/CPackSourceConfig.cmake")
SET(CPACK_SOURCE_TBZ2 "ON")
SET(CPACK_SOURCE_TGZ "ON")
SET(CPACK_SOURCE_TZ "ON")
SET(CPACK_SOURCE_ZIP "OFF")
SET(CPACK_SYSTEM_NAME "Darwin")
SET(CPACK_TOPLEVEL_TAG "Darwin")
SET(CPACK_WIX_SIZEOF_VOID_P "8")

# Configuration for component "y60vlc"

SET(CPACK_COMPONENTS_ALL Unspecified Y60VLCPlugin_development)
set(CPACK_COMPONENT_Y60VLC_DESCRIPTION "y60vlc runtime")
set(CPACK_COMPONENT_Y60VLC_GROUP y60_components_runtime)

# Configuration for component "y60vlc_development"

SET(CPACK_COMPONENTS_ALL Unspecified Y60VLCPlugin_development)
set(CPACK_COMPONENT_Y60VLC_DEVELOPMENT_DESCRIPTION "y60vlc header files")
set(CPACK_COMPONENT_Y60VLC_DEVELOPMENT_GROUP y60_components_development)
