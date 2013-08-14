
use("SceneViewer.js");
// load spark (with sparkish widgets)
this.useSparkWidgets = true;
use("spark/spark.js");
use("VlcCapture.js");

plug("y60vlc");

// load layouts and run the event loop
try {
    var ourApp = spark.loadFile("tests/LAYOUT/VideoTest.spark");

    var videoCapture = ourApp.getChildByName("zdf");
    print(videoCapture.uri);
    window.swapInterval = 1;
        
    var myRealOnFrame = ourApp.onFrame;

    ourApp.onFrame = function(theTime, theDeltaT) {
    
        if (videoCapture.playmode === "stop") {
            print("starting playback");
            videoCapture.playmode = "play";
        }
        
        myRealOnFrame.call(ourApp, theTime, theDeltaT);
    }
    ourApp.go();

} catch (ex) {
    reportException(ex);
    exit(1);
}

