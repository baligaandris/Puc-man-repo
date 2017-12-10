/*
=================
cRocket.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cRocket.h"


/*
=================================================================
Defualt Constructor
=================================================================
*/
cRocket::cRocket() : cSprite()
{
	this->rocketVelocity = { 0, 0 };

}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cRocket::update(double deltaTime, Node* lastNode, Node* nextNode)
{

	/*FPoint direction = { 0.0f, 0.0f };
	direction.X = (sin(this->getSpriteRotAngle()*PI / 180));
	direction.Y = -(cos(this->getSpriteRotAngle()*PI / 180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;*/

	/*this->rocketVelocity.x = this->rocketVelocity.x + direction.X;
	this->rocketVelocity.y = this->rocketVelocity.y + direction.Y;*/

	//SDL_Rect currentSpritePos = this->getSpritePos();
	//rocketPosition.x += this->rocketVelocity.x * deltaTime;
	//rocketPosition.y += this->rocketVelocity.y * deltaTime;
	if (lastNode!=NULL && nextNode!=NULL)
	{
		
		rocketPosition.x = rocketPosition.x - (lastNode->getSpritePos().x - nextNode->getSpritePos().x)*1*deltaTime;
		rocketPosition.y = rocketPosition.y - (lastNode->getSpritePos().y - nextNode->getSpritePos().y)*1*deltaTime;
	}

	//cout << deltaTime << endl

	//this->rocketVelocity.x *= 0.95;f
	//this->rocketVelocity.y *= 0.95;

	this->setSpritePos({ (int)rocketPosition.x , (int)rocketPosition.y });
	this->setBoundingRect(this->getSpritePos());

}
/*
=================================================================
Sets the velocity for the rocket
=================================================================
*/
void cRocket::setRocketVelocity(FPoint rocketVel)
{
	rocketVelocity = rocketVel;
}
/*
=================================================================
Gets the rocket velocity
=================================================================
*/
cRocket::FPoint cRocket::getRocketVelocity()
{
	return rocketVelocity;
}


void cRocket::setRocketStartingPosition(int x, int y) {
	rocketPosition.x = x;
	rocketPosition.y = y;
}

