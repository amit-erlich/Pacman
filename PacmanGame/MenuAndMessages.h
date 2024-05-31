#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include "io_utils.h"

using namespace std;
namespace fs = std::filesystem;

class MenuAndMessages {
	enum CalledFrom { MENU = 1, LEVEL = 2, SETTINGS = 3, COLORS = 4, EXIT = 5 };

	GameLevel gameLevel;
	bool colorMode;
	string chosenScreen;
	bool screenWasSelected;
	vector<string> screenList;
	int screenToLoadIndx;
	bool wantToLeave;
	bool errorExists;

public:
/* --------------------------------------- Function Declaration ---------------------------------------- */
	// C-tor
	MenuAndMessages();
	// D-tor
	~MenuAndMessages();

	// Reset and Get Functions
	void setColorMode(bool isOn);
	void resetScreenWasSelected();
	void resetWantToLeave();
	GameLevel getGameLevel();
	int getHighestScore();
	string getHighestScoreFromFile();
	bool getColorMode();
	string getChosenScreen();
	bool getWantToLeave();
	bool getIsErrorExists();
	string getScreenToLoad();

	void decreasScreenToLoadIndx();

/* --- Screen Functions --- */
	void menuWindow();
	void chooseLevelWindow();
	void loadGameWindow();
	void rulesWindow();
	void highestScoreWindow();
	void settingWindow();
	void chooseColorWindow();
	void ChooseScreenWindow();
	void exitWindow();

	/* --- Print Functions --- */
	void printMenu();
	void PrintChooseLevel();
	void printLoadGame();
	void printRules();
	void printHighestScore();
	void PrintSetting();
	void PrintChooseColor();
	void PrintChooseScreen();
	void PrintExit();
	void printLoseMsg();
	void printVictoryMsg();
	void printExitGameMsg();
	void PrintScreenErrorMsg(FileErrors fileError, string fileName = "");
	void printByeByeMsg();

	/* --- Check Functions --- */
	bool checkInputValidation(CalledFrom calledFrom, char key);
	bool checkIfAnyScreenFilesExists();
	bool checkFileNameValidation(string fileName);
	static bool isFileExists(const string fileName);
	bool isThereNextScreen();

	void fillScreenList();
};