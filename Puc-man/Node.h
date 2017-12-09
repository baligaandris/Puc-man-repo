#pragma once
#include "cSprite.h"

class Node : public cSprite
{
private:
	SDL_Point asteroidVelocity;
	Node* neighbourUp;
	Node* neighbourDown;
	Node* neighbourLeft;
	Node* neighbourRight;
	enum directions {up, down, left, right};
public:
	Node();
	void update(double deltaTime);		// Rocket update method
	void setAsteroidVelocity(SDL_Point asteroidVel);   // Sets the velocity for the asteroid
	SDL_Point getAsteroidVelocity();				 // Gets the asteroid velocity
	void setNeighbours(Node* up, Node* down, Node* left, Node* right );
	Node* getNeighbour(int dir);
};
