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
#include <asl/base/Time.h>
#include <asl/base/assure.h>
#include <asl/base/Block.h>
#include <y60/base/DecoderManager.h>
#include <asl/base/Dashboard.h>
#include <asl/base/Auto.h>

#include <iostream>

using namespace std;
using namespace asl;

extern "C"
EXPORT asl::PlugInBase * y60vlc_instantiatePlugIn(asl::DLHandle myDLHandle) {
    return new y60::VLC(myDLHandle);
}
    
namespace y60 {
    // callback function. copy image from buffer sdk to y60 frame queue.
    void 
    VLC::frameCallback(void * objectPointer, int theColumns, int theRows, 
                void* theImagePointer) 
    {
        y60::VLC * obj = (y60::VLC*)objectPointer;

        size_t myBufferSize = theRows * theColumns * 4;
        unsigned char * myImagePointer = static_cast<unsigned char*>(theImagePointer);

        if (!objectPointer) {
            return;
        }

        ScopeLocker myFrameLock(obj->_myFrameLock, true);

        obj->_myFrameBuffer.resize(myBufferSize);

        std::copy(myImagePointer, myImagePointer+myBufferSize, obj->_myFrameBuffer.begin());
        obj->_myFrameBufferWidth = theColumns;
        obj->_myFrameBufferHeight = theRows;
    }

    VLC::VLC(asl::DLHandle theDLHandle) 
        : PlugInBase(theDLHandle),
            _myInitialized(false)
    {
    }

    VLC::~VLC() {
    }

    // copy camera frame from queue to y60 dom. 
    void VLC::readFrame(dom::ResizeableRasterPtr theTargetRaster) {
        ScopeLocker myFrameLock(_myFrameLock, true);
        if (_myFrameBuffer.size() == 0) {
            return;
        }

		if (getFrameWidth() != _myFrameBufferWidth) setFrameWidth(_myFrameBufferWidth);
		if (getFrameHeight() != _myFrameBufferHeight) setFrameHeight(_myFrameBufferHeight);

        theTargetRaster->resize(getFrameWidth(), getFrameHeight());
        
        unsigned int frameSize = _myFrameBufferWidth * _myFrameBufferHeight * 4;
        
        std::copy(_myFrameBuffer.begin(), _myFrameBuffer.end(),  
                        theTargetRaster->pixels().begin());
        _myFrameBuffer.resize(0); // clear the buffer, marking that we already read it.
    }

    void VLC::load(const std::string & theFilename) {
        try {
			AC_INFO << "loading CCBlueBox Component URL: " << theFilename;
           
            setPixelFormat(y60::BGRA);

        } catch (std::runtime_error& e) {
            std::string what = std::string("CCBlueBox exception: ") + e.what();
            throw VLC::Exception(what, PLUS_FILE_LINE);
        }
    }

    std::string VLC::canDecode(const std::string & theUrl,
                                        asl::Ptr<asl::ReadableStreamHandle> theStream)
    {
        if (theUrl.find("ccbluebox://") != std::string::npos) {
            return MIME_TYPE_CAMERA;
        } else {
            return "";
        }
    }

    asl::Ptr<y60::CaptureDevice> VLC::instance() const {
        return asl::Ptr<CaptureDevice>(new VLC(getDLHandle()));
    }

    void VLC::stopCapture() {
        CaptureDevice::stopCapture();
    }

    void VLC::startCapture() {
        
        CaptureDevice::startCapture();
    }

    void VLC::pauseCapture() {
        CaptureDevice::pauseCapture();
    }
}
