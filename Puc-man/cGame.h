#ifndef _GAME_H
#define _GAME_H

/*
==================================================================================
cGame.h
==================================================================================
*/

#include <SDL.h>

// Game specific includes
#include "PMGame.h"
#include <sstream>

using namespace std;

class cGame
{
public:
	cGame();

	void initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void cleanUp(SDL_Window* theSDLWND);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer);
	void render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre);
	void update();
	void update(double deltaTime);
	bool getInput(bool theLoop);
	double getElapsedSeconds();

	static cGame* getInstance();
	SDL_Renderer* renderer;
	int score;
private:
	bool pacmanInstantiated = false;
	SDL_Window* window;
	vector<cEnemy*> theEnemies;
	bool getHighScore;
	int highScore;
	enum GameState
	{
		Play, Menu, End
	} gameState = Play;
	bool fileAvailable;
	float FXCooldown;
	static cGame* pInstance;
	// for framerates
	time_point< high_resolution_clock > m_lastTime;
	time_point< high_resolution_clock > m_CurrentTime;
	duration< double > deltaTime;
	
	//for file handling
	cFileHandler theFile;

	// Sprites for displaying background and rocket textures
	cSprite spriteBkgd;
	cSprite instructions;
	cPacMan thePacman;
	Node theAsteroid;
	cPickUp theBullet;
	// game related variables
	vector<LPCSTR> textureName;
	vector<LPCSTR> textName;
	vector<LPCSTR> texturesToUse;
	vector<vector<Node*>> theNodes;
	vector<cPickUp*> thePickups;
	// Fonts to use
	vector<LPCSTR> fontList;
	vector<LPCSTR> fontsToUse;
	// Text for Game
	vector<LPCSTR> gameTextList;
	// Game Sounds
	vector<LPCSTR> soundList;
	vector<soundType> soundTypes;
	vector<LPCSTR> soundsToUse;
	// Game objects
	// Define the elements and there position in/on the array/map
	int renderWidth, renderHeight;
	float pacManSpeed;
	enum directions {
		up, down, left, right, none
	};
	directions nextTurn;
	Node* lastCollision;
	Node* nextNode;

};

#endif


