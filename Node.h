#pragma once
#include "cSprite.h"

class Node : public cSprite
{
private:
	SDL_Point asteroidVelocity;
	Node* neighbourNorth;
	Node* neighbourSouth;
	Node* neighbourWest;
	Node* neighbourEast;
public:
	Node();
	void update(double deltaTime);		// Rocket update method
	void setAsteroidVelocity(SDL_Point asteroidVel);   // Sets the velocity for the asteroid
	SDL_Point getAsteroidVelocity();				 // Gets the asteroid velocity
};
