#include "GhostBest.h"

/* ------------ GhostBest ------------
* Sends values to Ghost c-tor and 
* initializes the rest of the members.
------------------------------------*/
GhostBest::GhostBest(int x, int y)
	:Ghost(x, y) 
{
    count5steps = 0; 
    pathmoves = "-1";
}

/* -------- setCount5steps --------- */
void GhostBest::setCount5steps(int num)
{
    count5steps = num;
}

/* ---------------------------------- moveGhost -----------------------------------
* Moves the ghost accordinaly to the char in pathmoves string in place count5steps.
* If sting is empty (= "-1" ) moves the ghost randomly.
---------------------------------------------------------------------------------*/
void GhostBest::moveGhost(char ch, int dest_x, int dest_y, int dirIndx)
{
    // calculate new path from ghost to pacman every 5 steps
    if (count5steps == 0)
        pathMoves(Position{ dest_x, dest_y });
    
    // set ghost direction accordinaly to the char in pathmoves
    if ((pathmoves != "-1") && (pathmoves.length() - 1 >= count5steps))
    {
        switch (pathmoves[count5steps])
        {
        case 'D':
            setDirection(Directions::DOWN);
            break;
        case 'U':
            setDirection(Directions::UP);
            break;
        case 'R':
            setDirection(Directions::RIGHT);
            break;
        default: // case 'L':
            setDirection(Directions::LEFT);
            break;
        }
    }
	move(ch);
    count5steps++;

    if (count5steps == 5)
        count5steps = 0;
}


/* ------------------------ pathMoves ---------------------------
* Get: dest
* 
* Finds the shortest path from ghost to destination in the board.
* If path not found: pathmoves = "-1".
---------------------------------------------------------------*/
// Function to find the shortest path from the source to destination in the given  matrix
void GhostBest::pathMoves(Position dest)
{
    Position src = { this->getPosition().getX() , this->getPosition().getY() };
    Board board = Board();

    // Stores the moves of the directions of adjacent cells
    int dWidth[] =  { -1, 0, 0, 1 };
    int dLength[] = { 0, -1, 1, 0 };

    // Stores the distance for each cell from the source cell
    int d[BoardSize::WIDTH][BoardSize::LENGTH + 1];
    memset(d, -1, sizeof d);

    // Distance of source cell is 0
    d[src.y][src.x] = 0;

    // Initialize a visited array
    bool visited[BoardSize::WIDTH][BoardSize::LENGTH + 1];
    memset(visited, false, sizeof visited);

    // Mark source cell as visited
    visited[src.y][src.x] = true;

    // Create a queue for BFS
    queue<Node> q;

    // Distance of source cell is 0
    Node s = { src, 0 };

    // Enqueue source cell
    q.push(s);

    // Keeps track of whether destination is reached or not
    bool ok = false;

    // Iterate until queue is not empty
    while (!q.empty()) 
    {

        // Deque front of the queue
        Node curr = q.front();
        Position pt = curr.pt;

        // If the destination cell is reached, then find the path
        if (pt.x == dest.x && pt.y == dest.y) 
        {

            int xx = pt.x, yy = pt.y;
            int dist = curr.dist;

            // Assign the distance of destination to the distance matrix
            d[pt.y][pt.x] = dist;

            // Stores the smallest path
            pathmoves = "";

            // Iterate until source is reached
            while (xx != src.x || yy != src.y) 
            {
                if (yy > 0 && d[yy - 1][xx] == dist - 1) // Append D
                {
                    pathmoves += 'D';
                    yy--;
                }
                if (yy < BoardSize::WIDTH - 1 && d[yy + 1][xx] == dist - 1) // Append U
                {
                    pathmoves += 'U';
                    yy++;
                }
                if (xx > 0 && d[yy][xx - 1] == dist - 1) // Append R
                {
                    pathmoves += 'R';
                    xx--;
                }
                if (xx < (BoardSize::LENGTH - 1) && d[yy][xx + 1] == dist - 1) // Append L
                {
                    pathmoves += 'L';
                    xx++;
                }
                dist--;
            }

            // Reverse the backtracked path
            reverse(pathmoves.begin(),
                pathmoves.end());

            ok = true;
            break;
        }

        // Pop the start of queue
        q.pop();

        Point tempP;
        // Explore all adjacent directions
        for (int i = 0; i < 4; i++) 
        {
            int width = pt.y + dWidth[i];
            int length = pt.x + dLength[i];
            tempP.init(length, width);

            // If the current cell is valid cell and can be traversed
            if (isValid(width, length) 
                && (board.getCharInBoard(tempP) != Characters::WALL) && (board.getCharInBoard(tempP) != Characters::INVISIBLE_TUNNEL) 
                && !visited[width][length])
            {
                // Mark the adjacent cells as visited
                visited[width][length] = true;

                // Enqueue the adjacent cells
                Node adjCell
                    = { { length, width }, curr.dist + 1 };
                q.push(adjCell);

                // Update the distance of the adjacent cells
                d[width][length] = curr.dist + 1;
            }
        }
    }

    // If the destination is not reachable
    if (!ok)
        pathmoves = "-1";
}

/* --------------- isValid ---------------
* Get: row, col
* Return: true if valid, else - false.
* 
* Check if the given cell is valid or not.
----------------------------------------*/
bool GhostBest::isValid(int row, int col)
{
    return (row >= 0) && (col >= 0)
        && (row < this->getPosition().getWidth()) && (col < this->getPosition().getLength());
}