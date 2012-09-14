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
#include <y60/video/AsyncDecoder.h>

#include <vlc/vlc.h>

namespace y60 {
    /*! @addtogroup Y60componentsVLC */
/*  @{ */
    const std::string MIME_TYPE_MPG = "application/mpg";
    class VLC : 
        public AsyncDecoder,
        public asl::PlugInBase  
    {
    public:
        VLC(asl::DLHandle theDLHandle);
        ~VLC();
        virtual asl::Ptr<MovieDecoderBase> instance() const;
        std::string canDecode(const std::string & theUrl, asl::Ptr<asl::ReadableStreamHandle> theStream = asl::Ptr<asl::ReadableStreamHandle>());
        /**
         * loads a movie from the file given by theFilename
         * @param theFilename file to load into the decoder
         */
        void load(const std::string & theFilename);
        double readFrame(double theTime, unsigned, RasterVector theTargetRaster);

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
        void setupVideo();
        int _myFrameWidth;
        int _myFrameHeight;

        libvlc_instance_t *libvlc;
        libvlc_media_t *m;
        libvlc_media_player_t *mp;
        
    };
/* @} */
}

#endif // _ac_VLC_h_
