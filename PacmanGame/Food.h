#pragma once
#include "Point.h"

using namespace std;

class Food {
	int numberOfFood;

public:
	/* C-tor */
	Food();

	/* Set andGet Function */
	void setNumberOfFood(int _numberOfFood);
	int getNumberOfFood() const;

	/* Update Number Of Food Function */
	void decreasNumberOfFood();



};