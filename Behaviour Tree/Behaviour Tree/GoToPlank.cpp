#include "stdafx.h"
#include "GoToPlank.h"

bool GoToPlank::run()	
{
	

	if (Blackboard::Instance()->distanceFromPlank > 0.6) //this checks if we are close to the plank, if not, it will move closer to the plank
	{
		Blackboard::Instance()->distanceFromPlank -= 0.6;
		std::cout << "The plank is " << Blackboard::Instance()->distanceFromPlank << " m away" << std::endl;
		return false; //returns false so that the tree will continue this node until close enough
	}
	else 
	{
		Blackboard::Instance()->distanceFromPlank = 0;	//If close enough it will set the distance to 0 so that the tree can continue
		std::cout << "We're at the plank now" << std::endl;
		return true;	//returns true so that the sequence can continue
	}
}
