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

float cRocket::lerp(float v0, float v1, float t)
{

	return (1 - t) * v0 + t * v1;
}

//void cRocket::setBoundingRect(SDL_Rect pRect)
//{
//	SDL_Point sPos = { this->getSpritePos().x, this->getSpritePos().y };
//	this->boundingRect = { sPos.x+ (this->getSpritePos().w/2), sPos.y+ (this->getSpritePos().h/2), 1, 1}; //(pRect, sPos.x - m_Radius, sPos.y - m_Radius, (textureWidth / 2 + sPos.x), (textureHeight / 2 + sPos.y));
//
//}

/*
=================================================================
Update the sprite position
=================================================================
*/


//
//	glm::vec2 spriteVelocityAdd = glm::vec2(0.0f, 0.0f);
//	spriteVelocityAdd.x = (glm::sin(glm::radians(spriteRotation)));
//	spriteVelocityAdd.y = -(glm::cos(glm::radians(spriteRotation)));
//
//	spriteVelocityAdd *= spriteTranslation;
//
//	rocketVelocity += spriteVelocityAdd;
//
//	spritePos2D += rocketVelocity * deltaTime;
//
//	rocketVelocity *= 0.95;
//
//	/*
//	==============================================================
//	| Check for collisions
//	==============================================================
//	*/
//	for (vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin(); bulletIterartor != theRocketBullets.end(); ++bulletIterartor)
//	{
//		(*bulletIterartor)->update(deltaTime);
//		for (vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin(); asteroidIterator != theAsteroids.end(); ++asteroidIterator)
//		{
//			if ((*asteroidIterator)->collidedWith((*asteroidIterator)->getBoundingRect(), (*bulletIterartor)->getBoundingRect()))
//			{
//				// if a collision set the bullet and asteroid to false
//				(*asteroidIterator)->setActive(false);
//				(*bulletIterartor)->setActive(false);
//			}
//		}
//	}
//
//	vector<cBullet*>::iterator bulletIterartor = theRocketBullets.begin();
//	while (bulletIterartor != theRocketBullets.end())
//	{
//		if ((*bulletIterartor)->isActive() == false)
//		{
//			bulletIterartor = theRocketBullets.erase(bulletIterartor);
//		}
//		else
//		{
//			//(*bulletIterartor)->update(deltaTime);
//			(*bulletIterartor)->render();
//			(*bulletIterartor)->renderCollisionBox();
//			++bulletIterartor;
//		}
//	}
//}