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
//
// Description: TODO
//
// Last Review: NEVER, NOONE
//
//  review status report: (perfect, ok, fair, poor, disaster, notapplicable, unknown)
//    usefullness            : unknown
//    formatting             : unknown
//    documentation          : unknown
//    test coverage          : unknown
//    names                  : unknown
//    style guide conformance: unknown
//    technical soundness    : unknown
//    dead code              : unknown
//    readability            : unknown
//    understandabilty       : unknown
//    interfaces             : unknown
//    confidence             : unknown
//    integration            : unknown
//    dependencies           : unknown
//    cheesyness             : unknown
//
//    overall review status  : unknown
//
//    recommendations:
//       - unknown
// __ ___ ____ _____ ______ _______ ________ _______ ______ _____ ____ ___ __
*/

#ifndef _ac_VLC_h_
#define _ac_VLC_h_


#include <asl/base/settings.h>
#include <asl/base/PlugInBase.h>
#include <asl/base/ReadWriteLock.h>
#include <y60/video/CaptureDevice.h>

#include <vlc/vlc.h>

namespace y60 {
    /*! @addtogroup Y60componentsVLC */
/*  @{ */
    class VLC : 
        public CaptureDevice,
        public asl::PlugInBase  
    {
    public:
        static const std::string MIME_TYPE;
        VLC(asl::DLHandle theDLHandle);
        ~VLC();
        virtual asl::Ptr<CaptureDevice> instance() const;
        std::string canDecode(const std::string & theUrl, asl::Ptr<asl::ReadableStreamHandle> theStream = asl::Ptr<asl::ReadableStreamHandle>());
        /**
         * loads a movie from the file given by theFilename
         * @param theFilename file to load into the decoder
         */
        void load(const std::string & theFilename);
        void readFrame(dom::ResizeableRasterPtr theTargetRaster);

        /**
         * Starts movie decoding
         * @param theStartTime movie-time to start decoding at.
         */
        // void startMovie(double theStartTime = 0.0f, bool theStartAudioFlag = true);

        //void resumeMovie(double theStartTime = 0.0f, bool theResumeAudioFlag = true);

        /**
         * Called to stop the decoding.
         */
        // void stopMovie(bool theStopAudioFlag = true);
        // void closeMovie();

        const char * getName() const { return "y60vlc"; }

        // void shutdown();

	    DEFINE_EXCEPTION(Exception, asl::Exception);

     private:
        asl::ReadWriteLock _myFrameLock;
        // callbacks & static dispatchers
        void *lock(void ** pixels);
        static void * lock(void* self, void ** pixels) { return reinterpret_cast<VLC*>(self)->lock(pixels); };

        void unlock(void* id, void * const * pixels);
        static void unlock(void* self, void* id, void * const * pixels) { reinterpret_cast<VLC*>(self)->unlock(id, pixels); }; 
        
        void display(void* id);
        static void display(void* self, void* id) { reinterpret_cast<VLC*>(self)->display(id); };

        unsigned setup_video(char * chroma, unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines);
        static unsigned setup_video(void** self, char * chroma, unsigned *width, unsigned *height, unsigned *pitches, unsigned *lines) {
            return reinterpret_cast<VLC*>(*self)->setup_video(chroma, width, height, pitches, lines);
        }

        void cleanup_video() {};
        static void cleanup_video(void* self) {
            static_cast<VLC*>(self)->cleanup_video();
        }

        std::string _mediaURL;
        int _myFrameWidth;
        int _myFrameHeight;

        PixelEncoding _rasterEncoding;
        asl::Block * _curBuffer;
        libvlc_instance_t * _libvlc;
        libvlc_media_player_t * _mp;
        
    };
/* @} */
}

#endif // _ac_VLC_h_
