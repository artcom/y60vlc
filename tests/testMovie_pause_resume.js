
use("SceneViewer.js");
// load spark (with sparkish widgets)
this.useSparkWidgets = true;
use("spark/spark.js");
use("VlcCapture.js");

plug("y60vlc");

// load layouts and run the event loop
try {
    var lastToggleTime = null;
    var ourApp = spark.loadFile("tests/LAYOUT/VideoTest.spark");

    var videoCapture = ourApp.getChildByName("zdf");
    print(videoCapture.uri);
    window.swapInterval = 1;
        
    var myRealOnFrame = ourApp.onFrame;

    var on = true;
    ourApp.onFrame = function(theTime, theDeltaT) {
        
        if (lastToggleTime !== null && theTime - lastToggleTime > 1) {
            
            on = !on;

            print("on: " + on);
            if (on === true) 
                videoCapture.playmode = "play";
            else
                videoCapture.playmode = "pause";

            lastToggleTime = null;
        }
        if (lastToggleTime === null) {
            lastToggleTime = theTime;
        }
        
        myRealOnFrame.call(ourApp, theTime, theDeltaT);
    }
    ourApp.go();

} catch (ex) {
    reportException(ex);
    exit(1);
}

