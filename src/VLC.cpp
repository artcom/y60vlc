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

    VLC::VLC(asl::DLHandle theDLHandle) 
        : AsyncDecoder(),
        PlugInBase(theDLHandle)
    {
    }

    VLC::~VLC() {
    }

    asl::Ptr<MovieDecoderBase> VLC::instance() const {
        return asl::Ptr<MovieDecoderBase>(new VLC(getDLHandle()));
    }

    double
    VLC::readFrame(double theTime, unsigned /*theFrame*/, RasterVector theTargetRaster)
    {
        AC_TRACE << "readFrame, Time wanted=" << theTime;
        //ASSURE(!getEOF());
        if (getEOF()) {
            return theTime;
        }
        if (getPlayMode() == y60::PLAY_MODE_STOP) {
            AC_DEBUG << "readFrame: not playing.";
            return theTime;
        }
        return theTime;
    }
    std::string
    VLC::canDecode(const std::string & theUrl, asl::Ptr<asl::ReadableStreamHandle> theStream) {
        // assume that ffmpeg can decode everything, except y60 formats
        if (asl::toLowerCase(asl::getExtension(theUrl)) != "m60" &&
            asl::toLowerCase(asl::getExtension(theUrl)) != "x60" &&
            asl::toLowerCase(asl::getExtension(theUrl)) != "d60" &&
            asl::toLowerCase(asl::getExtension(theUrl)) != "i60" )
        {
            AC_DEBUG << "VLC can decode :" << theUrl << endl;
            return MIME_TYPE_MPG;
        } else {
            AC_DEBUG << "VLC can not decode :" << theUrl << endl;
            return "";
        }
    }
    
    void
    VLC::load(const std::string & theFilename) {
        AC_DEBUG << "VLC::load(" << theFilename << ")";

        setupVideo();
        
    }
    void 
    VLC::setupVideo() {
        AC_DEBUG << "VLC::setupVideo";
        // AVCodecContext * myVCodec = _myVStream->codec;
        // _myVideoStartTimestamp = -1; // used as flag, we use the dts of the first decoded frame

        Movie * myMovie = getMovie();
        AC_DEBUG << "PF=" << myMovie->get<RasterPixelFormatTag>();

        PixelEncoding myRasterEncoding = PixelEncoding(getEnumFromString(myMovie->get<RasterPixelFormatTag>(),
                                                        PixelEncodingString));

        // TargetPixelFormatTag is the format the incoming movieframe will be converted in
        if (myMovie->get<TargetPixelFormatTag>() != "") {
            TextureInternalFormat myTargetPixelFormat = TextureInternalFormat(getEnumFromString(myMovie->get<TargetPixelFormatTag>(), TextureInternalFormatStrings));
            switch(myTargetPixelFormat) {
                case TEXTURE_IFMT_YUVA420:
                    myRasterEncoding = YUVA420;
                    break;
                case TEXTURE_IFMT_YUV420:
                    myRasterEncoding = YUV420;
                    break;
                case TEXTURE_IFMT_YUV422:
                    myRasterEncoding = YUV422;
                    break;
                case TEXTURE_IFMT_YUV444:
                    myRasterEncoding = YUV444;
                    break;
                case TEXTURE_IFMT_RGBA8:
                    myRasterEncoding = RGBA;
                    break;
                case TEXTURE_IFMT_ALPHA:
                    myRasterEncoding = ALPHA;
                    break;

                case TEXTURE_IFMT_LUMINANCE:
                case TEXTURE_IFMT_LUMINANCE8:
                case TEXTURE_IFMT_LUMINANCE16:
                case TEXTURE_IFMT_INTENSITY:
                    myRasterEncoding = GRAY;
                    break;
                case TEXTURE_IFMT_RGB:
                    myRasterEncoding = RGB;
                    break;
                default:
                    AC_FATAL << "Unsupported pixel format " << myMovie->get<TargetPixelFormatTag>() << " in FFMpegDecoder2";
                    break;
            }
        }

        // Setup size and image matrix
        _myFrameWidth = 800;
        _myFrameHeight = 600;


        switch (myRasterEncoding) {
            case RGBA:
                {AC_DEBUG << "Using TEXTURE_IFMT_RGBA8 pixels";}
                //_myDestinationPixelFormat = PIX_FMT_BGRA;
                //_myBytesPerPixel = 4;
                myMovie->createRaster(_myFrameWidth, _myFrameHeight, 1, y60::BGRA);
                break;
            case ALPHA:
                {AC_DEBUG << "Using Alpha pixels";}
                //_myDestinationPixelFormat = PIX_FMT_GRAY8;
                //_myBytesPerPixel = 1;
                myMovie->createRaster(_myFrameWidth, _myFrameHeight, 1, y60::ALPHA);
                break;
            case GRAY:
                {AC_DEBUG << "Using GRAY pixels";}
                //_myDestinationPixelFormat = PIX_FMT_GRAY8;
                //_myBytesPerPixel = 1;
                myMovie->createRaster(_myFrameWidth, _myFrameHeight, 1, y60::GRAY);
                break;
            case YUV420:
                {AC_DEBUG << "Using YUV420 pixels";}
                //_myDestinationPixelFormat = PIX_FMT_YUV420P;
                /*
                if (myVCodec->pix_fmt != PIX_FMT_YUV420P) {
                    AC_WARNING<<"you're trying to use YUV2RGB shader but the source video pixel format is not YUV420p, src: " + theFilename;
                }
                */
                myMovie->createRaster(_myFrameWidth, _myFrameHeight, 1, y60::GRAY);
                myMovie->addRasterValue(createRasterValue( y60::GRAY, _myFrameWidth/2, _myFrameHeight/2), y60::GRAY, 1);
                myMovie->addRasterValue(createRasterValue( y60::GRAY, _myFrameWidth/2, _myFrameHeight/2), y60::GRAY, 1);
                break;
            case YUVA420:
                {AC_DEBUG << "Using YUVA420 pixels";}
                //_myDestinationPixelFormat = PIX_FMT_YUVA420P;
                /*
                if (myVCodec->pix_fmt != PIX_FMT_YUVA420P) {
                    AC_WARNING<<"you're trying to use YUV2RGB shader but the source video pixel format is not YUVA420p, src: " + theFilename;
                }
                */
                myMovie->createRaster(_myFrameWidth, _myFrameHeight, 1, y60::GRAY);
                myMovie->addRasterValue(createRasterValue( y60::GRAY, _myFrameWidth/2, _myFrameHeight/2), y60::GRAY, 1);
                myMovie->addRasterValue(createRasterValue( y60::GRAY, _myFrameWidth/2, _myFrameHeight/2), y60::GRAY, 1);
                myMovie->addRasterValue(createRasterValue( y60::GRAY, _myFrameWidth, _myFrameHeight), y60::GRAY, 1);
                break;
            case YUV422:
                {AC_DEBUG << "Using YUV422 pixels";}
                //_myDestinationPixelFormat = PIX_FMT_YUV422P;
                /*
                if (myVCodec->pix_fmt != PIX_FMT_YUV422P) {
                    AC_WARNING<<"you're trying to use YUV2RGB shader but the source video pixel format is not YUV422p, src: " + theFilename;
                }
                */
                myMovie->createRaster(_myFrameWidth, _myFrameHeight, 1, y60::GRAY);
                myMovie->addRasterValue(createRasterValue( y60::GRAY, _myFrameWidth, _myFrameHeight/2), y60::GRAY, 1);
                myMovie->addRasterValue(createRasterValue( y60::GRAY, _myFrameWidth, _myFrameHeight/2), y60::GRAY, 1);
                break;
            case YUV444:
                {AC_DEBUG << "Using YUV444 pixels";}
                //_myDestinationPixelFormat = PIX_FMT_YUV444P;
                /*
                if (myVCodec->pix_fmt != PIX_FMT_YUV444P) {
                    AC_WARNING<<"you're trying to use YUV2RGB shader but the source video pixel format is not YUV444p, src: " + theFilename;
                }
                */
                myMovie->createRaster(_myFrameWidth, _myFrameHeight, 1, y60::GRAY);
                myMovie->addRasterValue(createRasterValue( y60::GRAY, _myFrameWidth, _myFrameHeight), y60::GRAY, 1);
                myMovie->addRasterValue(createRasterValue( y60::GRAY, _myFrameWidth, _myFrameHeight), y60::GRAY, 1);
                break;
            case RGB:
            default:
                {AC_DEBUG << "Using BGR pixels";}
                //_myDestinationPixelFormat = PIX_FMT_BGR24;
                //_myBytesPerPixel = 3;
                myMovie->createRaster(_myFrameWidth, _myFrameHeight, 1, y60::BGR);
                break;
        }
        unsigned myRasterCount = myMovie->getNode().childNodesLength();
        /*
        if (_myDestinationPixelFormat == PIX_FMT_YUV420P
            || _myDestinationPixelFormat == PIX_FMT_YUV422P
            || _myDestinationPixelFormat == PIX_FMT_YUV444P)
        {
            memset(myMovie->getRasterPtr(0)->pixels().begin(), 16, myMovie->getRasterPtr(0)->pixels().size());
            memset(myMovie->getRasterPtr(1)->pixels().begin(), 127, myMovie->getRasterPtr(1)->pixels().size());
            memset(myMovie->getRasterPtr(2)->pixels().begin(), 127, myMovie->getRasterPtr(2)->pixels().size());
        } else if (_myDestinationPixelFormat == PIX_FMT_YUVA420P ) {
            memset(myMovie->getRasterPtr(0)->pixels().begin(), 16, myMovie->getRasterPtr(0)->pixels().size());
            memset(myMovie->getRasterPtr(1)->pixels().begin(), 127, myMovie->getRasterPtr(1)->pixels().size());
            memset(myMovie->getRasterPtr(2)->pixels().begin(), 127, myMovie->getRasterPtr(2)->pixels().size());
            memset(myMovie->getRasterPtr(3)->pixels().begin(), 0, myMovie->getRasterPtr(3)->pixels().size());
        } else {
            for (unsigned i = 0; i < myRasterCount; i++) {
                myMovie->getRasterPtr(i)->clear();
            }
        }
        */

        // allocate frame for YUV data
        // _myFrame = avcodec_alloc_frame();
    }
}
