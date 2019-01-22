#include "stdafx.h"
#include "Sequence.h"

bool Sequence::run() // this needs each of its children to return true
{
	for (Node* child : getChildren()) //loops through all of its children until one returns false
	{ 
		if (!child->run()) 
		{
			std::cout << std::endl;
			return false;
		}
	}
	return true;  // if all children return true the selector will return true
}