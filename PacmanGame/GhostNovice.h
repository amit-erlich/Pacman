#pragma once
#include "Ghost.h"

class GhostNovice : public Ghost
{

public:
	/* C-tor */
	GhostNovice(int x = 1, int y = 1);

	/* Move Ghost Function */
	virtual void moveGhost(char ch, int dest_x, int dest_y, int dirIndx = -1) override;
};