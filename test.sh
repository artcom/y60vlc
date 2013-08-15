#!/bin/bash
ENVIRONMENT="../obj/lib/:SCRIPTS:.:/Users/juliank/Development/Y60/y60vlc/src/js:/Users/juliank/Development/Y60/y60vlc_build/lib/"

#y60 -I $ENVIRONMENT tests/testMovieChangeChannels.tst.js rehearsal $*
y60 -I $ENVIRONMENT tests/testMovieStartStop.tst.js rehearsal $*
#y60 -I $ENVIRONMENT tests/testMoviePauseResume.tst.js rehearsal $*
#y60 -I $ENVIRONMENT tests/testMovieLooping.tst.js rehearsal $*