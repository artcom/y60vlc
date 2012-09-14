plug("y60vlc");
//print("hello world");

use("SceneTester.js");
use("ImageManager.js");

var _myMovieMaterial;
var _myMovieImage;
var _myMovieQuad;
var _myMovieBody;

function setup(theViewer) {
    _myMovieImage = theViewer.getImageManager().createMovie("tests/test720p.mkv", "y60vlc");
    _myMovieImage.playmode = "play";
    _myMovieImage.audio = false;
    _myMovieMaterial = Modelling.createUnlitTexturedMaterial(theViewer.getScene(), _myMovieImage);
    _myMovieQuad = Modelling.createQuad(theViewer.getScene(), _myMovieMaterial.id, [-100,-100,0], [100,100,0]);
    _myMovieBody = Modelling.createBody(theViewer.getScene().world, _myMovieQuad.id);
    _myMovieBody.position.z = -100;
}

function teardown(theViewer) {
    _myMovieBody.parentNode.removeChild(_myMovieBody);
    _myMovieBody = null;
    _myMovieQuad.parentNode.removeChild(_myMovieQuad);
    _myMovieQuad = null;
    _myMovieMaterial.parentNode.removeChild(_myMovieMaterial);
    _myMovieMaterial = null;
    _myMovieImage.parentNode.removeChild(_myMovieImage);
    _myMovieImage = null;
}

try {
    var ourShow = new SceneViewer(arguments);

    ourShow.SceneTester = [];

    var myFrameCount = 0;
    /*
    ourShow.SceneTester.onFrame = ourShow.onFrame;
    ourShow.onFrame = function(theTime) {
        ourShow.SceneTester.onFrame(theTime);
        if (myFrameCount % 5 == 0) {
            teardown(ourShow);
        }
        if (myFrameCount % 5 == 1) {
            setup(ourShow);
        }
        myFrameCount++;
    }
*/

    ourShow.setup(400, 400);
    setup(ourShow);

    ourShow.go();
} catch (ex) {
    print("-----------------------------------------------------------------------------------------");
    print("### Error: " + ex);
    print("-----------------------------------------------------------------------------------------");
    exit(-1);
}

