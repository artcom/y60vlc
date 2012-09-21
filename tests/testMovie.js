
use("SceneViewer.js");
// load spark (with sparkish widgets)
this.useSparkWidgets = true;
use("spark/spark.js");
use("VlcCapture.js");

plug("y60vlc");

// load layouts and run the event loop
try {
    var lastChannelChangeTime = null;
    var ourApp = spark.loadFile("tests/LAYOUT/VideoTest.spark");

    var videoCapture = ourApp.getChildByName("zdf");
    print(videoCapture.uri);
    window.swapInterval = 1;
        
    var myRealOnFrame = ourApp.onFrame;
    ourApp.onFrame = function(theTime, theDeltaT) {
        if (lastChannelChangeTime !== null && theTime - lastChannelChangeTime > 5) {
            print ("changing channel");
            videoCapture.uri = "rtp://0.0.0.0:3000";
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

