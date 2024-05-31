#include <windows.h>
#include <iostream>
#include "Food.h"

using namespace std;

/*------------------- Food ------------------
* Initialize food with 0.
--------------------------------------------*/
Food::Food()
{
	numberOfFood = 0;
}

/*------------------- setNumberOfFood ------------------*/
void Food::setNumberOfFood(int _numberOfFood)
{
	numberOfFood = _numberOfFood;
}

/*----------- getNumberOfFood -----------
* Return: number of food left on board
----------------------------------------*/
int Food::getNumberOfFood() const
{
	return numberOfFood;
}

/*---------- decreasNumberOfFood ----------
*Decreasing number of food left in board
-------------------------------------------*/
void Food::decreasNumberOfFood()
{
	numberOfFood--;
}