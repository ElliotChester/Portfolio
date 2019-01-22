#include "stdafx.h"
#include "CrossRiver.h"

bool CrossRiver::run()
{
	Blackboard::Instance()->riverCrossed = true; //sets river crossed to true so that the tree does not continue to run anymore
	
	std::cout << "We Crossed the river" << std::endl;

	return true;
}
