#pragma once
#include <iostream>
#include "io_utils.h"

using namespace std;

enum BoardSize { LENGTH = 80, WIDTH = 25 };
enum Directions { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, STAY = 4, X, Y };

class Point {
	char figure;
	int x, y; 
	Color color;
	static int currBoardWidth;
	static int currBoardLength;
	static bool drawIt;
public:
	void init(int x1 = 1, int y1 = 1, char figure1 = '@', Color color = Color::WHITE);

	/* Set and Get Functions */
	void set(const Point& p);
	void setColor(Color color);
	void setX(int xPos);
	void setY(int yPos);
	void setFigure(char ch);
	static void setWidthLength(const int& w, const int& l);
	static void setDoNotDrawIt();
	int getX()const;
	int getY()const;
	char getFigure()const;
	static int getWidth();
	static int getLength();
	static bool getDrawIt();

	bool isColorSet()const;

	/* Draw Functions */
	void draw(char ch)const;
	void drawPoint();

	/* Move Point Function */
	void move(const Directions& dir);

	/* Clear Char Function */
	friend void clear(const Point& p);

	/* Comper Function */
	friend bool compare(const Point& p1, const Point& p2);

};