#include "stdafx.h"
#include "RiverCrossedNode.h"

bool RiverCrossedNode::run()
{
	std::cout << "Have we crossed The river?" << std::endl;
	return  Blackboard::Instance()->riverCrossed; //returns the value of a bool in the blackboard
}
