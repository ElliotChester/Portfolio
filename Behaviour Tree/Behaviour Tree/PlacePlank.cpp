#include "stdafx.h"
#include "PlacePlank.h"

bool PlacePlank::run() //This is used to set the value of a bool
{
	if (!Blackboard::Instance()->hasPlank) 
	{
		std::cout << "We dont have the plank to place" << std::endl;
		return false;
	}
	else 
	{
		std::cout << "Placing the plank over the river" << std::endl;

		Blackboard::Instance()->plankOverRiver = true;
		return true;
	}
	
}
