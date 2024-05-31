#include "GhostNovice.h"

/* ------- GhostNovice -------
* Sends values to Ghost c-tor.
----------------------------*/
GhostNovice::GhostNovice(int x, int y)
	:Ghost(x, y) {}

/* --------------------- moveGhost ----------------------
* Get: ch, dest_x, dest_y
* 
* Moves the ghost and draw ch in ghost previous position.
-------------------------------------------------------*/
void GhostNovice::moveGhost(char ch, int dest_x, int dest_y, int dirIndx)
{
	move(ch, dirIndx);
}