#include "stdafx.h"
#include "HavePlank.h"

bool HavePlank::run()
{
	std::cout << "Do we have the plank?" << std::endl;
	return Blackboard::Instance()->hasPlank; //simply returns if we have the plank or not
}
