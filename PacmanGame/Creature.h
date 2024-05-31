#pragma once
#include <vector>
#include "Point.h"
#include "io_utils.h"

class Creature 
{
	Point body;
	Directions dir;
	string moves;
public:
	/* C-tor */
	Creature(int x, int y, char ch, Directions direction);

	/* Set and Get Functions */
	void setFigure(char ch);
	void setPosition(const int& x, const int& y);
	void setDirection(const Directions& dir);
	void setColor(const Color& color);
	void setMoves(string oldMoves);
	char getFigure()const;
	Directions getDirection(int dirIndx = -1) const;
	Point& getPosition();
	Point getNextPosition(int dirIndx = -1) const;
	string getMoves() const;
	string& getMoves();
	void restartPosition(Point pos, Directions direction = Directions::UP);

	/* Move Ghost Function */
	void move(char ch = ' ', int dirIndx = -1);
	void changeDirection();

	void addMove(const Directions _dir);
	void clearMoves();

	int getRandNumFrom0To3();

	void draw();

	static bool checkMovesValidation(string moves, bool is5ValidChar = false);
};