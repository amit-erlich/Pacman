#include "Ghost.h"

/* -------------------------------- Ghost ----------------------------------
* Get: x, y
*
* Initialized the ghost with default values (or with the values it received).
* Draw the ghost in its start position.
---------------------------------------------------------------------------*/
Ghost::Ghost(int x, int y)
	:Creature(x, y, Characters::GHOST, Directions::UP) {}