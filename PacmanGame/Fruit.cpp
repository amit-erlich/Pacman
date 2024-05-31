#include "Fruit.h"

/*------------------- Fruit ------------------
* Initialize Fruit with defult values.
--------------------------------------------*/
Fruit::Fruit(int x, int y):Creature(x, y, '5', Directions::UP), stepsLeft(10), fruitIndxInList(0) {}

/*------------------- getNumberOfSteps --------------------------
* RETURN: how much steps left to the fruit until it disappears
-----------------------------------------------------------------*/
int Fruit::getNumberOfSteps() const
{
	return stepsLeft;
}

/*------------------- getPoints -------------------
* RETURN: how much points the fruit is worth.
---------------------------------------------------*/
int Fruit::getPoints()
{
	return (getFigure() - 48); //from char to int
}

vector<Point> Fruit::getAllFruits() const
{
	return allFruits;
}

/*------------------- reset -------------------
* reset fruit stepsLeft,figure position.
----------------------------------------------*/
void Fruit::reset(const Point& position)
{
	stepsLeft = 10;
	setFigure(randomNumBetween5to9());
	setDirection((Directions)getRandNumFrom0To3());
	setPosition(position.getX(), position.getY());
}

/*------------------- reset -------------------
* reset fruit stepsLeft,figure position.
----------------------------------------------*/
void Fruit::reset()
{
	stepsLeft = 10;
	if (fruitIndxInList < allFruits.size())
		getPosition().set(allFruits[fruitIndxInList++]);
}

void Fruit::addFruit()
{
	allFruits.push_back(this->getPosition());
}

void Fruit::addFruit(Point fruit)
{
	allFruits.push_back(fruit);
}

void Fruit::clearAllFruits()
{
	allFruits.clear();
	fruitIndxInList = 0;
}

/*---------- randomNumBetween5to9 ------------*/
char Fruit::randomNumBetween5to9()
{
	return (char)((rand() % 5) + 5 + 48); // (char)48 = '0'
}

/*---------- decreasNumberOfSteps ------------*/
void Fruit::decreasNumberOfSteps()
{
	stepsLeft--;
}