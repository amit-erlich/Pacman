#pragma once
#pragma warning(disable : 4996) 
#include "Food.h"
#include <iomanip>
#include <fstream>
#include <string>

using namespace std;


class Board {
	static char board[BoardSize::WIDTH][BoardSize::LENGTH + 1];
	int maxNumOfFood;
	Point sideBar;
	Color color;

	int width, length;
	Point creatures[5];
	int countGhosts;

	FileErrors error;

public:
	
	/* C-tor */
	Board();

	bool init(string fileName);
	
	/* Get And Set Functions */
	void reset();
	void resetBoard();
	void setColor(const Color& c);
	const int getWidth() const;
	const int getLength() const;
	const Point& getSideBar() const;
	char getCharInBoard(const Point& p) const;
	int getMaxNumOfFood() const;
	Point* getCreaturesArr();
	int getNumOfGhost();
	const FileErrors& getError() const;

	bool tryToReadFileToArr(string fileName);
	bool readFileToArr(fstream& screenFile, bool& pacmanExists, bool& sideBarExists);
	bool writeToArr(const string line, int& x, int& y, bool& pacmanExists, bool& sideBarExists);
	void addInvisibleTunnelToBoard();
	void addSideBarIntoBoard(const string line, int& x, int& y);

	bool checkCharValidationInScreen(char ch);

	/* Print and Clear Fuctions */
	void clearCharInBoard(const Point& p);
	void printBoard() const;
};