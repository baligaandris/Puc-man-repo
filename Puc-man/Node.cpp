/*
=================
cAsteroid.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "Node.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
Node::Node() : cSprite()
{
	
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void Node::update(double deltaTime)
{
	this->setBoundingRect(this->getSpritePos());
}


void Node::setNeighbours(Node* up, Node* down, Node* left, Node* right)
{
	
		neighbourUp = up;
	
		neighbourDown = down;
	
		neighbourRight = right;
	
		neighbourLeft = left;
	
}

Node* Node::getNeighbour(int dir)
{
	if (dir == 1)
	{
		return neighbourUp;
	}
	if (dir == 2) {
		return neighbourDown;
	}
	if (dir == 3) {
		return neighbourRight;
	}
	if (dir == 4) {
		return neighbourLeft;
	}
}

