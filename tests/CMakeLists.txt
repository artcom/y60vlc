set(JSTESTS
MovieChangeChannels
MovieStartStop
MoviePauseResume
MovieLooping
Cube
)

foreach(TEST ${JSTESTS})
  y60_add_jstest(${TEST} y60_js)
endforeach(TEST)