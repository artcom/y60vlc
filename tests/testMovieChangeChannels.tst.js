
use("SceneViewer.js");
// load spark (with sparkish widgets)
this.useSparkWidgets = true;
use("spark/spark.js");
use("VlcCapture.js");

plug("y60vlc");

// load layouts and run the event loop
try {
    var lastChannelChangeTime = null;
    var ourApp = spark.loadFile("tests/LAYOUT/VideoStreamTest.spark");

    var channels = [
        "rtp://239.35.10.19:10000",
        "rtp://239.35.10.20:10000",
        "rtp://239.35.10.22:10000",
        "rtp://239.35.10.23:10000",
        "rtp://239.35.10.2:10000"
    ];
    var currentChannel = 0;

    var videoCapture = ourApp.getChildByName("zdf");

    print("# TEST: testMovie_change_channels #");
    print(videoCapture.uri);
    window.swapInterval = 1;
    
    var adjustSize = function (theVideo) {
        theVideo.width = theVideo.frameWidth;
        theVideo.height = theVideo.frameHeight;
    };

    var myRealOnFrame = ourApp.onFrame;
    ourApp.onFrame = function(theTime, theDeltaT) {
        
        adjustSize(videoCapture);

        if (lastChannelChangeTime !== null && theTime - lastChannelChangeTime > 10) {
            
            currentChannel++;
            if (currentChannel >= channels.length) {
                currentChannel = 0;
            }
            
            print ("Changing to channel: " + channels[currentChannel]);
            videoCapture.uri = channels[currentChannel];
            
            lastChannelChangeTime = null;
        }
        if (lastChannelChangeTime === null) {
            lastChannelChangeTime = theTime;
        }
        
        myRealOnFrame.call(ourApp, theTime, theDeltaT);
    }
    ourApp.go();

} catch (ex) {
    reportException(ex);
    exit(1);
}

