#include <windows.h>
#include <iostream>
#include "Board.h"
#include "io_utils.h"

using namespace std;

char Board::board[BoardSize::WIDTH][BoardSize::LENGTH + 1] = { '\0' };

/*---------------------- Board -----------------------------------
* Initialize board parameters and side bar.
-----------------------------------------------------------------*/
Board::Board()
{
	maxNumOfFood = 0;
	color = Color::WHITE;
	width = length = 0;
	countGhosts = 0;
	sideBar.init();
	error = FileErrors::NO_ERROR_EXISTS;
}

/*---------------------- init -----------------------------------
* Initialize the board: read from file, add tunnels and food.
-----------------------------------------------------------------*/
bool Board::init(string fileName)
{
	reset();
	// read screen from file to board and check if board in file is valid
	if (!tryToReadFileToArr(fileName))
		return false;

	addInvisibleTunnelToBoard();

	/*add and counting number of food in the game*/
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < length; j++)

			if (board[i][j] == Characters::SPACE)
			{
				board[i][j] = Characters::BREADCRUMBS;
				maxNumOfFood++;
			}
	}
	error = FileErrors::NO_ERROR_EXISTS;
	return true;
}

/*---------------------- reset -----------------------------------
* Reset board and board parameters.
-----------------------------------------------------------------*/
void Board::reset()
{
	resetBoard(); 
	maxNumOfFood = 0;
	width = length = 0;
	countGhosts = 0;
}

/*---------------------- resetBoard -----------------------------------
* Reset board whith '\0'.
-----------------------------------------------------------------*/
void Board::resetBoard()
{
	for (int i = 0; i < BoardSize::WIDTH; i++)
		for (int j = 0; j < BoardSize::LENGTH; j++)
			board[i][j] = '\0';
}

/*----------------------- setColor ---------------------
GET: color
set board color 
-------------------------------------------------------------*/
void Board::setColor(const Color& c)
{
	color = c;
}

/*----------------------- getWidth ---------------------
*RETURN: Width.
-------------------------------------------------------------*/
const int Board::getWidth() const
{
	return width;
}

/*----------------------- getLength ---------------------
*RETURN: Length.
-------------------------------------------------------------*/
const int Board::getLength() const
{
	return length;
}

/*----------------------- getSideBar ---------------------
*RETURN: Side Bar
-------------------------------------------------------------*/
const Point& Board::getSideBar() const
{
	return sideBar;
}

/*----------------------- getCharInBoard ---------------------
GET: location (x,y)
RETURN: char
This function return the char at a specific point in board.
--------------------------------------------------------------*/
char Board::getCharInBoard(const Point& p) const
{
	return board[p.getY()][p.getX()];
}

/*----------------------- getMaxNumOfFood ---------------------
RETURN: maximun number of food in the game
--------------------------------------------------------------*/
int Board::getMaxNumOfFood() const
{
	return maxNumOfFood;
}

/*----------------------- getCreaturesArr ---------------------
RETURN: array of creatures.
--------------------------------------------------------------*/
Point* Board::getCreaturesArr()
{
	return creatures;
}

/*----------------------- getNumOfGhost ---------------------
RETURN: Number of ghost
--------------------------------------------------------------*/
int Board::getNumOfGhost()
{
	return countGhosts;
}

const FileErrors& Board::getError() const
{
	return error;
}

/*----------------------- tryToReadFileToArr ----------------------------
RETURN: TRUE if Read File To Array succeed and legal , FALSE otherwise.
-------------------------------------------------------------------------*/
bool Board::tryToReadFileToArr(string fileName)
{
	// check if file exisist - if not return false
	fstream screenFile(fileName, ios::in);
	if (!screenFile) // if failed to open
	{
		error = FileErrors::ERROR_2;
		return false;
	}

	// else - file opened
	bool pacmanExists = false;
	bool sideBarExists = false;

	if (!readFileToArr(screenFile, pacmanExists, sideBarExists))
	{
		screenFile.close();
		return false;
	}

	if (!pacmanExists)
	{
		error = FileErrors::ERROR_7;
		screenFile.close();
		return false;
	}

	if (!sideBarExists)
	{
		error = FileErrors::ERROR_6;
		screenFile.close();
		return false;
	}

	screenFile.close();
	return true;
}

/*----------------------- readFileToArr ---------------------
*RETURN: TRUE if Read File To Array succeed, FALSE otherwise.
--------------------------------------------------------------*/
bool Board::readFileToArr(fstream& screenFile, bool& pacmanExists, bool& sideBarExists)
{
	int i, j = 0;
	string curLine;

	while (!screenFile.eof())
	{
		i = 0;
		getline(screenFile, curLine);

		if (j == 0) // read first line of the file
		{
			while ((i < BoardSize::LENGTH) && (curLine[i] != '\0'))
			{
				if (!writeToArr(curLine, i, j, pacmanExists, sideBarExists))
					return false;
				i++;
			}
		}
		else
		{
			while (i < length)
			{
				if ((curLine.length() > i) && (!writeToArr(curLine, i, j, pacmanExists, sideBarExists)))
					return false;
				else if ((curLine.length() <= i) && (board[j][i] == '\0')) // if line is shorter than board length - fill missing chars with '%'
					board[j][i] = Characters::EMPTY_PLACE;
				i++;
			}
		}
		// if file ended and there is only '&' in the last line of the file
		if (!((screenFile.eof()) && (curLine == "&")))
			j++;
	}
	width = j;
	Point::setWidthLength(width, length); // update current board size
	return true;
}

/*----------------------- writeToArr ---------------------
*RETURN: TRUE if Read File To Array succeed, FALSE otherwise.
*Read from file to array, count widthand length, ghosts.
--------------------------------------------------------------*/
bool Board::writeToArr(const string line, int& x, int& y, bool& pacmanExists, bool& sideBarExists)
{ 
	if (board[y][x] == '\0') // if the current position in the array is unwritten
	{
		switch (line[x])
		{
		case Characters::SIDE_BAR:// sideBarExists = true and save it position in sideBar
			if (sideBarExists) // if sidebar allready exists
			{
				error = FileErrors::ERROR_3;
				return false;
			}
			sideBarExists = true;
			sideBar.init(x, y, '&');
			if (((y == 0) && (x + 20 >= BoardSize::LENGTH + 1)) || ((y > 0) && (x + 20 > length)))
			{
				error = FileErrors::ERROR_8;
				return false; // not good - board invalid
			}
			else
			{
				addSideBarIntoBoard(line, x, y);
				(y == 0) ? (length += 19) : (1);
			}
			break;
		case Characters::PACMAN: // pacmanExists = true and save it position in creatures[0]
			if (pacmanExists) // if pacman allready exists 
			{
				error = FileErrors::ERROR_4;
				return false;
			}
			pacmanExists = true;
			creatures[0].init(x, y);
			board[y][x] = Characters::EMPTY_PLACE;
			break;
		case Characters::GHOST:// save ghosts positions in creatures[countGhosts], (countGhosts > 0)
			if (countGhosts >= 4) // there are allready 4 ghosts exists
			{
				error = FileErrors::ERROR_5;
				return false;
			}
			countGhosts++;
			creatures[countGhosts].init(x, y, '$');
			board[y][x] = Characters::SPACE;
			break;
		default:
			if (!checkCharValidationInScreen(line[x]))// check if all chars are valid
			{
				error = FileErrors::ERROR_9;
				return false;
			}
			board[y][x] = line[x];
			break;
		}
		(y == 0) ? (length++) : (1);
	}
	return true;
}

/*----------------------- addInvisibleTunnelToBoard ---------------------
* Place '!' along the enitre length of the board frame
-------------------------------------------------------------------------*/
void Board::addInvisibleTunnelToBoard()
{
	for (int i = 0; i < length; i++)
	{
		if ((board[0][i] == Characters::EMPTY_PLACE) || (board[0][i] == Characters::SPACE))
			board[0][i] = Characters::INVISIBLE_TUNNEL;
		if ((board[width - 1][i] == Characters::EMPTY_PLACE) || (board[width - 1][i] == Characters::SPACE))
			board[width - 1][i] = Characters::INVISIBLE_TUNNEL;
	}
	for (int i = 0; i < width; i++)
	{
		if ((board[i][0] == Characters::EMPTY_PLACE) || (board[i][0] == Characters::SPACE))
			board[i][0] = Characters::INVISIBLE_TUNNEL;
		if ((board[i][length - 1] == Characters::EMPTY_PLACE) || (board[i][length - 1] == Characters::SPACE))
			board[i][length - 1] = Characters::INVISIBLE_TUNNEL;
	}
}

/*----------------------- addSideBarIntoBoard ---------------------
-------------------------------------------------------------------------*/
void Board::addSideBarIntoBoard(const string line, int& x, int& y)
{
	char sideBarArr[][21] = { {"####################"},
							  {"#%POINTS:%%%%%%%%%%#"},
							  {"####################"} };
	int tempX = x, tempY = y;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			board[tempY + i][tempX + j] = sideBarArr[i][j];
			if ((i == 0) && (line[x + 1] != '\0'))
				x++;
		}
	}
	x--;
}

/*----------------------- checkCharValidationInScreen ---------------------
-------------------------------------------------------------------------*/
bool Board::checkCharValidationInScreen(char ch)
{
	if ((ch != Characters::WALL) && (ch != Characters::EMPTY_PLACE) && (ch != Characters::SPACE))
		return false;
	return true;
}

/*----------------------- clearCharInBoard ---------------------
GET: location (x,y).
-------------------------------------------------------------*/
void Board::clearCharInBoard(const Point& p)
{
	board[p.getY()][p.getX()] = Characters::SPACE;
}

/*------------------------------------ printBoard --------------------------------------
* Print the game board
----------------------------------------------------------------------------------------*/
 void Board::printBoard() const
{
	 if (Point::getDrawIt())
	 {
		 gotoxy(0, 0);

		 for (int i = 0; i < BoardSize::WIDTH; i++)
		 {
			 for (int j = 0; j < BoardSize::LENGTH; j++)
			 {
				 if ((board[i][j] == Characters::EMPTY_PLACE) || (board[i][j] == Characters::INVISIBLE_TUNNEL)) // INVISIBLE_TUNNEL char is our way to keep the ghost from get into tunnel
					 cout << ' ';
				 else
				 {

					 if ((color != Color::WHITE) && (board[i][j] == Characters::WALL))/*if user chose colors*/
						 setTextColor(color);
					 else
						 setTextColor(Color::WHITE);
					 cout << board[i][j];
				 }
			 }
			 cout << endl;
		 }
		 setTextColor(Color::WHITE);
		 gotoxy(0, 0);
	 }
}

