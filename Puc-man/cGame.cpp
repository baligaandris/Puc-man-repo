/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	//set starting values
	getHighScore = true;
	window = theSDLWND;
	renderer = theRenderer;
	FXCooldown = 0;
	gameState = Menu;
	score = 0;
	//set Pacman's speed
	pacManSpeed = 100;
	//set standard direction to be "none"
	nextTurn = none;
	// Get width and height of render context
	SDL_SetWindowSize(theSDLWND, 800, 800);
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();

	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Store the textures
	textureName = { "asteroid1", "asteroid2", "asteroid3", "asteroid4", "bullet","theRocket","theBackground","node","arrowKeys","redSkull","blueSkull","greenSkull","pinkSkull"};
	texturesToUse = { "Images\\asteroid1.png", "Images\\asteroid2.png", "Images\\asteroid3.png", "Images\\asteroid4.png", "Images\\bullet.png", "Images\\Original_PacMan.png", "Images\\pacman_background.png","Images\\Node.png","Images\\Arrow_Keys.png","Images\\red_skull.png","Images\\blue_skull.png","Images\\green_skull.png","Images\\pink_skull.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}

	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/crackman.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	string scoreString2;
	string scoreString;
	scoreString = to_string(score);
	scoreString = "Score:" + scoreString;

	scoreString2 = "Final Score: " + to_string(score);

	gameTextList = { "Puc-Man", scoreString.c_str(), "Puc-Man", "High Score","Press Enter to Play" ,"Press Esc to Exit game","Game Over",scoreString2.c_str(),"Press Enter to Return to Main Menu" };
	
	theTextureMgr->addTexture("Title", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[1], SOLID,{ 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("MenuHeader", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[2], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("HighScore", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[3], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Press", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[4], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("Press2", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[5], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("GameOver", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[6], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("FinalScore", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[7], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	theTextureMgr->addTexture("ReturnToMain", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[8], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

	// Load game sounds
	soundList = { "theme", "waka", "explosion" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio/rolemusi_-_09_-_09_rolemusic_-_the_little_broth.wav", "Audio/waka1.wav", "Audio/explosion2.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
	spriteBkgd.setSpriteScale({ 0,-0.5f });
	spriteBkgd.scaleSprite();

	instructions.setSpritePos({ 300,300 });
	instructions.setTexture(theTextureMgr->getTexture("arrowKeys"));
	instructions.setSpriteDimensions(theTextureMgr->getTexture("arrowKeys")->getTWidth(), theTextureMgr->getTexture("arrowKeys")->getTHeight());
	instructions.setSpriteScale({ 0,0 });
	instructions.scaleSprite();

	// Create 2D vector array of NODES

	for (int column = 0; column < 10; column++)
	{
		vector<Node*> newVec;
		theNodes.push_back(newVec);
		for (int row = 0; row < 10; row++)
		{
			theNodes[column].push_back(new Node);
			theNodes[column][row]->setSpritePos({ 53+ 78* (column), 53+ 79*(row) });
			//theAsteroids[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
			int randAsteroid = rand() % 4;
			theNodes[column][row]->setTexture(theTextureMgr->getTexture("node"));
			theNodes[column][row]->setSpriteDimensions(theTextureMgr->getTexture("node")->getTWidth(), theTextureMgr->getTexture("node")->getTHeight());
			//theAsteroids[astro]->setAsteroidVelocity({ 3, 3 });
			theNodes[column][row]->setActive(true);
		}
	}

	//Set up connections between nodes following the classic Pac-man map design. This will take a while.
	for (int column = 0; column < 10; column++)
	{
		for (int row = 0; row < 10; row++)
		{
			//Connect everything with all their neighbours
			//first without the edges
			if (column > 0 && column<9 && row>0 && row< 9)
			{
				theNodes[column][row]->setNeighbours(theNodes[column][row-1],theNodes[column][row+1],theNodes[column-1][row],theNodes[column + 1][row]);
			}
			//then the edges without the corners
			if (column == 0 && row>0 && row<9)
			{
				theNodes[column][row]->setNeighbours(theNodes[column][row - 1], theNodes[column][row + 1], NULL, theNodes[column + 1][row]);
			}
			if (column == 9 && row>0 && row<9)
			{
				theNodes[column][row]->setNeighbours(theNodes[column][row - 1], theNodes[column][row + 1], theNodes[column - 1][row], NULL);
			}
			if (row == 0 && column>0 && column<9) 
			{
				theNodes[column][row]->setNeighbours(NULL, theNodes[column][row + 1], theNodes[column - 1][row], theNodes[column + 1][row]);
			}
			if (row == 9 && column>0 && column<9)
			{
				theNodes[column][row]->setNeighbours(theNodes[column][row - 1], NULL, theNodes[column - 1][row], theNodes[column + 1][row]);
			}
			//now the corners
			if (row == 0 && column == 0) 
			{
				theNodes[column][row]->setNeighbours(NULL, theNodes[column][row + 1], NULL, theNodes[column + 1][row]);
			}
			if (row == 0 && column == 9) 
			{
				theNodes[column][row]->setNeighbours(NULL, theNodes[column][row + 1], theNodes[column - 1][row], NULL);
			}
			if (row == 9 && column == 0)
			{
				theNodes[column][row]->setNeighbours(theNodes[column][row - 1], NULL, NULL, theNodes[column + 1][row]);
			}
			if (row == 9 && column == 9)
			{
				theNodes[column][row]->setNeighbours(theNodes[column][row - 1], NULL, theNodes[column - 1][row], NULL);
			}

			//disconnect the wrong connections in the row #0
			/*Vanilla line:
			theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4) , theNodes[column][row]->getNeighbour(3));
			*/
			if (row== 0)
			{
				//can't go down
				if (column == 1 || column == 3 || column == 6 || column == 8)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), NULL, theNodes[column][row]->getNeighbour(4) , theNodes[column][row]->getNeighbour(3));
				}
				//can't go right
				if (column == 4)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), NULL);
				}
				//can't go left
				if (column == 5)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), NULL, theNodes[column][row]->getNeighbour(3));
				}
			}
			
			//disconnect the wrong connections in row #1
			if (row == 1) 
			{
				//can't go up
				if (column==1 ||column==3||column==6||column==8)
				{
					theNodes[column][row]->setNeighbours(NULL, theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go down
				if (column==1 ||column==4||column==5||column==8)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), NULL, theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
			}
			//disconnect the wrong connections in row #2
			if (row == 2)
			{
				//can't go up
				if (column == 1||column == 4||column == 5|| column == 8)
				{
					theNodes[column][row]->setNeighbours(NULL, theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go down
				if (column == 0|| column == 1|| column == 3|| column == 6|| column == 8|| column == 9)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), NULL, theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go right
				if (column == 2 || column == 4 || column == 6) 
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), NULL);
				}
				//can't go left
				if (column == 3||column == 5||column==7)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), NULL, theNodes[column][row]->getNeighbour(3));
				}
			}
			//disconnect the wrong connections in row #3
			if (row == 3)
			{
				//can't go up
				if (column == 0|| column == 1|| column == 3|| column == 6|| column == 8|| column == 9)
				{
					theNodes[column][row]->setNeighbours(NULL, theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go down
				if (column == 0|| column == 1|| column == 4|| column == 5|| column == 8|| column == 9)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), NULL, theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go right
				if (column == 0|| column == 1|| column == 2|| column == 6|| column == 7|| column == 8)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), NULL);
				}
				//can't go left
				if (column == 1|| column == 2|| column == 3|| column == 7|| column == 8 ||column == 9)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), NULL, theNodes[column][row]->getNeighbour(3));
				}
			}
			//disconnect the wrong connections in row #4
			if (row ==4)
			{
				//can't go up
				if (column == 0 || column == 1 || column == 4 || column == 5 || column == 8 || column == 9) 
				{
					theNodes[column][row]->setNeighbours(NULL, theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go down
				if (column == 0 || column == 1 || column == 4 || column == 5 || column == 8 || column == 9)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), NULL, theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go right
				if (column == 3|| column == 4 || column == 5)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), NULL);
				}
				//can't go left
				if (column == 4|| column == 5 ||column == 6)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), NULL, theNodes[column][row]->getNeighbour(3));
				}
			}
			//disconnect the wrong connections in row #5
			if (row==5)
			{
				//can't go up
				if (column == 0|| column == 1|| column == 4|| column == 5|| column == 8|| column == 9)
				{
					theNodes[column][row]->setNeighbours(NULL, theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go down
				if (column == 0 || column == 1 || column == 4 || column == 5 || column == 8 || column == 9)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), NULL, theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go right
				if (column == 0 || column == 1 || column == 2 || column == 6 || column == 7 || column == 8)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), NULL);
				}
				//can't go left
				if (column == 1 || column == 2 || column == 3 || column == 7 || column == 8 || column == 9)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), NULL, theNodes[column][row]->getNeighbour(3));
				}
			}
			//disconnect the wrong connections in row #6
			if (row==6)
			{
				//can't go up
				if (column == 0 || column == 1 || column == 4 || column == 5 || column == 8 || column == 9)
				{
					theNodes[column][row]->setNeighbours(NULL, theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go down
				if (column == 1 || column == 3 || column == 6 || column == 8)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), NULL, theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go right
				if (column==4)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), NULL);
				}
				//can't go left
				if (column == 5)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), NULL, theNodes[column][row]->getNeighbour(3));
				}
			}
			//disconnect the wrong connections in row #7
			if (row==7)
			{
				//can't go up
				if (column == 1 || column == 3 || column == 6 || column == 8)
				{
					theNodes[column][row]->setNeighbours(NULL, theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go down
				if (column == 0 || column == 4 || column == 5 || column == 9)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), NULL, theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go right
				if (column ==1|| column ==7)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), NULL);
				}
				//can't go left
				if (column == 2||column == 8)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), NULL, theNodes[column][row]->getNeighbour(3));
				}
			}
			//disconnect the wrong connections in row #8
			if (row==8)
			{
				//can't go up
				if (column == 0 || column == 4 || column == 5 || column == 9)
				{
					theNodes[column][row]->setNeighbours(NULL, theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go down
				if (column == 1 || column == 2 || column == 3 || column == 6 || column == 7 || column == 8)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), NULL, theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
				//can't go right
				if (column == 2 || column == 4 || column == 6)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), NULL);
				}
				// can't go left
				if (column==3 || column == 5 || column == 7)
				{
					theNodes[column][row]->setNeighbours(theNodes[column][row]->getNeighbour(1), theNodes[column][row]->getNeighbour(2), NULL, theNodes[column][row]->getNeighbour(3));
				}
			}
			//disconnect the wrong connections in row #9
			if (row==9)
			{
				//can't go up
				if (column==1 || column == 2 || column == 3 || column == 6 || column == 7 || column == 8)
				{
					theNodes[column][row]->setNeighbours(NULL, theNodes[column][row]->getNeighbour(2), theNodes[column][row]->getNeighbour(4), theNodes[column][row]->getNeighbour(3));
				}
			}
		}
	}

	//Instatiate pickups
	int pickupCount=0;
	for (int column = 0; column < 10; column++)
	{
		for (int row = 0; row < 10; row++)
		{
			//add pickups to the under the node if it's possible
			if (theNodes[column][row]->getNeighbour(2)!=NULL)
			{
				for (size_t i = 1; i < 3; i++) //Starts from one to prevent the pickup to be placed on the node itself to aviod multiple pickups on top of each other we will place that separately
				{
					thePickups.push_back(new cBullet);
					int newBulletX;
					int newBulletY;
					newBulletX = theNodes[column][row]->getSpritePos().x + ((theNodes[column][row]->getNeighbour(2)->getSpritePos().x - theNodes[column][row]->getSpritePos().x) / 3*i);
					newBulletY = theNodes[column][row]->getSpritePos().y + ((theNodes[column][row]->getNeighbour(2)->getSpritePos().y - theNodes[column][row]->getSpritePos().y) / 3*i);
					cout << "created pickup" << endl;

					thePickups[pickupCount]->setSpritePos({ newBulletX,newBulletY });
					thePickups[pickupCount]->setTexture(theTextureMgr->getTexture("bullet"));
					thePickups[pickupCount]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
					thePickups[pickupCount]->setSpriteScale({ -0.8f,-0.8f });
					thePickups[pickupCount]->scaleSprite();
					thePickups[pickupCount]->setActive(true);
					pickupCount++;
				}
			}
			//add pickups to the right of the node if it's possible
			if (theNodes[column][row]->getNeighbour(4)!=NULL)
			{
				for (size_t i = 1; i < 3; i++)
				{
					thePickups.push_back(new cBullet);
					int newBulletX;
					int newBulletY;
					newBulletX = theNodes[column][row]->getSpritePos().x - ((theNodes[column][row]->getSpritePos().x - theNodes[column][row]->getNeighbour(4)->getSpritePos().x) / 3*i);
					newBulletY = theNodes[column][row]->getSpritePos().y + ((theNodes[column][row]->getSpritePos().y - theNodes[column][row]->getNeighbour(4)->getSpritePos().y) / 3*i);

					thePickups[pickupCount]->setSpritePos({ newBulletX,newBulletY });
					thePickups[pickupCount]->setTexture(theTextureMgr->getTexture("bullet"));
					thePickups[pickupCount]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
					thePickups[pickupCount]->setSpriteScale({ -0.8f,-0.8f });
					thePickups[pickupCount]->scaleSprite();
					pickupCount++;
				}
			}
			//add a pickup on top of the node if it is not isolated
			if (theNodes[column][row]->getNeighbour(1)!=NULL || theNodes[column][row]->getNeighbour(2) != NULL || theNodes[column][row]->getNeighbour(3) != NULL || theNodes[column][row]->getNeighbour(4) != NULL)
			{
				thePickups.push_back(new cBullet);
				int newBulletX;
				int newBulletY;
				newBulletX = theNodes[column][row]->getSpritePos().x-1;
				newBulletY = theNodes[column][row]->getSpritePos().y-1;

				thePickups[pickupCount]->setSpritePos({ newBulletX,newBulletY });
				thePickups[pickupCount]->setTexture(theTextureMgr->getTexture("bullet"));
				thePickups[pickupCount]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
				thePickups[pickupCount]->setSpriteScale({ -0.8f,-0.8f });
				thePickups[pickupCount]->scaleSprite();
				pickupCount++;
			}
		}
	}

	//Instantiate pacman
	if (pacmanInstantiated==false)
	{
		thePacman.setTexture(theTextureMgr->getTexture("theRocket"));
		thePacman.setSpriteDimensions(theTextureMgr->getTexture("theRocket")->getTWidth(), theTextureMgr->getTexture("theRocket")->getTHeight());
		thePacman.setRocketVelocity({ 0, 0 });
		thePacman.setSpriteScale({ (float)-0.8,(float)-0.8 });
		thePacman.scaleSprite();
		pacmanInstantiated = true;
	}
	int rwidth = theTextureMgr->getTexture("theRocket")->getTWidth();
	int rheight = theTextureMgr->getTexture("theRocket")->getTHeight();
	thePacman.setSpritePos({ theNodes[3][5]->getSpritePos().x - (rwidth / 10),theNodes[3][5]->getSpritePos().y - (rheight / 10) });
	thePacman.setRocketStartingPosition(theNodes[3][5]->getSpritePos().x + (theNodes[3][5]->getSpriteDimensions().w / 2) - ((thePacman.getSpriteDimensions().w * 0.2) / 2) + 1, theNodes[3][5]->getSpritePos().y + (theNodes[3][5]->getSpriteDimensions().h / 2) - ((thePacman.getSpriteDimensions().w * 0.2) / 2) + 1);
	nextNode = NULL;
	lastCollision = NULL;

	//Instantiate enemies
	//red enemy
	theEnemies.push_back(new cEnemy);
	int eWidth = theTextureMgr->getTexture("redSkull")->getTWidth();
	int eHeight = theTextureMgr->getTexture("redSkull")->getTHeight();
	int newEnemyX;
	int newEnemyY;
	newEnemyX = theNodes[2][2]->getSpritePos().x-(eWidth/20)+ theNodes[2][2]->getSpriteDimensions().w/2;
	newEnemyY = theNodes[2][2]->getSpritePos().y-(eHeight/20)+ theNodes[2][2]->getSpriteDimensions().h/2;
	theEnemies[0]->setSpritePos({ newEnemyX,newEnemyY });
	theEnemies[0]->setTexture(theTextureMgr->getTexture("redSkull"));
	theEnemies[0]->setSpriteDimensions(theTextureMgr->getTexture("redSkull")->getTWidth(), theTextureMgr->getTexture("redSkull")->getTHeight());
	theEnemies[0]->setSpriteScale({ -0.9f,-0.9f });
	theEnemies[0]->scaleSprite();
	theEnemies[0]->setActive(true);
	theEnemies[0]->setBoundingRect(theEnemies[0]->getSpritePos());

	//blue enemy
	theEnemies.push_back(new cEnemy);
	eWidth = theTextureMgr->getTexture("blueSkull")->getTWidth();
	eHeight = theTextureMgr->getTexture("blueSkull")->getTHeight();
	newEnemyX = theNodes[7][7]->getSpritePos().x - (eWidth / 20) + theNodes[7][7]->getSpriteDimensions().w / 2;
	newEnemyY = theNodes[7][7]->getSpritePos().y - (eHeight / 20) + theNodes[7][7]->getSpriteDimensions().h / 2;
	theEnemies[1]->setSpritePos({ newEnemyX,newEnemyY });
	theEnemies[1]->setTexture(theTextureMgr->getTexture("blueSkull"));
	theEnemies[1]->setSpriteDimensions(theTextureMgr->getTexture("blueSkull")->getTWidth(), theTextureMgr->getTexture("blueSkull")->getTHeight());
	theEnemies[1]->setSpriteScale({ -0.9f,-0.9f });
	theEnemies[1]->scaleSprite();
	theEnemies[1]->setActive(true);
	theEnemies[1]->setBoundingRect(theEnemies[1]->getSpritePos());

	//green enemy
	theEnemies.push_back(new cEnemy);
	eWidth = theTextureMgr->getTexture("greenSkull")->getTWidth();
	eHeight = theTextureMgr->getTexture("greenSkull")->getTHeight();
	newEnemyX = theNodes[2][7]->getSpritePos().x - (eWidth / 20) + theNodes[2][7]->getSpriteDimensions().w / 2;
	newEnemyY = theNodes[2][7]->getSpritePos().y - (eHeight / 20) + theNodes[2][7]->getSpriteDimensions().h / 2;
	theEnemies[2]->setSpritePos({ newEnemyX,newEnemyY });
	theEnemies[2]->setTexture(theTextureMgr->getTexture("greenSkull"));
	theEnemies[2]->setSpriteDimensions(theTextureMgr->getTexture("greenSkull")->getTWidth(), theTextureMgr->getTexture("greenSkull")->getTHeight());
	theEnemies[2]->setSpriteScale({ -0.9f,-0.9f });
	theEnemies[2]->scaleSprite();
	theEnemies[2]->setActive(true);
	theEnemies[2]->setBoundingRect(theEnemies[2]->getSpritePos());

	//pink enemy
	theEnemies.push_back(new cEnemy);
	eWidth = theTextureMgr->getTexture("pinkSkull")->getTWidth();
	eHeight = theTextureMgr->getTexture("pinkSkull")->getTHeight();
	newEnemyX = theNodes[7][2]->getSpritePos().x - (eWidth / 20) + theNodes[7][2]->getSpriteDimensions().w / 2;
	newEnemyY = theNodes[7][2]->getSpritePos().y - (eHeight / 20) + theNodes[7][2]->getSpriteDimensions().h / 2;
	theEnemies[3]->setSpritePos({ newEnemyX,newEnemyY });
	theEnemies[3]->setTexture(theTextureMgr->getTexture("pinkSkull"));
	theEnemies[3]->setSpriteDimensions(theTextureMgr->getTexture("pinkSkull")->getTWidth(), theTextureMgr->getTexture("pinkSkull")->getTHeight());
	theEnemies[3]->setSpriteScale({ -0.9f,-0.9f });
	theEnemies[3]->scaleSprite();
	theEnemies[3]->setActive(true);
	theEnemies[3]->setBoundingRect(theEnemies[3]->getSpritePos());
	
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	bool loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{

	SDL_RenderClear(theRenderer);
	//to handle different screens, we will render things based on gamestate
	if (gameState == Play)
	{
		//render background
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		//render all the nodes
		for (int column = 0; column < theNodes.size(); column++)
		{
			for (int row = 0; row < theNodes[column].size(); row++)
			{
				theNodes[column][row]->render(theRenderer, &theNodes[column][row]->getSpriteDimensions(), &theNodes[column][row]->getSpritePos(), theNodes[column][row]->getSpriteRotAngle(), &theNodes[column][row]->getSpriteCentre(), theNodes[column][row]->getSpriteScale());
			}

		}
		// Render each pickup in the vector array
		for (int draw = 0; draw < thePickups.size(); draw++)
		{
			thePickups[draw]->render(theRenderer, &thePickups[draw]->getSpriteDimensions(), &thePickups[draw]->getSpritePos(), thePickups[draw]->getSpriteRotAngle(), &thePickups[draw]->getSpriteCentre(), thePickups[draw]->getSpriteScale());
		}
		// Render the Title
		cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
		SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//render score
		tempTextTexture = theTextureMgr->getTexture("Score");
		pos = { 250, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(renderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// render the rocket
		thePacman.render(theRenderer, &thePacman.getSpriteDimensions(), &thePacman.getSpritePos(), thePacman.getSpriteRotAngle(), &thePacman.getSpriteCentre(), thePacman.getSpriteScale());
		// render the enemies
		for (int i = 0; i < theEnemies.size(); i++)
		{
			theEnemies[i]->render(theRenderer, &theEnemies[i]->getSpriteDimensions(), &theEnemies[i]->getSpritePos(), theEnemies[i]->getSpriteRotAngle(), &theEnemies[i]->getSpriteCentre(), theEnemies[i]->getSpriteScale());
		}
	}
	if (gameState == Menu) 
	{
		//render control instrictions
		instructions.render(renderer, &instructions.getSpriteDimensions(), &instructions.getSpritePos(), instructions.getSpriteRotAngle(),&instructions.getSpriteCentre(), instructions.getSpriteScale());
		//render title
		cTexture* tempTextTexture = theTextureMgr->getTexture("MenuHeader");
		SDL_Rect pos = { 300, 200, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//render highscore, pulling it from file, if necessary
		if (getHighScore)
		{
			getHighScore = false;
			//render High-score stored in the file
			// Set filename
			theFile.setFileName("Data/highScore.txt");
			// Check file is available
			if (!theFile.openFile(ios::in)) //open file for input output
			{
				cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
				fileAvailable = false;
			}
			else
			{
				cout << "File '" << theFile.getFileName() << "' opened for input!" << endl;
				fileAvailable = true;
			}
			string fileData;
			fileData = theFile.readDataFromFile();
			theFile.closeFile();

			stringstream convert(fileData); // stringstream used for the conversion into int. It is store in there to later compare with the player's score

			if (!(convert >> highScore))
				highScore = 0;

			string highScoreString2;
			string highScoreString;
			highScoreString = to_string(highScore);
			highScoreString2 = "HighScore: " + highScoreString;
			gameTextList[1] = highScoreString2.c_str();

			theTextureMgr->deleteTexture("HighScore");
			theTextureMgr->addTexture("HighScore", theFontMgr->getFont("spaceAge")->createTextTexture(renderer, gameTextList[1], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

		}
		tempTextTexture = theTextureMgr->getTexture("HighScore");
		pos = {275, 250, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//render Menu instructions
		tempTextTexture = theTextureMgr->getTexture("Press");
		pos = { 200, 500, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("Press2");
		pos = { 175, 550, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);


	}
	if (gameState == End)
	{
		//render game over text
		cTexture* tempTextTexture = theTextureMgr->getTexture("GameOver");
		SDL_Rect pos = { 275, 200, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		FPoint scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		//render final score
		string scoreString2;
		string scoreString;
		scoreString = to_string(score);
		scoreString2 = "Final Score: " + scoreString;
		gameTextList[7] = scoreString2.c_str();

		theTextureMgr->deleteTexture("FinalScore");
		theTextureMgr->addTexture("FinalScore", theFontMgr->getFont("spaceAge")->createTextTexture(renderer, gameTextList[7], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

		tempTextTexture = theTextureMgr->getTexture("FinalScore");
		pos = { 250, 400, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(renderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		//render menu options
		tempTextTexture = theTextureMgr->getTexture("ReturnToMain");
		pos = { 50, 500, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);

		tempTextTexture = theTextureMgr->getTexture("Press2");
		pos = { 175, 550, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	//to handle different screens, we will handle things based on gamestate

	if (gameState == Play)
	{
		// Update the visibility and position of each pickup
		vector<cBullet*>::iterator pickupIterator = thePickups.begin();
		while (pickupIterator != thePickups.end())
		{
			if ((*pickupIterator)->isActive() == false)
			{
				pickupIterator = thePickups.erase(pickupIterator);
			}
			else
			{
				(*pickupIterator)->update(deltaTime);
				++pickupIterator;
			}
		}
		/*
		==============================================================
		| Check for collisions
		==============================================================
		*/
		//Check for collisions between the pickups and the player, and if collided remove the pickup, and increase the score
		for (vector<cBullet*>::iterator pickupIterator = thePickups.begin(); pickupIterator != thePickups.end(); ++pickupIterator)
		{
			(*pickupIterator)->update(deltaTime);

			if (thePacman.collidedWith(&thePacman.getBoundingRect(), &(*pickupIterator)->getBoundingRect()))
			{

				(*pickupIterator)->setActive(false);
				score += 10;
				string scoreString2;
				string scoreString;
				scoreString = to_string(score);
				scoreString2 = "Score:" + scoreString;
				gameTextList[1] = scoreString2.c_str();

				theTextureMgr->deleteTexture("Score");
				theTextureMgr->addTexture("Score", theFontMgr->getFont("spaceAge")->createTextTexture(renderer, gameTextList[1], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));

				cTexture* tempTextTexture = theTextureMgr->getTexture("Score");
				SDL_Rect pos = { 250, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
				FPoint scale = { 1, 1 };
				tempTextTexture->renderTexture(renderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			}

		}

		//prep itarators for the 2d vector of nodes
		vector<vector<Node*>>::iterator row;
		vector<Node*>::iterator col;
		cRocket* pointRocket;
		pointRocket = &thePacman;
		SDL_Rect* pRocketColl;
		pRocketColl = &thePacman.getBoundingRect();
		//Check the nodes for collision with the player and the enemies and assign neighbours as the next node based on previous player input
		for (row = theNodes.begin(); row != theNodes.end(); ++row) {
			for (col = row->begin(); col != row->end(); ++col) {
				(*col)->update(deltaTime);

				//check collision between node and player
				if ((*col)->collidedWith(&(*col)->getBoundingRect(), pRocketColl))
				{
					if ((*col) != lastCollision) {
						//remember this node as the las collision if we have input for where to go.
						//this way we'll aviod collidion with the same node mulitple times in a row.
						if (nextTurn != none)
						{
							lastCollision = (*col);
						}
						int newX = (*col)->getSpritePos().x + ((*col)->getSpriteDimensions().w / 2) - ((thePacman.getSpriteDimensions().w * 0.2) / 2) + 1;
						int newY = (*col)->getSpritePos().y + ((*col)->getSpriteDimensions().h / 2) - ((thePacman.getSpriteDimensions().h * 0.2) / 2) + 1;
						thePacman.setSpritePos({ newX,newY });
						thePacman.setRocketStartingPosition(newX, newY);
						//based on the last input we got, assign the next target of the player
						if (nextTurn == up)
						{
							nextNode = (*col)->getNeighbour(1);
							thePacman.setSpriteRotAngle(90);
						}
						if (nextTurn == down)
						{
							nextNode = (*col)->getNeighbour(2);
							thePacman.setSpriteRotAngle(270);
						}
						if (nextTurn == left)
						{
							nextNode = (*col)->getNeighbour(4);
							thePacman.setSpriteRotAngle(0);
						}
						if (nextTurn == right)
						{
							nextNode = (*col)->getNeighbour(3);
							thePacman.setSpriteRotAngle(180);
						}
						//if we have no input, make sure we collide with this node in the next frame too, so it can move on when we do get an input
						if (nextNode == NULL)
						{
							lastCollision = NULL;
						}
						
					}

				}

				//check collision between node and enemy
				for (size_t i = 0; i < theEnemies.size(); i++)
				{
					if ((*col)->collidedWith(&(*col)->getBoundingRect(), &(theEnemies[i]->getBoundingRect()))) 
					{
						if ((*col)!=theEnemies[i]->getLastNode())
						{
							//if they collide assign a random target to the enemy choosing from the non-NULL neighbours of the node
							int newX = (*col)->getSpritePos().x + ((*col)->getSpriteDimensions().w / 2) - ((theEnemies[i]->getSpriteDimensions().w * 0.1) / 2) + 1;
							int newY = (*col)->getSpritePos().y + ((*col)->getSpriteDimensions().h / 2) - ((theEnemies[i]->getSpriteDimensions().h * 0.1) / 2) + 1;
							theEnemies[i]->setSpritePos({ newX,newY });
							theEnemies[i]->setEnemyStartingPosition(newX, newY);
							cout << "Enemy collided with node";
							vector<Node*> validNeighbours;
							for (int j = 1; j < 5; j++)
							{
								if ((*col)->getNeighbour(j)!=NULL)
								{
										validNeighbours.push_back((*col)->getNeighbour(j));
								}
							}
							int neighbourNumber = rand() % validNeighbours.size();
							srand(time(NULL));
							theEnemies[i]->setNextNode(validNeighbours[neighbourNumber]);
							theEnemies[i]->setLastNode((*col));
						}
					}
				}

			}
		}
		//Check the enemies for collision with the player, end the game if it happens
		for (int i = 0; i < theEnemies.size(); i++)
		{
			if (theEnemies[i]->collidedWith(&theEnemies[i]->getBoundingRect(),&thePacman.getBoundingRect()))
			{

				theNodes.clear();
				theEnemies.clear();
				thePickups.clear();
				gameState = End;
			}
		}

		//Play movement sound effect if the player is moving, and the sound effect is not already playing (I use an approximated float for the cooldown, as I couldn't get the lenght of the audio file)
		FXCooldown -= deltaTime;
		if (nextNode != NULL && FXCooldown <= 0) {
			theSoundMgr->getSnd("waka")->play(0);
			FXCooldown = 0.4; 
		}
		// Update the players position
		thePacman.update(deltaTime, lastCollision, nextNode);
		// Update the Enemies position
		for (size_t i = 0; i < theEnemies.size(); i++)
		{
			if (theEnemies[i]->getLastNode()!=NULL && theEnemies[i]->getNextNode()!=NULL)
			{
			theEnemies[i]->update(deltaTime,theEnemies[i]->getLastNode(),theEnemies[i]->getNextNode());
			}
		}
		//check if there are no more pickups, and end the game if there are none
		if (thePickups.size()==0)
		{
			theNodes.clear();
			theEnemies.clear();
			thePickups.clear();
			gameState = End;
		}
		
	}
	if (gameState == End)
	{
		if (score>highScore)
		{
			//on the end screen, if the player's final score is higher than the high score, save it into the file
			ofstream outFile("Data/highScore.txt", ios::out | ios::trunc); // clear contents
			outFile.open("Data/highScore.txt");
			outFile.close();

			theFile.openFile(ios::in | ios::out);
			highScore = score;
			string highScoreStr;
			highScoreStr = to_string(highScore);
			theFile.writeDataToFile(highScoreStr);

			theFile.closeFile();

			getHighScore = true;
		}
	}
}

	



bool cGame::getInput(bool theLoop)
{
	SDL_Event event;
	if (gameState == Play) {
		//get input from arrow keys, and store the direction pressed. If the direction is opposite to where the player is moving right now, turn around immediately
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				theLoop = false;
			}

			switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					gameState = Menu;
					break;
				case SDLK_DOWN:
				{
					nextTurn = down;
					if (nextNode != NULL)
					{
						if (nextNode->getSpritePos().y - lastCollision->getSpritePos().y < 0)
						{
							Node* tempNode;
							tempNode = nextNode;
							nextNode = lastCollision;
							lastCollision = tempNode;
							thePacman.setSpriteRotAngle(270);
						}
					}

				}
				break;

				case SDLK_UP:
				{
					nextTurn = up;
					if (nextNode != NULL)
					{
						if (nextNode->getSpritePos().y - lastCollision->getSpritePos().y > 0)
						{
							Node* tempNode;
							tempNode = nextNode;
							nextNode = lastCollision;
							lastCollision = tempNode;
							thePacman.setSpriteRotAngle(90);
						}
					}

				}
				break;
				case SDLK_RIGHT:
				{
					nextTurn = right;
					if (nextNode != NULL)
					{
						if (nextNode->getSpritePos().x - lastCollision->getSpritePos().x < 0)
						{
							Node* tempNode;
							tempNode = nextNode;
							nextNode = lastCollision;
							lastCollision = tempNode;
							thePacman.setSpriteRotAngle(180);
						}
					}

				}
				break;

				case SDLK_LEFT:
				{
					nextTurn = left;
					if (nextNode != NULL)
					{
						if (nextNode->getSpritePos().x - lastCollision->getSpritePos().x > 0)
						{
							Node* tempNode;
							tempNode = nextNode;
							nextNode = lastCollision;
							lastCollision = tempNode;
							thePacman.setSpriteRotAngle(0);
						}
					}

				}
				break;
				default:
					break;
				}

			default:
				break;
			}

		}
	}
	if (gameState == Menu) {
		//get input for enter and escape to start the gameplay, or quit the game respectively
		//also if player presses escape, return to the main menu
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				theLoop = false;
			}

			switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_RETURN:
					gameState = Play;
					break;
				case SDLK_DOWN:
				{


				}
				break;

				case SDLK_UP:
				{
					

				}
				break;
				case SDLK_RIGHT:
				{
					

				}
				break;

				case SDLK_LEFT:
				{
					

				}
				break;
				default:
					break;
				}

			default:
				break;
			}

		}
	}
	if (gameState == End)
	{
		//get input for enter and escape, to return to main menu or quit the game respectively
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				theLoop = false;
			}

			switch (event.type)
			{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_RETURN:
					this->initialise(window, renderer);
					gameState = Menu;
					break;
				case SDLK_DOWN:
				{


				}
				break;

				case SDLK_UP:
				{


				}
				break;
				case SDLK_RIGHT:
				{


				}
				break;

				case SDLK_LEFT:
				{


				}
				break;
				default:
					break;
				}

			default:
				break;
			}

		}
	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	//determine deltatime
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	//close file
	theFile.closeFile();

	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

