#pragma once
#ifndef _CENEMY_H
#define _CENEMY_H
#include "cSprite.h"
#include "asteroidsGame.h"

class cEnemy : public cSprite
{
private:
	struct FPoint
	{
		float x;
		float y;
	} enemyPosition;
	Node* nextNode;
	Node* lastNode;

public:
	cEnemy();
	void update(double deltaTime, Node* lastNode, Node* nextNode);

	void setEnemyStartingPosition(int x, int y);

	void setLastNode(Node * ln);

	void setNextNode(Node * nn);

	Node* getLastNode();

	Node* getNextNode();

};
#endif

