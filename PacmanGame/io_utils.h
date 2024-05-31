#pragma once
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

enum class FileErrors { NO_ERROR_EXISTS, ERROR_0, ERROR_1, ERROR_2, ERROR_3, ERROR_4, ERROR_5, ERROR_6, ERROR_7, ERROR_8, ERROR_9, ERROR_10};

enum class GameLevel { BEST = 1, GOOD = 2, NOVICE = 3 };

enum class Color {
	BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4, MAGENTA = 5, BROWN = 6, LIGHTGREY = 7, DARKGREY = 8,
	LIGHTBLUE = 9, LIGHTGREEN = 10, LIGHTCYAN = 11, LIGHTRED = 12, LIGHTMAGENTA = 13, YELLOW = 14, WHITE = 15
};

enum Characters{ PACMAN = '@', GHOST = '$', BREADCRUMBS = '.', WALL = '#', 
				 SPACE = ' ', INVISIBLE_TUNNEL = '!', EMPTY_PLACE = '%', SIDE_BAR = '&'};


void gotoxy(int x, int y);
void setTextColor(Color);
void hideCursor();
void clear_screen();