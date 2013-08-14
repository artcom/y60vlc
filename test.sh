#!/bin/bash
ENVIRONMENT="../obj/lib/:SCRIPTS:.:/Users/juliank/Development/Y60/y60vlc/src/js:/Users/juliank/Development/Y60/y60vlc_build/lib/"

y60 -I $ENVIRONMENT tests/testMovie_change_channels.tst.js rehearsal $*
#y60 -I $ENVIRONMENT tests/testMovie_start_stop.tst.js rehearsal $*
#y60 -I $ENVIRONMENT tests/testMovie_pause_resume.tst.js rehearsal $*
#y60 -I $ENVIRONMENT tests/testMovie_looping.tst.js rehearsal $*