/* __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
//
// Copyright (C) 1993-2008, ART+COM AG Berlin, Germany <www.artcom.de>
//
// These coded instructions, statements, and computer programs contain
// proprietary information of ART+COM AG Berlin, and are copy protected
// by law. They may be used, modified and redistributed under the terms
// of GNU General Public License referenced below.
//
// Alternative licensing without the obligations of the GPL is
// available upon request.
//
// GPL v3 Licensing:
//
// This file is part of the ART+COM Y60 Platform.
//
// ART+COM Y60 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// ART+COM Y60 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with ART+COM Y60.  If not, see <http://www.gnu.org/licenses/>.
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/

#include "VLC.h"
#include <asl/base/Logger.h>
#include <asl/base/os_functions.h>
#include <asl/base/file_functions.h>
#include <asl/base/string_functions.h>
#include <asl/base/Auto.h>

#include <y60/video/Movie.h>
#include <iostream>

using namespace std;
using namespace asl;

extern "C"
EXPORT asl::PlugInBase * y60vlc_instantiatePlugIn(asl::DLHandle myDLHandle) {
    return new y60::VLC(myDLHandle);
}
    
namespace y60 {

    const std::string VLC::MIME_TYPE = "application/x-vlc-plugin";


    VLC::VLC(asl::DLHandle theDLHandle) 
        : CaptureDevice(),
        PlugInBase(theDLHandle),
        _curBuffer(NULL),
        _mediaPlayer(NULL),
        _playTime(0),
        _libvlc(NULL),
        _EOF(false)
    {
        char const *vlc_argv[] =
        {
            "--no-osd",
            //"-vvv",
            "--reset-plugins-cache",
            "--no-xlib" // tell VLC to not use Xlib
        };
        int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);
        _libvlc = libvlc_new(vlc_argc, vlc_argv);
        _mediaPlayer = libvlc_media_player_new(_libvlc);
        libvlc_event_manager_t *eventManager = libvlc_media_player_event_manager(_mediaPlayer);
        libvlc_event_attach(eventManager, libvlc_MediaPlayerEndReached, VLC::handle_vlc_event, this);
    }

    VLC::~VLC() {
        if (_mediaPlayer) {
            libvlc_event_manager_t *eventManager = libvlc_media_player_event_manager(_mediaPlayer);
            libvlc_event_detach(eventManager, libvlc_MediaPlayerEndReached, VLC::handle_vlc_event, this);
            libvlc_media_player_stop(_mediaPlayer);
            libvlc_media_player_release(_mediaPlayer);
            _mediaPlayer = NULL;
        }
        if (_libvlc) {
            libvlc_release(_libvlc);
            _libvlc = NULL;
        }
        if (_curBuffer) {
            delete _curBuffer;
        }
        AC_DEBUG << "deleting VLC instance";
    }

    asl::Ptr<CaptureDevice> VLC::instance() const {
        AC_DEBUG << "creating new VLC instance";
        return asl::Ptr<CaptureDevice>(new VLC(getDLHandle()));
    }

    void
    VLC::readFrame(dom::ResizeableRasterPtr theTargetRaster) {
        ScopeLocker myFrameLock(_myFrameLock, true);
        if (_curBuffer == NULL) {
            return;
        }
		if (getFrameWidth() != _myFrameWidth) setFrameWidth(_myFrameWidth);
		if (getFrameHeight() != _myFrameHeight) setFrameHeight(_myFrameHeight);
        theTargetRaster->resize(getFrameWidth(), getFrameHeight());
        std::copy(_curBuffer->begin(), _curBuffer->end(), theTargetRaster->pixels().begin());
        
        libvlc_time_t curTimeCode = libvlc_media_player_get_time(_mediaPlayer);
        AC_TRACE << "-- timestamp from vlc: " << curTimeCode;
        setTimeCode(as_string(curTimeCode));

        // delete the current buffer, marking that we already copied it to the texture
        AC_TRACE << "freeing " << _curBuffer->size() << " bytes after copying to texture.";
        delete _curBuffer;
        _curBuffer = NULL;
    }

    std::string
    VLC::canDecode(const std::string & theUrl, asl::Ptr<asl::ReadableStreamHandle> theStream) {
        std::string::size_type schemaDelimiter = theUrl.find(":");
        if (schemaDelimiter != std::string::npos) {
            std::string urlSchema = theUrl.substr(0, schemaDelimiter);
            if (urlSchema == "file" || urlSchema == "rtp" || urlSchema == "http") {
                return MIME_TYPE;
            }
        }

        AC_WARNING << "VLC can not decode :" << theUrl << endl;
        return "";
    }
    
    void
    VLC::load(const std::string & theFilename) {

        AC_DEBUG << "VLC::load('" << theFilename << "')";

        _EOF = false;
        _playTime = 0;
        
        std::vector<std::string> elements = asl::splitString(theFilename, "#");
        if (elements.size() == 2) {
            AC_DEBUG << "parsed playback position at " << _playTime << " milliseconds.";
            _playTime = as<asl::Unsigned64>(elements[1]);
            
        }
        
        _mediaURL = elements[0];
        libvlc_media_t *media = libvlc_media_new_location(_libvlc, _mediaURL.c_str());
        libvlc_media_player_set_media(_mediaPlayer, media);
        libvlc_media_release(media);

        libvlc_video_set_callbacks(_mediaPlayer, VLC::lock, VLC::unlock, VLC::display, this);
        _rasterEncoding = BGR;
        setPixelFormat(_rasterEncoding);
        libvlc_video_set_format_callbacks(_mediaPlayer, VLC::setup_video, VLC::cleanup_video);

        libvlc_media_player_play(_mediaPlayer);
        libvlc_media_player_set_time(_mediaPlayer, _playTime);
    }

    void
    VLC::unload() {
        AC_DEBUG << "VLC::unload()";
    }

    unsigned  
    VLC::setup_video(char * chroma, unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines) {
        AC_DEBUG << "VLC requesting " << chroma << " " << *width << "x" << *height << " for " << _mediaURL;

        // TODO: we could use our YUV shader here
        AC_INFO << "Can't render " << chroma << ", asking libvlc to convert to RGB";
        memcpy(chroma, "RV24", 4);  
        _rasterEncoding = BGR;
        
        _myFrameWidth = *width;
        _myFrameHeight = *height;
        (*pitches) = getBytesRequired(_myFrameWidth, _rasterEncoding);
        (*lines) = _myFrameHeight;
        
        setPixelFormat(_rasterEncoding);
        setFrameHeight(_myFrameHeight);
        setFrameWidth(_myFrameWidth);

        return 1; // one color plane
    }

    void *
    VLC::lock(void ** pixels) {
        AC_TRACE << "lock " << _mediaURL;
        Block * newBuffer = new Block(getBytesRequired(_myFrameWidth, _rasterEncoding)* _myFrameHeight);
        AC_TRACE << "allocated " << newBuffer->size() << " bytes";
        *pixels = newBuffer->begin();
        return newBuffer; 
    };

    void 
    VLC::unlock(asl::Block* oldBuffer, void * const * pixels) { 
        AC_TRACE << "unlock " << _mediaURL;
        return; 
    };
    void VLC::display(Block* nextBuffer) {
        AC_TRACE << "display " << _mediaURL;

        ScopeLocker myFrameLock(_myFrameLock, true);
        if (_curBuffer) {
            AC_TRACE << "-- freeing " << _curBuffer->size() << " bytes after discarding frame.";
            delete _curBuffer;
        }
        _curBuffer = nextBuffer;
        return;
    };

   void 
    VLC::stopCapture() {
        AC_DEBUG << "stop capture";
        if (_mediaPlayer) {
            libvlc_media_player_stop(_mediaPlayer);
            _EOF = false;
        }
    };

    void
    VLC::startCapture() {
        AC_DEBUG << "start capture";
        if (_mediaPlayer) {
            _EOF = false;
            
            libvlc_media_player_play(_mediaPlayer);
            
            libvlc_state_t state = libvlc_media_player_get_state(_mediaPlayer);
            if (state != libvlc_Paused) {
                AC_DEBUG << "seeking to playback position at " << _playTime << " milliseconds.";
                libvlc_media_player_set_time(_mediaPlayer, _playTime);    
            }
        }
    };

    void
    VLC::pauseCapture() {
        AC_DEBUG << "pause capture";
        if (_mediaPlayer) {
            libvlc_media_player_set_pause(_mediaPlayer, 1);
        }
    };

    void
    VLC::handle_vlc_event(const struct libvlc_event_t *vlc_event) {
        if (vlc_event->type == libvlc_MediaPlayerEndReached) {
            AC_DEBUG << "handling vlc event: libvlc_MediaPlayerEndReached " << vlc_event->type;
            _EOF = true;
        }
    };
}
