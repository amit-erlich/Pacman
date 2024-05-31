#include "Pacman.h"

/* --------------------------------- Pacman ----------------------------------
* Get: x, y, life, points
* 
* Initialized the pacman with default values (or with the values it received).
* Draw the pacman in its start position.
----------------------------------------------------------------------------*/
Pacman::Pacman(int x, int y, int life, int points)
	:Creature(x, y, Characters::PACMAN, Directions::STAY)
{
	this->life = life;
	this->points = points;
	setArrowKeys();
}

/* -- setArrowKeys --
* Sets the game keys.
-------------------*/
void Pacman::setArrowKeys() 
{
	arrowKeys[Directions::UP] = 'w';
	arrowKeys[Directions::DOWN ] = 'x';
	arrowKeys[Directions::LEFT] = 'a';
	arrowKeys[Directions::RIGHT] = 'd';
	arrowKeys[Directions::STAY] = 's';
}

/* -- getLife --
* return: life
--------------*/
int Pacman::getLife() const
{
	return life;
}

/* -- getLife --
* return: &life
--------------*/
int& Pacman::getLife()
{
	return life;
}

/* -- getPoints --
* return: points
--------------*/
int Pacman::getPoints() const
{
	return points;
}

/* -- getPoints --
* return: &points
----------------*/
int& Pacman::getPoints()
{
	return points;
}

/* ---------------- getDirection ----------------
* Return: the pressed key or -1
*
* Check if key is one of the game keys.
* If it is, returns its place in arrowKeys array.
* Else - returns -1.
-----------------------------------------------*/
int Pacman::getDirection(char key) const
{
	for (int i = 0; i < 5; i++)
		if ((key == arrowKeys[i]) || (key + 32 == arrowKeys[i])) // Capital letter + 32 = lower letter in ascii
			return i;
	return -1;
}

vector<int> Pacman::getDeathTimes() const
{
	return deathTimes;
}

/* ---------- restartLifeAndPoints -----------
* reset life and points with start values
----------------------------------------------*/
void Pacman::restartLifeAndPoints()
{
	life = 3;
	points = 0;
}

void Pacman::addDeathTime(int time)
{
	deathTimes.push_back(time);
}

void Pacman::clearDeathTimes()
{
	deathTimes.clear();
}

/* ------------------- updateLives -------------------
* Reduces the life of the pacman by one,
* and prints the current amount of life to the screen.
----------------------------------------------------*/
void Pacman::updateLives(const Point& position, const bool& colorMode)
{
	life--;
	if (Point::getDrawIt())
		printLivesToScreen(position, colorMode);
}

/* ------------------- updatePoints --------------------
* Get: add, position
* 
* Adds 'add' to the pacman's points,
* and prints the current amount of points to the screen.
------------------------------------------------------*/
void Pacman::updatePoints(const int& add, const Point& position)
{
	points = points + add;
	if (Point::getDrawIt())
		printPointsToScreen(position);
}

/* ------- printLivesToScreen --------
* Prints pacman's lives to the screen.
------------------------------------*/
void Pacman::printLivesToScreen(const Point& position, const bool& colorMode) const
{
	// clear space
	gotoxy(position.getX() + 15, position.getY() + 1);
	cout << "   ";
	
	char ch = 3; // ascii code of heart
	for (int i = 0; i < life; i++)
	{
		if (colorMode) // colored hearts if game is colored
			setTextColor(Color::LIGHTRED);
		gotoxy(position.getX() + 15 + i, position.getY() + 1);
		cout << ch;
	}
	setTextColor(Color::WHITE);
	gotoxy(0, 0);
}

/* ------- printPointsToScreen --------
* Prints pacman's points to the screen.
-------------------------------------*/
void Pacman::printPointsToScreen(const Point& position) const
{
	// clear space
	gotoxy(position.getX() + 9, position.getY() + 1);
	cout << "    ";

	// print points
	gotoxy(position.getX() + 9, position.getY() + 1);
	cout << points;

	gotoxy(0, 0);
}