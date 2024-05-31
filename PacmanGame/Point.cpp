#include "Point.h"

/*initialize static parameters*/
int Point::currBoardWidth = 0;
int Point::currBoardLength = 0;
bool Point::drawIt = true;

/* --------------- init ---------------
* Get:  x1, y1, figure1, color
* 
* Initialize point with default values.
-------------------------------------*/
void Point::init(int x1, int y1, char figure1, Color color)		 
{
	x = x1;
	y = y1;
	figure = figure1;
	this->color = color;
}

void Point::set(const Point& p)
{
	x = p.x;
	y = p.y;
	figure = p.figure;
	drawPoint();
}

/* ------ setColor ------
* Get: color.
-------------------------*/
void Point::setColor(Color color)
{
	this->color = color;
}

/* -- setX --
* Get: x.
-----------*/
void Point::setX(int xPos)
{
	x = xPos;
}

/* -- setY --
* Get: y.
-----------*/
void Point::setY(int yPos)
{
	y = yPos;
}

/* --- setFigure ---
* Get: Figure.
--------------------*/
void Point::setFigure(char ch)
{
	figure = ch;
}

/* ---- setWidthLength ----
* Get: width and length
---------------------------*/
void Point::setWidthLength(const int& w, const int& l)
{
	Point::currBoardWidth = w;
	Point::currBoardLength = l;
}

void Point::setDoNotDrawIt()
{
	Point::drawIt = false;
}

/* -- getX --
* Return: x.
-----------*/
int Point::getX()const
{
	return x;
}

/* -- getY --
* Return: y.
-----------*/
int Point::getY()const
{
	return y;
}

char Point::getFigure()const
{
	return figure;
}

int Point::getWidth()
{
	return Point::currBoardWidth;
}

int Point::getLength()
{
	return Point::currBoardLength;
}

bool Point::getDrawIt()
{
	return Point::drawIt;
}

/* --------------- isColorSet ---------------
* Return: true if user chose colorful game,
          false otherwise
--------------------------------------------*/
bool Point::isColorSet()const
{
	if (color != Color::WHITE)
		return true;
	return false;
}

/* ---------- draw -----------
* Get: ch
* 
* Print ch at (x, y) position.
----------------------------*/
void Point::draw(char ch)const
{
	if (drawIt)
	{
		gotoxy(x, y);
		cout << ch;
		gotoxy(0, 24);
	}
}

/* ---------- drawPoint ----------
* Print figure at (x, y) position.
--------------------------------*/
void Point::drawPoint() 
{
	if (drawIt)
	{
		setTextColor(color);
		gotoxy(x, y);
		cout << figure;
		gotoxy(0, 24);
		setTextColor(Color::WHITE);
	}
}

/* -------------------- move --------------------
* Get: dir
* 
* Moves the point with the direction it received.
-----------------------------------------------*/
void Point::move(const Directions& dir)
{
	switch (dir) 
	{
	case Directions::UP:
		y--;
		if (y < 0) {
			y = Point::currBoardWidth - 1;
		}
		break;
	case Directions::DOWN:
		y++;
		if (y >= Point::currBoardWidth) {
			y = 0;
		}
		break;
	case Directions::LEFT:
		x--;
		if (x < 0) {
			x = Point::currBoardLength - 1;
		}
		break;
	case Directions::RIGHT:
		x++;
		if (x >= Point::currBoardLength) {
			x = 0;
		}
		break;
	}
	// in case of Directions::STAY change nothing
}

/* ---------- clear ----------
* GET: position
* Print ' ' in giving position.
--------------------------------*/
void clear(const Point& p)
{
	if (Point::drawIt)
	{
		gotoxy(p.getX(), p.getY());
		cout << (char)Characters::SPACE;
		gotoxy(0, 24);
	}
}

/* ---------------- compare ----------------
* GET: two positions
* RETURN: true if they are equal,
		  false othrwise.
* This function compares two positions
--------------------------------------------*/
bool compare(const Point& p1, const Point& p2)
{
	if ((p1.x == p2.x) && (p1.y == p2.y))
		return true;
	return false;
}