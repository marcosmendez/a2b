#include <CExitLevel.h>
#include <CGame.h>
#include <CPlay.h>
#include <CPause.h>
#include <CExplosion.h>
#include <main.h>

void CExitLevel::init () {		
	
	ListOfTrains& trains = game->getTrains ();
	color trainColor;
	for (ListOfTrainsIterator i = trains.begin (), j = trains.end(); i != j; ++i) {
		CTrain* train = *i;
		if (train->isSelected()) trainColor = CGameDevice::green; else trainColor = train->getTrack()->getColor();
		events.push_back (new CExplosion (train, trainColor));
	}	
	
	CTrain* qtrain = game->getQTrain();	
	if (qtrain) {
		if (qtrain->isSelected()) trainColor = CGameDevice::green; else trainColor = qtrain->getTrack()->getColor();
		events.push_back (new CExplosion (qtrain, trainColor));
	}

}

void CExitLevel::eval () {

	game->drawHUD ();
	drawEvents (events);				
	evalEvents (events); 
	if (events.size () == 0) doLevelComplete (); 	
	
}

CExitLevel::~CExitLevel () {
	clear<ListOfEvents, CEvent>(events);
}
