#include "cEnemy.h"


cEnemy::cEnemy()
{
	
}

void cEnemy::update(double deltaTime, Node * lastNode, Node * nextNode)
{

	if (lastNode != NULL && nextNode != NULL)
	{

		enemyPosition.x = enemyPosition.x - (lastNode->getSpritePos().x - nextNode->getSpritePos().x) * 1 * deltaTime;
		enemyPosition.y = enemyPosition.y - (lastNode->getSpritePos().y - nextNode->getSpritePos().y) * 1 * deltaTime;
	}

	this->setSpritePos({ (int)enemyPosition.x , (int)enemyPosition.y });
	this->setBoundingRect(this->getSpritePos());
}

void cEnemy::setEnemyStartingPosition(int x, int y) {
	enemyPosition.x = x;
	enemyPosition.y = y;
}

void cEnemy::setLastNode(Node* ln) 
{
	lastNode = ln;
}
void cEnemy::setNextNode(Node* nn)
{
	nextNode = nn;
}
Node* cEnemy::getLastNode()
{
	return lastNode;
}
Node* cEnemy::getNextNode()
{
	return nextNode;
}