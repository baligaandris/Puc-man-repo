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

class cRocket : public cSprite
{
private:
	struct FPoint
	{
		float x;
		float y;
	} rocketVelocity;

	FPoint rocketPosition;

public:
	cRocket();
	void update(double deltaTime);		// Rocket update method
	void setRocketVelocity(FPoint rocketVel);   // Sets the velocity for the rocket
	FPoint getRocketVelocity();				 // Gets the rocket velocity
	void setRocketStartingPosition(int x, int y);
};
#endif