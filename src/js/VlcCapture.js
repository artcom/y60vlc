/*jslint nomen:false*/
/*globals plug, spark, Node, createUniqueId, window, Logger, operatingSystem*/

spark.VlcCapture = spark.ComponentClass("VlcCapture");
spark.VlcCapture.Constructor = function (Protected) {
    var Base = {};
    var Public = this;
    Public.Inherit(spark.Image);

    /////////////////////
    // Private Members //
    /////////////////////

    var _myCaptureNode   = null;
    var _myCaptureType   = "";
    var _myIp            = "unknown";
    var _myDeviceId      = 0;
    var _myAVTCameraFlag = 0;
    var _myFramerate     = 30;
    var _myBitsPerPixel  = 24;
    
    /////////////////////
    // Private Methods //
    /////////////////////
    
    function createNewCaptureNode(theURI) {
        _myCaptureNode = Node.createElement("capture");
        window.scene.images.appendChild(_myCaptureNode);
        _myCaptureNode.name = "CaptureNode";
        _myCaptureNode.id = createUniqueId();
        _myCaptureNode.playmode = "play";
        _myCaptureNode.src = theURI;
        Logger.info("Create Capture node with src: " + _myCaptureNode.src);
        if (_myCaptureNode) {
            window.scene.loadCaptureFrame(_myCaptureNode);
        }
        Public.image = _myCaptureNode;
    }
    
    ////////////////////
    // Public Methods //
    ////////////////////
    
    Base.realize = Public.realize;
    Public.realize = function (theMaterialOrShape) {
        Base.realize(theMaterialOrShape);
        createNewCaptureNode(Protected.getString("uri"));
        useCaching = true;
    };

    Base.postRealize = Public.postRealize;
    Public.postRealize = function () {
        Base.postRealize();
    };

    Public.__defineGetter__("uri", function() {
        return _myCaptureNode.src; 
    });
    Public.__defineSetter__("uri", function(theNewUri) {
        _myCaptureNode.src = theNewUri;
    });

    Public.__defineGetter__("playmode", function() {
        return _myCaptureNode.playmode; 
    });
    Public.__defineSetter__("playmode", function(theNewPlaymode) {
        _myCaptureNode.playmode = theNewPlaymode; 
    });

    Public.__defineGetter__("frameWidth", function() {
        return this.image.raster.width;
    });
    Public.__defineGetter__("frameHeight", function() {
        return this.image.raster.height;
    });
    Public.__defineGetter__("timecode", function() {
        return _myCaptureNode.timecode;
    });

};

