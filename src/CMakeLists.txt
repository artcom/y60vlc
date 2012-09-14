
set(SOURCES
    VLC.cpp 
)

set(HEADERS
    VLC.h
)

if(NOT WIN32)
    find_package(Boost COMPONENTS system thread)
endif(NOT WIN32)

set(EXTERNALS Boost LIBVLC)

y60_add_component(
    y60vlc 
    SOURCES ${SOURCES}
    HEADERS ${HEADERS}
    DEPENDS y60video aslaudio
    EXTERNS ${EXTERNALS}
)

