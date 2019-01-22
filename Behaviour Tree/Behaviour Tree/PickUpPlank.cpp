#include "stdafx.h"
#include "PickUpPlank.h"

bool PickUpPlank::run() //this node is used to set the value of a bool to true in the blackboard
{
	Blackboard::Instance()->hasPlank = true;
	std::cout << "Picked Up Plank" << std::endl;
	return true;
}
