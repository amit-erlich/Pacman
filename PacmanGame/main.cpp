#include <windows.h>
#include <iostream>
#include "ThePacmanGame.h"
using namespace std;

bool checkInputValidation(int num, char* arr[], GameMode* mode);

int main(int argc, char* argv[])
{
	GameMode mode = GameMode::NORMAL;
	if (checkInputValidation(argc, argv, &mode))
	{
		hideCursor();
		
		ThePacmanGame game;
		game.setGameMode(mode);
		if (mode == GameMode::LOAD)
			game.loadModeOn();
		else if (mode == GameMode::LOAD_SILENT)
			game.loadSilentModeOn();
		else
		{
			game.setGameMode(mode);
			game.run();
		}
		return 0;
	}
	cout << "Invalid input. Enter nothing or only -save / -load / -load -silent" << endl; // print error msg
}

bool checkInputValidation(int num, char* arr[], GameMode* mode)
{
	string userInput1, userInput2;
	switch (num)
	{
	case 1:
		*mode = GameMode::NORMAL;
		break;
	case 2:
		userInput1 = arr[1];
		if (userInput1 == "-save")
			*mode = GameMode::SAVE;
		else if (userInput1 == "-load")
			*mode = GameMode::LOAD;
		else
			return false;
		break;
	case 3:
		userInput1 = arr[1];
		userInput2 = arr[2];
		if ((userInput1 == "-load") && (userInput2 == "-silent"))
			*mode = GameMode::LOAD_SILENT;
		else if ((userInput1 == "-save") && (userInput2 == "-silent"))
			*mode = GameMode::SAVE;
		else
			return false;
		break;
	default:
		return false;
		break;
	}
	return true;
}