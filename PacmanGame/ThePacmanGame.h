#pragma once
#include "io_utils.h"
#include "MenuAndMessages.h"
#include "Board.h"
#include "Food.h"
#include "Pacman.h"
#include "Point.h"
#include "Ghost.h"
#include "GhostBest.h"
#include "GhostNovice.h"
#include "Fruit.h"

enum GameMode { NORMAL, SAVE, LOAD, LOAD_SILENT };

class ThePacmanGame {
	enum { ESC = 27, EXIT = 71 };
	enum { MAX_NUM_OF_GHOSTS = 4 };
	enum GameStatus { RUN = 0, WON = 1, LOST = 2, GET_OUT = 3 };
	enum Foods { FOOD_POINTS = 1 };

	GameMode mode;

	MenuAndMessages menus;
	Board board;
	Food food;
	Pacman pacman;
	Ghost* ghost[MAX_NUM_OF_GHOSTS];
	Fruit fruit;

	string screenFileName;
	bool pacmanEaten; 
	bool gamePaused;
	bool moveGhosts;
	bool moveFruit;
	bool isFruitExist;
	int gameFinished;

	int numOfGhosts;
	int count20times; // for the ghost use
	int timeToCreateFruit;
	bool fruitHitPacmanOrGhost;

	Color ghostColors[MAX_NUM_OF_GHOSTS] = { Color::LIGHTCYAN , Color::LIGHTMAGENTA , Color::BROWN, Color::LIGHTGREEN };

	// for SAVE mode
	int countGameTime;
	string initialLivesAndPoints;
	string oldGhostsMoves[MAX_NUM_OF_GHOSTS];

	// for LOAD mode
	vector<string> loadFilesList;

public:
	// C-tor
	ThePacmanGame();

	// Set and Get Functions
	void setGameMode(GameMode _mode);
	void setGameColors();
	void setGhostColor(int ghostNum);
	void setGame();
	void setScreen();

	int getRandNumFrom1To9();
	int getTimeToCreateFruitLoadMode();
	Point getRandPositionForFruit();

	// Restart Functions
	void resetGame();
	void resetFruit();
	void restartPositions();
	void clearPositions();
	void clearCreaturesMoves();

	void run();

	void eatFood(Point position);
	void swapGhosts();
	void deleteAllGhosts();
	void updateHighScore();

	// Check Functions
	bool checkIfPacmanCanMove();
	bool checkIfGhostCanMove(int ghostNum);
	bool checkIfFruitCanMove();
	void checkIfGhostAndPacmanHit(Point pacmanPos);
	void checkIfPacmanHitFruit(bool isFruitHit = false);
	void checkIfGhostHitFruit(int ghostNum, bool isFruitHit = false);
	bool checkIfValidPosInsideBoard(Point position);
	
	// Handle situation Functions
	void handleUserInput();
	void handleGameRound();
	void handlePacman();
	void handleGhosts();
	void handleFruit();
	void handlePacmanEatenSituation(); 
	void gameFinishedSituation();

	// Save mode Functions
	void saveGame();
	void saveStepsFile(string fileName);
	void saveResultFile(string fileName);

	string isPacmanDied(int numOfDeath);

	// Load mode Functions
	void loadModeOn();
	bool checkStepsValidation(string fileName);
	bool checkResultValidation(string fileName);
	bool checkScreenValidation();

	bool checkAppearenceValidation(int fruitAppearance);
	bool checkIfHalfOfLength(int length1, int length2);
	bool checkNumOfGhostValidation(int stepsFileNumOfGhost);

	// Load Silent mode Functions
	void loadSilentModeOn();
	bool checkLivesAndPointsValidation(int lives, int points);
	bool checkDeathTimesValidation(int deathNum);
	bool checkdDiedOrPassedValidation(string diedOrPassed, int deathNum);
};