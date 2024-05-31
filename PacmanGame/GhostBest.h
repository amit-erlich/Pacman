#pragma once
#include <queue>
#include "Ghost.h"
#include "Board.h"

// Stores the coordinates of the matrix cell
struct Position {
	int x, y;
};

// Stores coordinates of a cell and its distance
struct Node {
	Position pt;
	int dist;
};

class GhostBest : public Ghost
{
	int count5steps;
	string pathmoves;
public:
	/* C-tor */
	GhostBest(int x = 1, int y = 1);

	/* Set Function */
	void setCount5steps(int num);

	/* Move Ghost Function */
	virtual void moveGhost(char ch, int dest_x, int dest_y, int dirIndx = -1) override;

	void pathMoves(Position dest);

	// Check Functions
	bool isValid(int row, int col);
};