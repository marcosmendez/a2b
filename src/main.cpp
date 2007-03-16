#include <GL/gl.h>
#include <GL/freeglut.h>
#include <CProfiler.h>
#include <CGameDevice.h>
#include <CFont.h>
#include <CCheckBox.h>
#include <CTextField.h>
#include <CProgressbar.h>
#include <CColorProducer.h>
#include <CMain.h>
#include <CPlay.h>
#include <CPause.h>
#include <CLevelComplete.h>
#include <CExitLevel.h>
#include <CGame.h>
#include <CExplosion.h>
#include <CCollision.h>
#include <CNode.h>
#include <main.h>
#include <CHiScoreTable.h>
#include <HiScore.h>
#include <CHiScores.h>
#include <CAbout.h>

#include <iostream>
using namespace std;

CLoop* current = 0;
CMain* cmain = 0;
CGame* game = 0;
CPlay* play = 0;
CPause* cpause = 0;
CLevelComplete* levelComplete = 0;
CExitLevel* exitLevel = 0;
CHiScores* hiscores = 0;
CAbout* about = 0;

int64_t TIME = 0;
string APP_NAME = "a2b v2.0"; 

CHiScoreTable hiscoretable ("../data/hiscores.txt");

void doMain () {
	
	cmain->init ();
	current = cmain;
	
}

void doNewGame () {
	
	cout << "new game created." <<endl;	
	static unsigned int seed = (unsigned int) clock64 ();
	if (game) delete game; game = new CGame (seed++);
	if (!cmain) cmain = new CMain (game); else cmain->setGame (game);	
	if (!play) play = new CPlay (game); else play->setGame (game);	
	if (!cpause) cpause = new CPause (game); else cpause->setGame (game);
	if (!levelComplete) levelComplete = new CLevelComplete (game); else levelComplete->setGame (game);
	if (!exitLevel) exitLevel = new CExitLevel (game); else exitLevel->setGame (game);
	doMain();

}

void doPlay (bool paused) {
	
	play->init (paused);
	current = play;

}

void doReplay () {
	
	game->stop (); 
	doPlay (false); // doPlay (paused)
		
}

void doAbort () {
	
	game->abort (); 
	doMain ();
	
}

void doPause () {
	
	cpause->init ();
	current = cpause;
	
}

void doExitLevel () {
	
	exitLevel->init ();
	current = exitLevel;	
	
}

void doLevelComplete () {

	game->levelComplete ();
	levelComplete->init (game->getLevel()-1);
	current = levelComplete;
	
}

void doAddScore (int score, double efficiency, int levelsCompleted) {

	HiScore hs (CGame::PLAYER_NAME, score, efficiency, levelsCompleted);
	hiscoretable.addEntry (hs);
	
}

void doHiScores () {
	
	hiscores->init ();
	current = hiscores;
	
}

void doAbout () {
	about->init ();
	current = about;
}

void doExit () {
	
	ofstream lastFile ("../data/last.txt", ios::out);
	lastFile << "last_player " << CGame::PLAYER_NAME << endl;
	lastFile << "last_browsed_level " << CGame::LAST_BROWSED_LEVEL << endl;	
	
	if (game) delete game; 
	if (cmain) delete cmain;
	if (play) delete play;
	if (cpause) delete cpause;
	if (levelComplete) delete levelComplete;
	if (exitLevel) delete exitLevel;
	if (hiscores) delete hiscores;
	if (about) delete about;
		
	exit(1);
	
}


void display(void) {
	
	int64_t t = clock64 ();
		static CProfiler p ("::display()", true);
		p.start ();
			glClear (GL_COLOR_BUFFER_BIT);
			current->eval ();
			glutSwapBuffers ();
		p.stop ();
	current->time (clock64() - t); 		
	
}

bool init () {	
	
	cout << APP_NAME << " started." <<endl;	
	
   CGameDevice::findClockSpeed ();
	
	ifstream settingsFile ("../data/settings.txt", ios::in);
	
	string ignore;
	settingsFile >> ignore >> CGameDevice::WINDOW_WIDTH >> CGameDevice::WINDOW_HEIGHT;

	CGameDevice::XMAX = CGameDevice::WINDOW_WIDTH - 1;
	CGameDevice::YMAX = CGameDevice::WINDOW_HEIGHT - 1;
	
	CGameDevice::SCREEN_WIDTH = CGameDevice::WINDOW_WIDTH;
	CGameDevice::SCREEN_HEIGHT = CGameDevice::WINDOW_HEIGHT;	
	
	int gutterSize; settingsFile >> ignore >> gutterSize;
	CGameDevice::ARENA = CBox (gutterSize, gutterSize + 2 * CGameDevice::stringHeight(), CGameDevice::WINDOW_WIDTH - gutterSize, CGameDevice::WINDOW_HEIGHT - gutterSize);		
	
	// game vars
	int evalsPerSec; settingsFile >> ignore >> evalsPerSec;
	CGame::EVAL_INTERVAL = (int64_t) (CGameDevice::CLOCKS_PER_SECOND / evalsPerSec);	
	
	// game event vars
	CCollision::EVAL_INTERVAL = CGameDevice::CLOCKS_PER_SECOND / 45; 
	CExplosion::EVAL_INTERVAL = CGameDevice::CLOCKS_PER_SECOND / 30;
	

	// train vars
	settingsFile >> ignore >> CGame::TRAINS_PER_LEVEL;
	settingsFile >> ignore >> CTrainLauncher::TRAINS_FOR_UNIT_LAUNCH_INTERVAL;
	settingsFile >> ignore >> CTrainLauncher::OPPOSING_TRAIN_FREQUENCY;
	settingsFile >> ignore >> CTrain::FIRST_COACH_SIZE;
	settingsFile >> ignore >> CTrain::DELTA_COACH_SIZE;
	settingsFile >> ignore >> CTrain::COACH_SEPARATION;
	settingsFile >> ignore >> CTrain::SPEED;
	settingsFile >> ignore >> CTrain::ACCELERATION;
	settingsFile >> ignore >> CTrain::MAX_SPEED; 	
	CTrain::MIN_SPEED = -CTrain::MAX_SPEED;
	CTrain::COACH_SEPARATION_2 = CTrain::COACH_SEPARATION * CTrain::COACH_SEPARATION;
	CTrain::FINAL_COACH_SIZE = CTrain::FIRST_COACH_SIZE + (CTrain::LAST_COACH) * CTrain::DELTA_COACH_SIZE;
	CTrain::TIME_TO_REACH_MAX_SPEED = (CTrain::MAX_SPEED - CTrain::SPEED) * CGame::EVAL_INTERVAL;	
	
	
	//
	// track vars
	//

	settingsFile >> ignore >> CTrack::WAYPOINT_SPACING;				
	
	// node vars
	settingsFile >> ignore >> CNode::NUM_SLOTS;
	settingsFile >> ignore >> CNode::SLOT_DISTANCE;	
	
	/*
		load last session data
	*/
	ifstream lastFile ("../data/last.txt", ios::in);	
	lastFile >> ignore >> CGame::PLAYER_NAME;
	lastFile >> ignore >> CGame::LAST_BROWSED_LEVEL;	
	
	hiscores = new CHiScores;
	about = new CAbout;
	
	doNewGame ();	
	
	return true;
	
}

int main (int argc, char** argv) {   
	if (init()) {		
		glutInit (&argc, argv);
		glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
		glutInitWindowSize (CGameDevice::SCREEN_WIDTH, CGameDevice::SCREEN_HEIGHT);
		glutCreateWindow (APP_NAME.c_str());	
		glutDisplayFunc (display);
		glutIdleFunc (display);
		glutReshapeFunc (CGameDevice::windowResized);
		glutPassiveMotionFunc (CGameDevice::mousePositionChanged);
		glutMouseFunc (CGameDevice::mouseClicked);
		glutKeyboardFunc (CGameDevice::charPressed);
		glutKeyboardUpFunc (CGameDevice::charReleased);
		glutSpecialFunc (CGameDevice::specialKeyPressed);
		glutSpecialUpFunc (CGameDevice::specialKeyReleased);
		glutCloseFunc (doExit);
		glutMainLoop ();
	}
	return 0;
}
