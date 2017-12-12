#pragma once
#include "cSprite.h"

class Node : public cSprite
{
private:
	Node* neighbourUp;
	Node* neighbourDown;
	Node* neighbourLeft;
	Node* neighbourRight;
public:
	Node();
	void update(double deltaTime);		// Rocket update method
	void setNeighbours(Node* up, Node* down, Node* left, Node* right );
	Node* getNeighbour(int dir);
};
