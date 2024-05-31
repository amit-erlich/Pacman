#pragma once
#include "Creature.h"


class Ghost: public Creature 
{
public:
	/* C-tor */
	Ghost(int x = 1, int y = 1);

	/* Move Ghost Function */
	virtual void moveGhost(char ch, int dest_x, int dest_y, int dirIndx = -1) = 0;
};


