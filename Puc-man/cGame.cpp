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
	//set Pacman's speed
	pacManSpeed = 100;
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Store the textures
	textureName = { "asteroid1", "asteroid2", "asteroid3", "asteroid4", "bullet","theRocket","theBackground","node"};
	texturesToUse = { "Images\\asteroid1.png", "Images\\asteroid2.png", "Images\\asteroid3.png", "Images\\asteroid4.png", "Images\\bullet.png", "Images\\rocketSprite.png", "Images\\starscape1024x768.png","Images\\Node.png"};
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Create textures for Game Dialogue (text)
	fontList = { "digital", "spaceAge" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/space age.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 36);
	}
	gameTextList = { "Asteroids"};
	
	theTextureMgr->addTexture("Title", theFontMgr->getFont("spaceAge")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 0, 255, 0, 255 }, { 0, 0, 0, 0 }));

	// Load game sounds
	soundList = { "theme", "shot", "explosion" };
	soundTypes = { MUSIC, SFX, SFX };
	soundsToUse = { "Audio/who10Edit.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	//theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());


	// Create 2D vector array of asteroids, which are now NODES

	for (int column = 0; column < 10; column++)
	{
		vector<Node*> newVec;
		theNodes.push_back(newVec);
		for (int row = 0; row < 10; row++)
		{
			theNodes[column].push_back(new Node);
			theNodes[column][row]->setSpritePos({ 100+ 64 * (column), 100+ 64*(row) });
			//theAsteroids[astro]->setSpriteTranslation({ (rand() % 8 + 1), (rand() % 8 + 1) });
			int randAsteroid = rand() % 4;
			theNodes[column][row]->setTexture(theTextureMgr->getTexture("node"));
			theNodes[column][row]->setSpriteDimensions(theTextureMgr->getTexture("node")->getTWidth(), theTextureMgr->getTexture("node")->getTHeight());
			//theAsteroids[astro]->setAsteroidVelocity({ 3, 3 });
			theNodes[column][row]->setActive(true);
		}
		
	}
	
	
	theRocket.setSpritePos( { theNodes[3][5]->getSpritePos().x,theNodes[3][5]->getSpritePos().y });
	theRocket.setRocketStartingPosition(theNodes[3][5]->getSpritePos().x, theNodes[3][5]->getSpritePos().y);
	cout << theNodes[3][5]->getSpritePos().x << theNodes[3][5]->getSpritePos().y;
	theRocket.setTexture(theTextureMgr->getTexture("theRocket"));
	theRocket.setSpriteDimensions(theTextureMgr->getTexture("theRocket")->getTWidth(), theTextureMgr->getTexture("theRocket")->getTHeight());
	theRocket.setRocketVelocity({ 0, 0 });


	
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
	spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
	// Render each asteroid in the vector array
	for (int column = 0; column < theNodes.size(); column++)
	{
		for (int row = 0; row < theNodes[column].size(); row++)
		{
			theNodes[column][row]->render(theRenderer, &theNodes[column][row]->getSpriteDimensions(), &theNodes[column][row]->getSpritePos(), theNodes[column][row]->getSpriteRotAngle(), &theNodes[column][row]->getSpriteCentre(), theNodes[column][row]->getSpriteScale());
		}
		
	}
	// Render each bullet in the vector array
	for (int draw = 0; draw < theBullets.size(); draw++)
	{
		theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
	}
	// Render the Title
	cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
	SDL_Rect pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	FPoint scale = { 1, 1 };
	tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	// render the rocket
	theRocket.render(theRenderer, &theRocket.getSpriteDimensions(), &theRocket.getSpritePos(), theRocket.getSpriteRotAngle(), &theRocket.getSpriteCentre(), theRocket.getSpriteScale());
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
	// Update the visibility and position of each asteriod
	/*vector<vector<Node*>>::iterator nodeIteratorColumn;
	vector<Node*>::iterator nodeIteratorRow;
	for (nodeIteratorColumn = theNodes.begin(); nodeIteratorColumn != theNodes.end(); nodeIteratorColumn++)
	{
		for (nodeIteratorRow = nodeIteratorColumn->begin(); nodeIteratorRow != nodeIteratorColumn->end(); nodeIteratorRow++)
		{
			if ((*nodeIteratorRow)->isActive() == false)
			{
				nodeIteratorRow = theNodes[nodeIteratorColumn].erase(nodeIteratorRow);
			}
			else
			{
				(*nodeIteratorRow)->update(deltaTime);

			}
		}
	}*/

	// Update the visibility and position of each bullet
	vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
	while (bulletIterartor != theBullets.end())
	{
		if ((*bulletIterartor)->isActive() == false)
		{
			bulletIterartor = theBullets.erase(bulletIterartor);
		}
		else
		{
			(*bulletIterartor)->update(deltaTime);
			++bulletIterartor;
		}
	}
	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	//for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
	//{
	//	//(*bulletIterartor)->update(deltaTime);
	//	for (vector<Node*>::iterator asteroidIterator = theNodes.begin(); asteroidIterator != theNodes.end(); ++asteroidIterator)
	//	{
	//		if ((*asteroidIterator)->collidedWith(&(*asteroidIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
	//		{
	//			// if a collision set the bullet and asteroid to false
	//			(*asteroidIterator)->setActive(false);
	//			(*bulletIterartor)->setActive(false);
	//			theSoundMgr->getSnd("explosion")->play(0);
	//		}
	//	}
	//}


	// Update the Rockets position
	theRocket.update(deltaTime);
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

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
				case SDLK_DOWN:
				{
					theRocket.setRocketVelocity({ 0,pacManSpeed });
					//if (theRocket.getSpritePos().x < (renderWidth - theRocket.getSpritePos().w))
					//{
					//	//theRocket.setSpriteTranslation({ -5, -5 });
					//	
					//}
				}
				break;

				case SDLK_UP:
				{
					theRocket.setRocketVelocity({ 0,-pacManSpeed });
					//if (theRocket.getSpritePos().x > 0)
					//{
					//	//theRocket.setSpriteTranslation({ 5, 5 });
					//	
					//}
				}
				break;
				case SDLK_RIGHT:
				{
					/*theRocket.setSpriteRotAngle(theRocket.getSpriteRotAngle() + 5);*/
					theRocket.setRocketVelocity({ pacManSpeed,0 });
				}
				break;

				case SDLK_LEFT:
				{
					theRocket.setRocketVelocity({ -pacManSpeed,0 });
					//theRocket.setSpriteRotAngle(theRocket.getSpriteRotAngle() - 5);
				}
				break;
				/*case SDLK_SPACE:
				{
					theBullets.push_back(new cBullet);
					int numBullets = theBullets.size() - 1;
					theBullets[numBullets]->setSpritePos({ theRocket.getBoundingRect().x + theRocket.getSpriteCentre().x, theRocket.getBoundingRect().y + theRocket.getSpriteCentre().y });
					theBullets[numBullets]->setSpriteTranslation({ 2, 2 });
					theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
					theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
					theBullets[numBullets]->setBulletVelocity({ 2, 2 });
					theBullets[numBullets]->setSpriteRotAngle(theRocket.getSpriteRotAngle());
					theBullets[numBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << theRocket.getBoundingRect().x << " y: " << theRocket.getBoundingRect().y << endl;
				}
				theSoundMgr->getSnd("shot")->play(0);
				break;*/
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

