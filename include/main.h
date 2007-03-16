#ifndef __MAIN
#define __MAIN

class CGameDevice;
class CMain;
class CPlay;
class CLevelComplete;
class CPause;
class CEmpty;
class CLoop;

void doMain ();
void doAbout ();
void doInstructions ();
void doNewGame ();
void doPlay (bool paused = false);
void doHiScores ();
void doExit ();
void doPause ();
void doReplay ();
void doAbort ();
void doExitLevel ();
void doLevelComplete ();
void doAddScore (int score, double efficiency, int levelsCompleted);

#endif
