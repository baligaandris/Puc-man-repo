/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPacMan.h"


/*
=================================================================
Defualt Constructor
=================================================================
*/
cPacMan::cPacMan() : cSprite()
{


}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cPacMan::update(double deltaTime, Node* lastNode, Node* nextNode)
{

	if (lastNode!=NULL && nextNode!=NULL)
	{
		
		Position.x = Position.x - (lastNode->getSpritePos().x - nextNode->getSpritePos().x)*1*deltaTime;
		Position.y = Position.y - (lastNode->getSpritePos().y - nextNode->getSpritePos().y)*1*deltaTime;
	}

	this->setSpritePos({ (int)Position.x , (int)Position.y });
	this->setBoundingRect(this->getSpritePos());

}


void cPacMan::setStartingPosition(int x, int y) {
	Position.x = x;
	Position.y = y;
}

