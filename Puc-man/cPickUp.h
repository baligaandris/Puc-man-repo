/*
=================
cBullet.h
- Header file for class definition - SPECIFICATION
- Header file for the Bullet class which is a child of cSprite class
=================
*/
#ifndef _CBULLET_H
#define _CBULLET_H
#include "cSprite.h"


class cPickUp : public cSprite
{
private:


public:
	cPickUp();
	void update(double deltaTime);
};
#endif