#include "MenuAndMessages.h"

/* -- MenuAndMessages --
* MenuAndMessages C-tor.
----------------------*/
MenuAndMessages::MenuAndMessages()
{
	gameLevel = GameLevel::NOVICE;
	colorMode = false;
	screenWasSelected = false;
	screenToLoadIndx = 0;
	wantToLeave = false;
	errorExists = false;
}

/* - ~MenuAndMessages --
* MenuAndMessages D-tor.
----------------------*/
MenuAndMessages::~MenuAndMessages()
{
	// clear all file names from vector
	screenList.clear();
}

void MenuAndMessages::setColorMode(bool isOn)
{
	colorMode = isOn;
}

void MenuAndMessages::resetScreenWasSelected()
{
	screenWasSelected = false;
}

void MenuAndMessages::resetWantToLeave()
{
	wantToLeave = false;
}

GameLevel MenuAndMessages::getGameLevel()
{
	return gameLevel;
}

int MenuAndMessages::getHighestScore()
{
	string score = getHighestScoreFromFile();
	if (score == "" || score == "File could not be open.")
		return 0;
	else
		return stoi(score);
}

string MenuAndMessages::getHighestScoreFromFile()
{
	string score;
	fstream scoreFile("highScore.txt", ios::in);
	if (!scoreFile) // if failed to open
		score = "File could not be open.";
	else
	{
		getline(scoreFile, score);
		scoreFile.close();
	}
	return score;
}

bool MenuAndMessages::getColorMode()
{
	return colorMode;
}

string MenuAndMessages::getChosenScreen()
{
	return chosenScreen;
}

bool MenuAndMessages::getWantToLeave()
{
	return wantToLeave;
}

bool MenuAndMessages::getIsErrorExists()
{
	return errorExists;
}

/* -------------------- getScreenToLoad --------------------
* Return: screen file name
* 
* Returns the current screen file name that needs to be run,
* empty string otherwise.
----------------------------------------------------------*/
string MenuAndMessages::getScreenToLoad()
{
	if (screenWasSelected) // if player choose to run a specific screen
		return chosenScreen;

	// if there is no more screen files to run
	if (screenToLoadIndx == screenList.size())
		return "";

	// else - return the current screen file name that needs to be run
	return screenList[screenToLoadIndx++];
}

void MenuAndMessages::decreasScreenToLoadIndx()
{
	screenToLoadIndx--;
}

/* ----------------------- menuWindow ----------------------- 
* Prints the menu and wait until the user enter his choice.
* If user choose:
* 1 = let user choose game level and load the game.
* 2 = present the instructions.
* 3 = present the settings.
* 9 = call exitWindow() to present the exit screen,
      and exit the program if the user choose to (returns 9).
-----------------------------------------------------------*/
void MenuAndMessages::menuWindow()
{
	char key = 0;
	printMenu();
	do
	{
		if (_kbhit()) // if any key is pressed
		{
			key = _getch();
			if (checkInputValidation(CalledFrom::MENU, key)) // check key's validation
			{
				switch (key)
				{
				case '1':
					chooseLevelWindow(); 
					loadGameWindow();
					break;
				case '2': // present rules window
					rulesWindow();
					printMenu();
					break;
				case '3': // present highest score window
					highestScoreWindow();
					printMenu();
					break;
				case '4': // present setting window
					settingWindow();
					printMenu();
					break;
				case '9': // present exit window
					exitWindow();
					if (!wantToLeave) // if user choose not to leave
					{
						key = '0';
						printMenu();
					}
					break;
				}
			}
		}

	} while ((key != '1') && (key != '9')); // run as long as the user does not choose whether to play or exit
}

/* ------------------ chooseLevelWindow ------------------
* Prints the choose level window and wait for user choice.
--------------------------------------------------------*/
void MenuAndMessages::chooseLevelWindow()
{
	char key = 0;
	PrintChooseLevel();
	do
	{
		if (_kbhit()) // get input from user - which level he want to play
			key = _getch();
	} while (!checkInputValidation(CalledFrom::LEVEL, key));
	
	gameLevel = (GameLevel)(key - '1' + 1);
}

/* --------------------- loadGameWindow ----------------------
* Prints the load game window and check if sceen files exists.
* Fill screenList with screen files name if there are,
* else - print erorr message.
------------------------------------------------------------*/
void MenuAndMessages::loadGameWindow()
{
	printLoadGame();
	screenList.clear();

	if (screenWasSelected) // if player choose to run a specific screen
	{
		if (!isFileExists(chosenScreen)) // check if chosenScreen file exists
		{
			errorExists = true;
			PrintScreenErrorMsg(FileErrors::ERROR_1, chosenScreen); // show message - file not exists
		}
		else
			errorExists = false;
	}
	else
	{
		if (!checkIfAnyScreenFilesExists())
		{
			errorExists = true;
			PrintScreenErrorMsg(FileErrors::ERROR_0);// show message - no file exists
		}
		else
		{
			errorExists = false;
			fillScreenList(); // fill screenList with all screen files name that exists
		}
	}
}

/* --------------------- rulesWindow ---------------------
* Prints the game instructions.
* The instructions are displayed until any key is pressed.
--------------------------------------------------------*/
void MenuAndMessages::rulesWindow()
{
	printRules();
	// wait until any key is pressed
	while (!_kbhit());
	_getch();
}

/* ----------- highestScoreWindow ------------
* Prints the highest score recorded if exists.
--------------------------------------------*/
void MenuAndMessages::highestScoreWindow()
{
	printHighestScore();
	gotoxy(2, 12);
	if (!isFileExists("highScore.txt"))
		cout << "No previously recorded score found.";
	else
	{
		string score = getHighestScoreFromFile();
		if (score == "")
			cout << "No previously recorded score found.";
		else
			cout << score;
	}
	// wait until any key is pressed
	while (!_kbhit());
	_getch();
}

/* ----------------------- settingWindow ------------------------
* Prints the game setting and wait for user choice.
* (1) set game colors, (2) choose a spesific screen for the game,
* or (9) to return to menu.
---------------------------------------------------------------*/
void MenuAndMessages::settingWindow()
{
	char key = 0;
	PrintSetting();
	do
	{
		if (_kbhit()) // get input from user
		{
			key = _getch();
			if (checkInputValidation(CalledFrom::SETTINGS, key))
			{
				switch (key)
				{
				case '1':
					chooseColorWindow();
					PrintSetting();
					break;
				case '2':
					ChooseScreenWindow();
					PrintSetting();
					break;
				}
			}
		}
	} while (key != '9');
}

/* ------------------------ chooseColorWindow -------------------------
* Prints the choose color window and asks if the user want to play with
* colored game or not.
---------------------------------------------------------------------*/
void MenuAndMessages::chooseColorWindow()
{
	char key = 0;
	PrintChooseColor();
	do
	{
		if (_kbhit()) // get input from user - with or without color.
			key = _getch();
	} while (!checkInputValidation(CalledFrom::COLORS, key));
	
	if (key == '1')
		 colorMode = true;
	else
		 colorMode = false;
}

/* -----------------=---- ChooseScreenWindow ----------------------
* Prints the choose screen window and wait until user enter a valid
* screen file name.
-----------------------------------------------------------------*/
void MenuAndMessages::ChooseScreenWindow()
{
	PrintChooseScreen();

	// check if there are screen files
	if (checkIfAnyScreenFilesExists())
	{
		// if there are:
		string fileName;
		bool fileIsValid = false;;
		while (!fileIsValid)
		{
			fileName = "";
			gotoxy(2, 12);
			cout << "Enter file name: (pacman_*.screen)";
			gotoxy(2, 13);
			cout << " >> ";
			cin >> fileName;
			if (!checkFileNameValidation(fileName))
			{
				gotoxy(2, 14);
				cout << "File name is not valid.";
				Sleep(1500);
				PrintChooseScreen();
			}
			else if (!isFileExists(fileName))
			{
				gotoxy(2, 14);
				cout << "File is not exists.";
				Sleep(1500);
				PrintChooseScreen();
			}
			else
				fileIsValid = true;
		}
		chosenScreen = fileName;
		screenWasSelected = true;
	}
	else // if there are no files:
	{
		gotoxy(2, 12);
		cout << "There are not any files for the game screen.";
		gotoxy(21, 22);
		cout << "press any key to return to the manu.";
		// wait until any key is pressed
		while (!_kbhit());
		_getch();
	}
}

/* ------------------------- exitWindow --------------------------
* Prints the exit window and asks if the user want to exit or not.
----------------------------------------------------------------*/
void MenuAndMessages::exitWindow()
{
	char key = 0;
	PrintExit();
	do
	{
		if (_kbhit()) // get input from user - stay or leave.
			key = _getch();
	} while (!checkInputValidation(CalledFrom::EXIT, key));
	if (key == '1')
		wantToLeave = false;
	else 
	{
		wantToLeave = true;
		// If wish to leave
		printByeByeMsg();
	}
}

/* ----- printMenu ------
* Prints the menu window.
-----------------------*/
void MenuAndMessages::printMenu()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                   _____                  __  __                              #" << endl <<
			"#                  |  __ \\                |  \\/  |                             #" << endl <<
			"#                  | |__) |_ _  ___ ______| \\  / | __ _ _ __                   #" << endl <<
			"#                  |  ___/ _` |/ __|______| |\\/| |/ _` | '_ \\                  #" << endl <<
			"#                  | |  | (_| | (__       | |  | | (_| | | | |                 #" << endl <<
			"#                  |_|   \\__,_|\\___|      |_|  |_|\\__,_|_| |_|                 #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (1) Start a new game                                 #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (2) Instructions and keys                            #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (3) Highest score                                    #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (4) Settins                                          #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (9) Exit                                             #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

/* ----- PrintChooseLevel -------
* Prints the choose level window.
-------------------------------*/
void MenuAndMessages::PrintChooseLevel()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                            _____________________                             #" << endl <<
			"#                           |                     |                            #" << endl <<
			"#                           |     Choose your     |                            #" << endl <<
			"#                           |     game level:     |                            #" << endl <<
			"#                           |_____________________|                            #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (1) Hard                                             #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (2) Normal                                           #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (3) Easy                                             #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

void MenuAndMessages::printLoadGame()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                   _____                  __  __                              #" << endl <<
			"#                  |  __ \\                |  \\/  |                             #" << endl <<
			"#                  | |__) |_ _  ___ ______| \\  / | __ _ _ __                   #" << endl <<
			"#                  |  ___/ _` |/ __|______| |\\/| |/ _` | '_ \\                  #" << endl <<
			"#                  | |  | (_| | (__       | |  | | (_| | | | |                 #" << endl <<
			"#                  |_|   \\__,_|\\___|      |_|  |_|\\__,_|_| |_|                 #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                  LOADING...                                  #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
	Sleep(1000);
}

/* --------- printRules ---------
* Prints the instructions window.
-------------------------------*/
void MenuAndMessages::printRules()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                INSTRUCTIONS:                                 #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"# Move the Pacman and eat all the breadcrumbs inside the maze while avoiding   #" << endl <<
			"# the two ghosts.                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"# Pacman can move left, right, up and down, but can't go through a wall.       #" << endl <<
			"# Pacman can also stay in his place.                                           #" << endl <<
			"#                                                                              #" << endl <<
			"# Each breadcrumb equal to 1 point. Once you eaten all the breadcrumbs on the  #" << endl <<
			"# screen - you win!                                                            #" << endl <<
			"# You can gain more points by eating random fruits that will appear on the     #" << endl <<
			"# screen.                                                                      #" << endl <<
			"#                                                                              #" << endl <<
			"# But be careful - Pacman has only 3 lives and the ghosts can eat him.         #" << endl <<
			"# In case a ghost eats the Pacman, you will lose a life. If all lives are gone #" << endl <<
			"#  - you lose.                                                                 #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                Press any key to continue >>  #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";

	_getch();
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                INSTRUCTIONS:                                 #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"# Keys to move Pacman:                                                         #" << endl <<
			"# A = LEFT     D = RIGHT     W = UP     X = DOWN     S = STAY                  #" << endl <<
			"#                                                                              #" << endl <<
			"# Game characters:                                                             #" << endl <<
			"# @ = Pacman     $ = Ghost     . = Breadcrumb                                  #" << endl <<
			"#                                                                              #" << endl <<
			"# To exit the game while playing, press G (Get out of the game).               #" << endl <<
			"# To pause the game, press ESC.                                                #" << endl <<
			"#                                                                              #" << endl <<
			"# The game has 3 levels:                                                       #" << endl <<
			"# (1) HARD   - Ghost will try to chase the pacman.                             #" << endl <<
			"# (2) NORMAL - Ghost will try to chase the pacman occasionally.                #" << endl <<
			"# (3) EASY   - Ghost moves randomlly.                                          #" << endl <<
			"#                                                                              #" << endl <<
			"# In the Settings window you can choose to play a colorful game or to play a   #" << endl <<
			"# specific game screen.                                                        #" << endl <<
			"#                                                                              #" << endl <<
			"#                     press any key to return to the manu.                     #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

void MenuAndMessages::printHighestScore()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                            _____________________                             #" << endl <<
			"#                           |                     |                            #" << endl <<
			"#                           |    Highest score    |                            #" << endl <<
			"#                           |      recorded:      |                            #" << endl <<
			"#                           |_____________________|                            #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                     press any key to return to the manu.                     #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

/* ----- PrintSetting ------
* Prints the setting window.
--------------------------*/
void MenuAndMessages::PrintSetting()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                   SETTINGS:                                  #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"# (1) Game colors                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"# (2) Load a specific game screen                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                    press (9) key to return to the manu.                      #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

/* ----- PrintChooseColor -------
* Prints the choose color window.
-------------------------------*/
void MenuAndMessages::PrintChooseColor()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                            _____________________                             #" << endl <<
			"#                           |                     |                            #" << endl <<
			"#                           |    Do you want to   |                            #" << endl <<
			"#                           |   play with color?  |                            #" << endl <<
			"#                           |_____________________|                            #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (1) Yes                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (9) No                                               #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

/* ----- PrintChooseScreen -------
* Prints the choose screen window.
--------------------------------*/
void MenuAndMessages::PrintChooseScreen()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                            _____________________                             #" << endl <<
			"#                           |                     |                            #" << endl <<
			"#                           |    Choose a game    |                            #" << endl <<
			"#                           |       screen:       |                            #" << endl <<
			"#                           |_____________________|                            #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

/* ----- PrintExit ------
* Prints the exit window.
-----------------------*/
void MenuAndMessages::PrintExit()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                            _____________________                             #" << endl <<
			"#                           |                     |                            #" << endl <<
			"#                           |    Do you want to   |                            #" << endl <<
			"#                           |    exit the game?   |                            #" << endl <<
			"#                           |_____________________|                            #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (1) No                                               #" << endl <<
			"#                                                                              #" << endl <<
			"#                         (9) Yes                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

/* --- printLoseMsg ----
* Prints s lose message.
----------------------*/
void MenuAndMessages::printLoseMsg()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                  You lose!                                   #" << endl <<
			"#                                                                              #" << endl <<
			"#                   ___   _   __  __ ___    _____   _____ ___                  #" << endl <<
			"#                  / __| /_\\ |  \\/  | __|  / _ \\ \\ / / __| _ \\                 #" << endl <<
			"#                 | (_ |/ _ \\| |\\/| | _|  | (_) \\ V /| _||   /                 #" << endl <<
			"#                  \\___/_/ \\_\\_|  |_|___|  \\___/ \\_/ |___|_|_\\                 #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                     press any key to return to the manu.                     #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

/* --- printVictoryMsg ----
* Prints a victory message.
-------------------------*/
void MenuAndMessages::printVictoryMsg()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                   Bravo!!                                    #" << endl <<
			"#                                                                              #" << endl <<
			"#                    __   _____  _   _  __      _____  _  _                    #" << endl <<
			"#                    \\ \\ / / _ \\| | | | \\ \\    / / _ \\| \\| |                   #" << endl <<
			"#                     \\ V / (_) | |_| |  \\ \\/\\/ / (_) | .` |                   #" << endl <<
			"#                      |_| \\___/ \\___/    \\_/\\_/ \\___/|_|\\_|                   #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                     press any key to return to the manu.                     #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

/* ---- printExitGameMsg -----
* Prints an exit game message.
----------------------------*/
void MenuAndMessages::printExitGameMsg()
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                  EXIT GAME.                                  #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                     press any key to return to the manu.                     #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
}

/* -------------- PrintScreenErrorMsg --------------
* Get: fileError, fileName
* 
* Prints fileName file error according to fileError.
--------------------------------------------------*/
void MenuAndMessages::PrintScreenErrorMsg(FileErrors fileError ,string fileName)
{
	clear_screen();
	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                            _____________________                             #" << endl <<
			"#                           |                     |                            #" << endl <<
			"#                           |    ERROR loading    |                            #" << endl <<
			"#                           |        file:        |                            #" << endl <<
			"#                           |_____________________|                            #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                     press any key to return to the manu.                     #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";
	
	gotoxy(2, 12);
	
	switch (fileError)
	{
	case FileErrors::ERROR_0:
		cout << "No file exist.";
		break;
	case FileErrors::ERROR_1:
		cout << "File does not exist: " << fileName;
		break;
	case FileErrors::ERROR_2:
		cout << "File could not be open: " << fileName;
		break;
	case FileErrors::ERROR_3:
		cout << "File includes more than one sidebar: " << fileName;
		break;
	case FileErrors::ERROR_4:
		cout << "File includes more than one pacman: " << fileName;
		break;
	case FileErrors::ERROR_5:
		cout << "File includes more than four ghosts: " << fileName;
		break;
	case FileErrors::ERROR_6:
		cout << "File does not includes sidebar: " << fileName;
		break;
	case FileErrors::ERROR_7:
		cout << "File does not includes pacman: " << fileName;
		break;
	case FileErrors::ERROR_8:
		cout << "File does not includes enough space for sidebar: " << fileName;
		break;
	case FileErrors::ERROR_9:
		cout << "File includes invalid char: " << fileName;
		break;
	case FileErrors::ERROR_10:
		cout << "Invalid file: " << fileName;
		gotoxy(0, 22);
		cout << "#                        press any key to exit program.                        #";
		break;
	default:
		cout << "Undefined error";
		break;
	}

	// wait until any key is pressed
	while (!_kbhit());
	_getch();
}

/* -- printByeByeMsg ----
* Prints Bye bye message.
-----------------------*/
void MenuAndMessages::printByeByeMsg()
{
	clear_screen();
	
	// those two lines are needed to print the copyright sign
	SetConsoleOutputCP(1252);
	auto cp = GetConsoleOutputCP();

	cout << "################################################################################" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                          ___            _                                    #" << endl <<
			"#                         | _ )_  _ ___  | |__ _  _ ___                        #" << endl <<
			"#                         | _ \\ || / -_) | '_ \\ || / -_)                       #" << endl <<
			"#                         |___/\\_, \\___| |_.__/\\_, \\___|                       #" << endl <<
			"#                               |__/            |__/                           #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                                                                              #" << endl <<
			"#                         © Amit Erlich and Raz Bracha                         #" << endl <<
			"#                                                                              #" << endl <<
			"################################################################################";

	Sleep(1750);
	clear_screen();
}

/* --------------------------- checkMenuInputValidation ----------------------------
* Get: calledFrom, key
* Return: true / false.
* 
* Checks if key is valid and returns an answer accordingly.
----------------------------------------------------------------------------------*/
bool MenuAndMessages::checkInputValidation(CalledFrom calledFrom, char key)
{
	switch (calledFrom)
	{
	case CalledFrom::MENU:
		if ((key == '1') || (key == '2') || (key == '3') || (key == '4') || (key == '9'))
			return true;
	case CalledFrom::LEVEL:
		if ((key == '1') || (key == '2') || (key == '3'))
			return true;
	case CalledFrom::SETTINGS:
		if ((key == '1') || (key == '2') || (key == '9'))
			return true;
	case CalledFrom::COLORS:
		if ((key == '1') || (key == '9'))
			return true;
	case CalledFrom::EXIT:
		if ((key == '1') || (key == '9'))
			return true;
	default:
		return false;
	}
}

/* ---------- checkIfAnyScreenFilesExists -----------
* Return: true/ false
* 
* Checks if there is at least one screen file exists.
* If there is - return true, false otherwise.
---------------------------------------------------*/
bool MenuAndMessages::checkIfAnyScreenFilesExists()
{
	bool filesExists = false;

	string path = ".";
	// run all files in the current directory until finds screen file or until there are no more files to go through
	for (const auto& entry : fs::directory_iterator(path))
		if (checkFileNameValidation(entry.path().string().substr(2)))
		{
			// found screen file
			filesExists = true;
			break;
		}

	return filesExists;
}

/* ------------------ checkFileNameValidation -------------------
* Get: fileName
* Return: true / false
* 
* Checks if fileName is a valid screen file name: pacman_*.screen
* If it does - return true, false otherwise.
---------------------------------------------------------------*/
bool MenuAndMessages::checkFileNameValidation(string fileName)
{
	if (fileName.length() < strlen("pacman_.screen")) // check if shorter than minimum length
		return false;

	for (int i = 0; i < strlen("pacman_"); i++) // check if starts with "pacman_"
		if (fileName[i] != "pacman_"[i])
			return false;

	for (int i = 0; i < strlen(".screen"); i++) // check if ends with ".screen"
		if (fileName[fileName.length() - 7 + i] != ".screen"[i])
			return false;

	return true;
}

/* ------------------ isFileExists ------------------
* Get: fileName
* Return: true / false
*
* Checks if fileName exists in the current directory.
---------------------------------------------------*/
bool MenuAndMessages::isFileExists(const string fileName)
{
	ifstream f(fileName.c_str());
	return f.good();
}

/* ----- isThereNextScreen ------
* Return: true / false
*
* Checks if there is next screen.
-------------------------------*/
bool MenuAndMessages::isThereNextScreen()
{
	if (screenWasSelected)
	{
		screenWasSelected = false;
		return false;
	}
	return (screenList.size() - screenToLoadIndx);
}

/* ------------------------------- fillScreenList -------------------------------
* Fill screenList with all screen files that are exists in the current directory.
-------------------------------------------------------------------------------*/
void MenuAndMessages::fillScreenList()
{
	string path = ".";
	for (const auto& entry : fs::directory_iterator(path))
		if (checkFileNameValidation(entry.path().string().substr(2)))
			screenList.push_back(entry.path().string().substr(2));
	screenToLoadIndx = 0;
}