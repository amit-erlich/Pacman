#include "ThePacmanGame.h"
#include "MenuAndMessages.h"

/* ------ ThePacmanGame ------
* This function sets the game.
----------------------------*/
ThePacmanGame::ThePacmanGame()
{
	screenFileName = "";
	pacmanEaten = false;
	gamePaused = false;
	moveGhosts = false;
	moveFruit = false;
	isFruitExist = false;
	gameFinished = GameStatus::RUN; // 0 = game not over, 1 = player won, 2 = player lost.
	numOfGhosts = 0;
	count20times = 0;
	timeToCreateFruit = getRandNumFrom1To9();
	fruitHitPacmanOrGhost = false;
	countGameTime = 0;
	initialLivesAndPoints = "";
}

void ThePacmanGame::setGameMode(GameMode _mode)
{
	mode = _mode;
}

/* -- setGameColors ---
* Sets the game colors.
---------------------*/
void ThePacmanGame::setGameColors()
{
	if (menus.getColorMode()) // if color mode is on
	{
		board.setColor(Color::LIGHTBLUE);
		pacman.setColor(Color::YELLOW);
		fruit.setColor(Color::LIGHTRED);
	}
	else
	{
		board.setColor(Color::WHITE);
		pacman.setColor(Color::WHITE);
		fruit.setColor(Color::WHITE);
	}
}

/* -- setGhostColor --
* Sets ghost color.
--------------------*/
void ThePacmanGame::setGhostColor(int ghostNum)
{
	if (menus.getColorMode())
		ghost[ghostNum]->setColor(ghostColors[ghostNum]);
	else
		ghost[ghostNum]->setColor(Color::WHITE);
}

/* ------------------------------------------- setGame -------------------------------------------
* Checks if board is valid and print error message if it is not.
* If it is ok - reads board from file and sets food, all creatues positions.
* Prints the board and the sideBar data.
------------------------------------------------------------------------------------------------*/
void ThePacmanGame::setGame()
{
	if (checkScreenValidation())
		setScreen(); // board is valid
}

void ThePacmanGame::setScreen()
{
	food.setNumberOfFood(board.getMaxNumOfFood());

	board.printBoard();

	countGameTime = 0;
	restartPositions();
	moveGhosts = false;
	fruitHitPacmanOrGhost = false;

	if (Point::getDrawIt())
	{
		pacman.printLivesToScreen(board.getSideBar(), menus.getColorMode());
		pacman.printPointsToScreen(board.getSideBar());
	}

	if ((mode == GameMode::SAVE) || (mode == GameMode::LOAD_SILENT))
	{
		if (mode == GameMode::SAVE)
			clearCreaturesMoves();
		initialLivesAndPoints = to_string(pacman.getLife()) + " " + to_string(pacman.getPoints());
	}
}

/* ----- getRandNumFrom0To9 ------
* Returns a number between 1 to 9.
--------------------------------*/
int ThePacmanGame::getRandNumFrom1To9()
{
	return 1 + (rand() % 9);
}

int ThePacmanGame::getTimeToCreateFruitLoadMode()
{
	string moves = fruit.getMoves();
	int count5s = 0;
	
	if (fruitHitPacmanOrGhost)
	{
		for (int i = ((countGameTime + 1) / 2); (i < moves.length() && (moves[i] == '5')); i++)
			count5s++;
		fruitHitPacmanOrGhost = false;
	}
	else
	{
		for (int i = (countGameTime / 2); (i < moves.length() && (moves[i] == '5')); i++)
			count5s++;
		if (count5s == 10)
			count5s--;
	}
	return count5s;
}

/* ------------------ getRandPositionForFruit ------------------
* Return: pointToReturn
* 
* Creates a random point within the board frame,
* and checks if its position is valid.
* Repeat these steps until the position is valid and returns is.
--------------------------------------------------------------*/
Point ThePacmanGame::getRandPositionForFruit()
{
	int x, y, width = board.getWidth(), length = board.getLength();
	bool validPos = false;
	Point pointToReturn;

	while (!validPos)
	{
		// create random point inside the board
		x = (rand() % (length - 2)) + 1;
		y = (rand() % (width - 2)) + 1;
		pointToReturn.init(x, y);

		validPos = checkIfValidPosInsideBoard(pointToReturn);
	}
	return pointToReturn;
}

/* -- resetGame ---
* Reset a new game.
-----------------*/
void ThePacmanGame::resetGame()
{
	setGameColors();
	pacman.restartLifeAndPoints();

	setGame();
}

/* ----------------------------- resetFruit -----------------------------
* Resets all fruit-releted variables and delete fruit figure from screen.
-----------------------------------------------------------------------*/
void ThePacmanGame::resetFruit()
{
	// clear fruit position from screen
	char tempCh = board.getCharInBoard(fruit.getPosition());
	(tempCh == Characters::EMPTY_PLACE) ? (clear(fruit.getPosition())) : (fruit.getPosition().draw(board.getCharInBoard(fruit.getPosition())));

	timeToCreateFruit = ((mode != GameMode::LOAD) && (mode != GameMode::LOAD_SILENT)) ? getRandNumFrom1To9() : getTimeToCreateFruitLoadMode();
	isFruitExist = false;
}

/* ------------ restartPositions ------------
* Sets all creaturs to their start positions.
-------------------------------------------*/
void ThePacmanGame::restartPositions()
{
	deleteAllGhosts();

	Point* creatures = board.getCreaturesArr();
	numOfGhosts = board.getNumOfGhost();
	pacman.restartPosition(creatures[0], Directions::STAY);

	for (int i = 0; i < numOfGhosts; i++)
	{
		switch (menus.getGameLevel())
		{
		case GameLevel::BEST:
			ghost[i] = new GhostBest();
			dynamic_cast<GhostBest*>(ghost[i])->setCount5steps(i);
			break;
		case GameLevel::GOOD:
			ghost[i] = new GhostNovice();
			break;
		case GameLevel::NOVICE:
			ghost[i] = new GhostNovice();
			break;
		default:
			break;
		}
		setGhostColor(i);
		ghost[i]->restartPosition(creatures[i + 1], Directions::UP);
		if (mode != GameMode::NORMAL)
			ghost[i]->setMoves(oldGhostsMoves[i]);
	}

	resetFruit();
}

/* -------------- clearPositions --------------
* Clears pacman and ghosts positions from sceen
---------------------------------------------*/
void ThePacmanGame::clearPositions()
{
	clear(pacman.getPosition());

	for (int i = 0; i < numOfGhosts; i++)
		clear(ghost[i]->getPosition());
}

void ThePacmanGame::clearCreaturesMoves()
{
	pacman.clearMoves();
	pacman.clearDeathTimes();
	for (int i = 0; i < numOfGhosts; i++)
		ghost[i]->clearMoves();
	fruit.clearMoves();
	fruit.clearAllFruits();
}

/* ------------------------- run --------------------------
* Prints the board and in charge of the course of the game.
---------------------------------------------------------*/
void ThePacmanGame::run()
{
	do
	{
		menus.resetWantToLeave();
		menus.menuWindow();
		if (!menus.getWantToLeave() && !menus.getIsErrorExists())
		{
			resetGame();

			do // runs as long as player did not lose and there is at least one screen
			{
				if (board.getError() == FileErrors::NO_ERROR_EXISTS) // if board is valid
				{
					do
					{
						if (_kbhit()) // if user pressed any key
							handleUserInput();
						
						handleGameRound();
						
					} while (gameFinished == GameStatus::RUN);
				}

				if ((gameFinished != GameStatus::GET_OUT) && (mode == GameMode::SAVE))
					saveGame();
				
				if (menus.isThereNextScreen())
					if ((gameFinished == GameStatus::WON) || (gameFinished == GameStatus::RUN)) // if player has won the current screen, go to the next game screen
						setGame();

			} while (((board.getError() == FileErrors::NO_ERROR_EXISTS) || (menus.isThereNextScreen())) && (gameFinished == GameStatus::RUN)); // exits when: player chose to leave / he lost / he finished all the screens
			updateHighScore();
			if (board.getError() == FileErrors::NO_ERROR_EXISTS)
				gameFinishedSituation();
		}
	} while (!menus.getWantToLeave()); // player chose to exit the program
	deleteAllGhosts();
}

/* ------------------------- eatFood --------------------------
* Get: position
* 
* Deletes food from board in the position the function recived,
* and update pacman points.
-------------------------------------------------------------*/
void ThePacmanGame::eatFood(Point position)
{
	board.clearCharInBoard(position); // delete food from board (array)
	pacman.updatePoints(Foods::FOOD_POINTS, board.getSideBar()); // add point to pacman
	food.decreasNumberOfFood();
	if (food.getNumberOfFood() == 0) // if pacman ate all food
			gameFinished = GameStatus::WON;
}

/* ------------------------- swapGhosts --------------------------
* Swap between GhostBest and GhostNovice,
* (be GhostNovice for 5 moves and than be GhostBest for 15 moves).
----------------------------------------------------------------*/
void ThePacmanGame::swapGhosts()
{
	if (count20times == 5) // change to GhostBest
	{
		for (int i = 0; i < numOfGhosts; i++)
		{
			Ghost* tempGhost = ghost[i]; // save the current pointer to the ghost
			ghost[i] = new GhostBest();
			dynamic_cast<GhostBest*>(ghost[i])->setCount5steps(i);
			ghost[i]->restartPosition(tempGhost->getPosition()); // copy position from old ghost to new
			if (mode == GameMode::SAVE)
				ghost[i]->setMoves(tempGhost->getMoves()); // copy moves from old ghost to new
			delete tempGhost; // delete old ghost pointer
			setGhostColor(i);
		}
	}
	else if (count20times == 20) // change to GhostNovice
	{
		for (int i = 0; i < numOfGhosts; i++)
		{
			Ghost* tempGhost = ghost[i]; // save the current pointer to the ghost
			ghost[i] = new GhostNovice();
			ghost[i]->restartPosition(tempGhost->getPosition()); // copy position from old ghost to new
			if (mode == GameMode::SAVE)
				ghost[i]->setMoves(tempGhost->getMoves()); // copy moves from old ghost to new
			delete tempGhost; // delete old ghost pointer
			setGhostColor(i);
		}
		count20times = 0;
	}
	count20times++;
}

/* ------------- deleteAllGhosts -------------
* Deletes all ghosts pointer from ghost-array.
--------------------------------------------*/
void ThePacmanGame::deleteAllGhosts()
{
	if (ghost[0] != nullptr) // if there are ghosts - delete their pointer
		for (int i = 0; i < numOfGhosts; i++)
		{
			if (mode == GameMode::SAVE)
				oldGhostsMoves[i] = ghost[i]->getMoves();
			delete ghost[i];
		}
}

/* ---------------------------- updateHighScore ----------------------------
* Check if current pacman's score is higher than the highest score recorded,
* and updates highScore file if it is.
--------------------------------------------------------------------------*/
void ThePacmanGame::updateHighScore()
{
	int currHighestScore = menus.getHighestScore();

	if (pacman.getPoints() > currHighestScore)
	{
		fstream scoreFile("highScore.txt", ios::out | ios::trunc);
		if (!scoreFile) // if failed to open
			cout << "File could not be open.";
		else
			scoreFile << pacman.getPoints(); // insert the new high score to highScore file
	}
}

/* ----------- checkIfPacmanCanMove -----------
* Return: false if cant move, otherwise - true.
---------------------------------------------*/
bool ThePacmanGame::checkIfPacmanCanMove()
{
	Point nextPosition;
	nextPosition = ((mode != GameMode::LOAD) && (mode != GameMode::LOAD_SILENT)) ? pacman.getNextPosition() : pacman.getNextPosition(countGameTime);
	char nextPositionChar = board.getCharInBoard(nextPosition);

	// check if pacman hit ghost
	checkIfGhostAndPacmanHit(nextPosition);
	if (pacmanEaten) // pacman hit ghost
		return false;
	else if (nextPositionChar == Characters::WALL) // there is a wall
		return false;
	else if (nextPositionChar == Characters::BREADCRUMBS) // there is food
		eatFood(nextPosition);
	return true;
}

/* ------------ checkIfGhostCanMove ----------- 
* Return: false if cant move, otherwise - true.
---------------------------------------------*/
bool ThePacmanGame::checkIfGhostCanMove(int ghostNum)
{
	if ((mode != GameMode::LOAD) && (mode != GameMode::LOAD_SILENT))
		ghost[ghostNum]->changeDirection();
	Point nextPosition = ghost[ghostNum]->getPosition();
	nextPosition.move(ghost[ghostNum]->getDirection(((mode != GameMode::LOAD) && (mode != GameMode::LOAD_SILENT)) ? (-1) : (countGameTime / 2)));
	char nextPositionChar = board.getCharInBoard(nextPosition);

	// there is a wall or invisible tunnel
	if ((nextPositionChar == Characters::WALL) || (nextPositionChar == Characters::INVISIBLE_TUNNEL)) 
		return false;

	checkIfGhostAndPacmanHit(pacman.getPosition());
	if (pacmanEaten) // ghost hit pacman
		return false;
	return true;
}

/* ----------- checkIfFruitCanMove ------------
* Return: false if cant move, otherwise - true.
---------------------------------------------*/
bool ThePacmanGame::checkIfFruitCanMove()
{
	if ((mode != GameMode::LOAD) && (mode != GameMode::LOAD_SILENT))
		fruit.changeDirection();
	Point nextPosition = fruit.getPosition();
	nextPosition.move(fruit.getDirection(((mode != GameMode::LOAD) && (mode != GameMode::LOAD_SILENT)) ? (-1) : (countGameTime / 2)));
	char nextPositionChar = board.getCharInBoard(nextPosition);

	// there is a wall or invisible tunnel
	if ((nextPositionChar == Characters::WALL) || (nextPositionChar == Characters::INVISIBLE_TUNNEL)) 
		return false;

	return true;
}

/* -------------- checkIfGhostAndPacmanHit --------------- */
void ThePacmanGame::checkIfGhostAndPacmanHit(Point pacmanPos)
{
	// compare pacman and ghosts positions
	for (int i = 0; i < numOfGhosts; i++)
	{
		if (compare(pacmanPos, ghost[i]->getPosition()))
		{
			pacmanEaten = true;
			break;
		}
	}
}

/* ------- checkIfPacmanHitFruit ------- */
void ThePacmanGame::checkIfPacmanHitFruit(bool isFruitHit)
{
	if (compare(pacman.getPosition(), fruit.getPosition()))
	{
		if (isFruitHit)
			fruitHitPacmanOrGhost = true;
		resetFruit();
		pacman.updatePoints(fruit.getPoints(), board.getSideBar()); // add point to pacman
		pacman.draw();
	}
}

/* ------------- checkIfGhostHitFruit ------------- */
void ThePacmanGame::checkIfGhostHitFruit(int ghostNum, bool isFruitHit)
{
	if (isFruitExist && compare(ghost[ghostNum]->getPosition(), fruit.getPosition()))
	{
		if (isFruitHit)
			fruitHitPacmanOrGhost = true;
		resetFruit();
		ghost[ghostNum]->draw();
	}
}

bool ThePacmanGame::checkIfValidPosInsideBoard(Point position)
{
	bool validPos = false;
	Point sideBar = board.getSideBar();

	if (board.getCharInBoard(position) != Characters::WALL) // check if there is a wall in this position
	{
		// check if position is inside sideBar
		if (position.getY() == (sideBar.getY() + 1))
		{
			if ((position.getX() < sideBar.getX()) || (position.getX() > (sideBar.getX() + 19)))
				validPos = true; // if not inside
		}
		else
			validPos = true; // if not a wall
	}
	return validPos;
}

void ThePacmanGame::handleUserInput()
{
	char key = 0;
	int dir;

	key = _getch();
	if (key == ESC) // pause / unpause the game
		gamePaused = !gamePaused;
	else if ((key == EXIT) || (key == EXIT + 32)) // user choose to exit the game (EXIT = G, EXIT + 32 = 'g')
	{
		gameFinished = GameStatus::GET_OUT;
		menus.resetScreenWasSelected();
	}
	else if ((dir = pacman.getDirection(key)) != -1) // change pacman direction
		pacman.setDirection((Directions)dir);
}

void ThePacmanGame::handleGameRound()
{
	if (!gameFinished && !gamePaused) // if game not paused
	{
		handlePacman();

		if (!pacmanEaten && moveGhosts) // can move every 2 pacman's steps
		{
			if (numOfGhosts)  // (if ghost exists)
			{
				handleGhosts();
				if (menus.getGameLevel() != GameLevel::NOVICE)
					checkIfGhostAndPacmanHit(pacman.getPosition());
			}
			handleFruit();
		}

		moveGhosts = !moveGhosts; // ghost move switch

		if (!mode == GameMode::NORMAL)
			countGameTime++;

		if (Point::getDrawIt())
			Sleep(150);
	}
}

/* ------- handlePacman ------- */
void ThePacmanGame::handlePacman()
{
	if (checkIfPacmanCanMove())
	{
		if ((mode != GameMode::LOAD) && (mode != GameMode::LOAD_SILENT))
			pacman.move();
		else
			pacman.move(' ', countGameTime);

		if (mode == GameMode::SAVE)
			pacman.addMove(((Creature)pacman).getDirection());

		if (isFruitExist)
			checkIfPacmanHitFruit();
	}
	else
	{
		if (pacmanEaten)
		{
			if (mode == GameMode::SAVE)
				pacman.addMove(((Creature)pacman).getDirection());
			handlePacmanEatenSituation();
		}
		else if (mode == GameMode::SAVE)
			pacman.addMove(Directions::STAY);
	}
}

/* ------- handleGhosts ------- */
void ThePacmanGame::handleGhosts()
{
	if (menus.getGameLevel() == GameLevel::GOOD)
		swapGhosts();

	for (int i = 0; i < numOfGhosts; i++) // check ghost movement for every ghost
	{
		if (checkIfGhostCanMove(i))
		{
			// draw the right char in ghost position before she moves
			char tempCh = board.getCharInBoard(ghost[i]->getPosition());
			if (tempCh == Characters::EMPTY_PLACE)
				tempCh = ' ';
			if ((mode != GameMode::LOAD) && (mode != GameMode::LOAD_SILENT))
				ghost[i]->moveGhost(tempCh, pacman.getPosition().getX(), pacman.getPosition().getY());
			else
				ghost[i]->moveGhost(tempCh, pacman.getPosition().getX(), pacman.getPosition().getY(), (countGameTime / 2));

			if (mode == GameMode::SAVE)
				ghost[i]->addMove(ghost[i]->getDirection());

			if (isFruitExist)
				checkIfGhostHitFruit(i);
		}
		else
		{
			ghost[i]->draw();
			if (pacmanEaten)
			{
				if (mode == GameMode::SAVE)
					ghost[i]->addMove(ghost[i]->getDirection());
				handlePacmanEatenSituation();
			}
			else if (mode == GameMode::SAVE)
				ghost[i]->addMove(Directions::STAY);
		}
	}
}

/* ------- handleFruit ------- */
void ThePacmanGame::handleFruit()
{
	if ((timeToCreateFruit == 0) && (!isFruitExist))
	{
		if ((mode != GameMode::LOAD) && (mode != GameMode::LOAD_SILENT))
			fruit.reset(getRandPositionForFruit());
		else
			fruit.reset();
		isFruitExist = true;
		checkIfPacmanHitFruit();
		for (int i = 0; i < numOfGhosts; i++)
			checkIfGhostHitFruit(i);

		if (mode == GameMode::SAVE)
		{
			fruit.addFruit();
			fruit.addMove(Directions::STAY);
		}
	}
	else if (timeToCreateFruit == 0) // fruit exists
	{
		fruit.decreasNumberOfSteps();
		if (fruit.getNumberOfSteps() == 0) // time for fruit to disappear
		{
			resetFruit();
			if (mode == GameMode::SAVE)
				fruit.addMove(Directions::X);
		}
		else
		{
			if (checkIfFruitCanMove())
			{
				char tempCh = board.getCharInBoard(fruit.getPosition());
				if (tempCh == Characters::EMPTY_PLACE)
					tempCh = ' ';
				fruit.move(tempCh, (((mode != GameMode::LOAD) && (mode != GameMode::LOAD_SILENT)) ? (-1) : (countGameTime / 2)));

				if (mode == GameMode::SAVE)
					fruit.addMove(fruit.getDirection());

				// check if fruit hit any creature
				checkIfPacmanHitFruit(true);
				for (int i = 0; i < numOfGhosts; i++)
					checkIfGhostHitFruit(i, true);
			}
			else if (mode == GameMode::SAVE)
				fruit.addMove(Directions::STAY);
		}
	}
	else
	{
		timeToCreateFruit--;
		if (mode == GameMode::SAVE)
			fruit.addMove(Directions::X);
	}
}

/* ------- handlePacmanEatenSituation ------- */
void ThePacmanGame::handlePacmanEatenSituation()
{
	pacman.updateLives(board.getSideBar(), menus.getColorMode()); // pacman loses one life
	
	if ((mode == GameMode::SAVE) || (mode == GameMode::LOAD_SILENT))
		pacman.addDeathTime(countGameTime);

	if (pacman.getLife() == 0) // if lost all lives
		gameFinished = GameStatus::LOST;
	else
	{
		clearPositions();
		restartPositions();
	}
	pacmanEaten = false;
}

/* ----- gameFinishedSituation --------
* Prints victory / lose / exit message.
-------------------------------------*/
void ThePacmanGame::gameFinishedSituation()
{
	if (gameFinished == GameStatus::WON)
		menus.printVictoryMsg();
	else if (gameFinished == GameStatus::LOST)
		menus.printLoseMsg();
	else if (gameFinished == GameStatus::GET_OUT)
		menus.printExitGameMsg();
	_getch(); // wait until any key is pressed
}

void ThePacmanGame::saveGame()
{
	string shortFileName = screenFileName.substr(0, screenFileName.length() - 6);

	// save steps file
	saveStepsFile(shortFileName + "steps");

	// save result file
	saveResultFile(shortFileName + "result");

	clearCreaturesMoves();
}

void ThePacmanGame::saveStepsFile(string fileName)
{
	fstream stepsFile(fileName, ios::out | ios::trunc);
	if (!stepsFile) // if failed to open
	{
		gotoxy(1, 1);
		cout << "File could not be open.";
	}
	else
	{
		if (menus.getColorMode())
			stepsFile << "colored" << endl;
		else
			stepsFile << "not_colored" << endl;

		// pacman steps
		stepsFile << pacman.getMoves() << endl;

		// fruit steps, number of fruits, figures and positions
		stepsFile << fruit.getMoves() << endl;

		vector<Point> allFruits = fruit.getAllFruits();
		stepsFile << allFruits.size() << endl;

		for (int i = 0; i < allFruits.size(); i++)
			stepsFile << allFruits[i].getFigure() << " " << allFruits[i].getX() << " " << allFruits[i].getY() << " ";

		// number of ghosts
		stepsFile << endl << numOfGhosts << endl;
		// ghost steps
		for (int i = 0; i < numOfGhosts; i++)
			stepsFile << ghost[i]->getMoves() << endl;

		stepsFile.close();
	}
}

void ThePacmanGame::saveResultFile(string fileName)
{
	fstream resultFile(fileName, ios::out | ios::trunc);
	if (!resultFile) // if failed to open
	{
		gotoxy(1, 1);
		cout << "File could not be open.";
	}
	else
	{
		// lives left and the points pacman earned
		resultFile << initialLivesAndPoints << endl;

		vector<int> deathTimes = pacman.getDeathTimes();
		resultFile << deathTimes.size();
		for (int i = 0; i < deathTimes.size(); i++)
			resultFile << " " << deathTimes[i];

		resultFile << endl << countGameTime << endl;
		resultFile << isPacmanDied(deathTimes.size());

		resultFile.close();
		pacman.clearDeathTimes();
	}
}

string ThePacmanGame::isPacmanDied(int numOfDeath)
{
	if (numOfDeath == (initialLivesAndPoints[0] - 48))
		return "died";
	return "passed";
}

void ThePacmanGame::loadModeOn()
{
	string fileName;
	bool noErrorExists = true;

	menus.loadGameWindow();
	if (!menus.getIsErrorExists())
	{
		// run through all files in vector:
		do
		{
			screenFileName = menus.getScreenToLoad();
			fileName = screenFileName.substr(0, screenFileName.length() - 6);
			// check if all files (result, screen, steps) exists in the current directory
			if (MenuAndMessages::isFileExists(fileName + "steps"))
			{
				if ((mode == GameMode::LOAD_SILENT) && !(MenuAndMessages::isFileExists(fileName + "result")))
					noErrorExists = false;
				else
				{
					menus.decreasScreenToLoadIndx();
					// check files validation:
					if (checkScreenValidation())
					{
						restartPositions();
						if (checkStepsValidation(fileName + "steps"))
						{
							setGameColors();
							setScreen();

							do // "run" the loaded game
								handleGameRound();
							while (gameFinished == GameStatus::RUN);

							// check if game result is equal to result file - in LOAD SILENT mode
							if ((mode == GameMode::LOAD_SILENT) && !checkResultValidation(fileName + "result"))
								noErrorExists = false;
							else
								clearCreaturesMoves();
						}
						else
							noErrorExists = false;
					}
					else
						noErrorExists = false;
				}
			}
			else
				noErrorExists = false;

			if (noErrorExists && menus.isThereNextScreen())
				if (gameFinished != GameStatus::LOST) // if player has won the current screen, go to the next game screen
					gameFinished = GameStatus::RUN;

		} while (noErrorExists && ((menus.isThereNextScreen())) && (gameFinished == GameStatus::RUN)); // exits when: player lost / finished all the screens

		if (noErrorExists && Point::getDrawIt())
			gameFinishedSituation();
	}
	else
		noErrorExists = false;

	clearCreaturesMoves();
	deleteAllGhosts();

	clear_screen();
	if (mode == GameMode::LOAD && !noErrorExists)
		cout << "Load Failed" << endl;

	if (mode == GameMode::LOAD_SILENT)
	{
		if (noErrorExists)
			cout << "Test Passed" << endl;
		else
			cout << "Test Failed" << endl;
	}
}

bool ThePacmanGame::checkStepsValidation(string fileName)
{
	int numOfFruit, numOfGhostsInFile;
	Point newFruit;

	fstream stepsFile(fileName, ios::in);
	if (!stepsFile) // if failed to open
	{
		gotoxy(0, 0);
		cout << "File could not be open.";
		return false;
	}

	try
	{
		string isColored;
		if (stepsFile >> isColored)
		{
			if (isColored.compare(("colored")) == 0)
				menus.setColorMode(true);
		}
		else
			throw 1;

		// ------ Pacman ------
		if ((stepsFile >> pacman.getMoves()) && Creature::checkMovesValidation(pacman.getMoves()));
		else
			throw 1;

		// ------ Fruit ------
		if ((stepsFile >> fruit.getMoves()) && Creature::checkMovesValidation(fruit.getMoves(), true));
		else
			throw 1;

		if (stepsFile >> numOfFruit)
		{
			int x, y, i = 0;
			char figure;
			while ((i < numOfFruit) && (stepsFile >> figure >> x >> y))
			{
				// check - figure
				if (((figure - 48) < 5) || ((figure - 48) > 9))
					throw 1;

				newFruit.setFigure(figure);
				newFruit.setX(x);
				newFruit.setY(y);

				// check - (x, y) is a valid position inside board
				if (checkIfValidPosInsideBoard(newFruit))
				{
					fruit.addFruit(newFruit);
					i++;
				}
				else
					throw 1;				
			}
		}
		else
			throw 1;

		// check FruitsAppearance
		if (!checkAppearenceValidation(numOfFruit))
			throw 1;

		if (!checkIfHalfOfLength(fruit.getMoves().length(), pacman.getMoves().length()))
			throw 1;
		
		// ------ ghost ------
		if (stepsFile >> numOfGhostsInFile)
		{
			if (!checkNumOfGhostValidation(numOfGhostsInFile))
				throw 1;
			int i = 0;
			while ((i < numOfGhostsInFile) && (stepsFile >> oldGhostsMoves[i]))
			{
				if (!Creature::checkMovesValidation(oldGhostsMoves[i]))
					throw 1;

				// check if length of ghost moves is half of pacman moves
				if (!checkIfHalfOfLength(oldGhostsMoves[i].length(), pacman.getMoves().length()))
					throw 1;
				ghost[i]->setMoves(oldGhostsMoves[i]);
				i++;
			}
		}
		else
			throw 1;
	}
	catch (const int num)
	{
		menus.PrintScreenErrorMsg(FileErrors::ERROR_10, fileName);
		stepsFile.close();
		return false;
	}

	stepsFile.close();
	return true;
}

bool ThePacmanGame::checkResultValidation(string fileName)
{
	int lives, points;
	int deathNum, deathOrPassedTime;
	int deathTime, i = 0;
	string diedOrPassed;

	fstream resultFile(fileName, ios::in);
	if (!resultFile) // if failed to open
	{
		gotoxy(0, 0);
		cout << "File could not be open.";
		return false;
	}
	try
	{
		if (resultFile >> lives >> points)
		{
			if (!checkLivesAndPointsValidation(lives, points))
				throw 1;
		}
		else
			throw 1;

		if (resultFile >> deathNum)
		{
			if (!checkDeathTimesValidation(deathNum))
				throw 1;


			while (i < deathNum && resultFile >> deathTime)
			{
				if (deathTime > pacman.getMoves().length() + 2 || deathTime != (pacman.getDeathTimes())[i])
					throw 1;
				i++;
			}
		}
		else
			throw 1;

		if (resultFile >> deathOrPassedTime)
		{
			if ((deathOrPassedTime > pacman.getMoves().length() + 2))
				throw 1;
		}
		else
			throw 1;

		if (resultFile >> diedOrPassed)
		{
			if (!(checkdDiedOrPassedValidation(diedOrPassed, deathNum)) || diedOrPassed.compare(isPacmanDied(deathNum)))
				throw 1;
		}
		else
			throw 1;
	}
	catch (const int num)
	{
		menus.PrintScreenErrorMsg(FileErrors::ERROR_10, fileName);
		resultFile.close();
		return false;
	}

	resultFile.close();
	return true;
}

bool ThePacmanGame::checkScreenValidation()
{
	clear_screen();
	gameFinished = GameStatus::RUN;

	screenFileName = menus.getScreenToLoad();
	if (!board.init(screenFileName)) // if board invalid - show error message
	{
		menus.PrintScreenErrorMsg(board.getError(), screenFileName);
		return false;
	}
	return true;
}

bool ThePacmanGame::checkAppearenceValidation(int fruitAppearance)
{
	string fruitMoves = fruit.getMoves();
	bool charIsX = true;
	int countFruitAppearance = 0, movesSize = fruitMoves.length();

	for (int i = 0; i < movesSize; i++)
	{
		if (fruitMoves[i] == '5')
			charIsX = true;
		else if (charIsX)
		{
			charIsX = false;
			countFruitAppearance++;
		}
	}

	return (countFruitAppearance == fruitAppearance);
}

bool ThePacmanGame::checkIfHalfOfLength(int length1, int length2)
{
	if (!((length1 == (length2 / 2)) || (length1 == ((length2 / 2) + 1))))
		return false;
	return true;
}

bool ThePacmanGame::checkNumOfGhostValidation(int stepsFileNumOfGhost)
{
	if (stepsFileNumOfGhost != numOfGhosts)
		return false;
	return true;
}

void ThePacmanGame::loadSilentModeOn()
{
	Point::setDoNotDrawIt();
	loadModeOn(); // turn off: print and sleep, turn on: save but only for what is written in result file
}

bool ThePacmanGame::checkLivesAndPointsValidation(int lives, int points)
{
	if (lives < 0 || lives > 3 || points < 0)
		return false;
	string tempInitialLivesAndPoints = to_string(lives) + " " + to_string(points);
	if (tempInitialLivesAndPoints.compare(initialLivesAndPoints) != 0)
		return false;
	return true;
}

bool ThePacmanGame::checkDeathTimesValidation(int deathNum)
{
	if (deathNum < 0 || deathNum > 3)
		return false;
	if (deathNum != pacman.getDeathTimes().size())
		return false;
	return true;
}

bool ThePacmanGame::checkdDiedOrPassedValidation(string diedOrPassed, int deathNum)
{
	if (diedOrPassed.compare("died") == 0)
	{
		if (pacman.getLife() != deathNum)
			return false;
	}
	else 
	{
		if (diedOrPassed.compare("passed") == 0)
			if (pacman.getLife() == deathNum)
				return false;
	}

	return true;
}