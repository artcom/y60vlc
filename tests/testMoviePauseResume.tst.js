
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
    var videoCapture = new spark.VlcCapture();
    ourApp.addChild(videoCapture);
    videoCapture.name = "movie";
    videoCapture.x = 0;
    videoCapture.y = 0;
    videoCapture.z = 2;
    videoCapture.width = 160;
    videoCapture.height = 120;
    videoCapture.visible = true;
    videoCapture.type = "vlc";
    videoCapture.uri = "file:///PATH_TO_PROJECT/y60vlc/tests/test_videos/mpeg1_160x120_25_10_audio.mpg#1000";

    videoCapture.realize();
    videoCapture.postRealize();

    var on = true;
    window.swapInterval = 1;

    print("# TEST: testMovie_pause_resume #");
    print(videoCapture.uri);
    
    var adjustSize = function (theVideo) {
        theVideo.width = theVideo.frameWidth;
        theVideo.height = theVideo.frameHeight;
    };

    var myRealOnFrame = ourApp.onFrame;
    ourApp.onFrame = function(theTime, theDeltaT) {
        
        adjustSize(videoCapture);

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

