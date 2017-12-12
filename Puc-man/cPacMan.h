/*
=================
cRocket.h
- Header file for class definition - SPECIFICATION
- Header file for the Rocket class which is a child of cSprite class
=================
*/
#ifndef _CROCKET_H
#define _CROCKET_H
#include "cSprite.h"
#include "PMGame.h"

class cPacMan : public cSprite
{
private:
	struct FPoint
	{
		float x;
		float y;
	} Position;



public:
	cPacMan();
	void update(double deltaTime, Node* lastNode, Node* nextNode);		// Rocket update method
	void setStartingPosition(int x, int y);
	
};
#endif


