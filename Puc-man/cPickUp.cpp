/*
=================
cBullet.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPickUp.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cPickUp::cPickUp() : cSprite()
{
	
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cPickUp::update(double deltaTime)
{
	this->setBoundingRect(this->getSpritePos());
}

