#pragma once
#include "Creature.h"

enum Appear { NO, YES };

class Fruit : public Creature
{
	int stepsLeft;
	vector<Point> allFruits;
	int fruitIndxInList;

public:
	Fruit(int x = 1, int y = 1);

	/* Get and Reset Functions */
	int getNumberOfSteps() const;
	int getPoints();
	vector<Point> getAllFruits() const;
	void reset(const Point& position);
	void reset();

	void addFruit(); // for SAVE mode
	void addFruit(Point fruit); // for LOAD mode
	void clearAllFruits();

	char randomNumBetween5to9();
	void decreasNumberOfSteps();
};