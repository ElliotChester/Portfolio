#include "stdafx.h"
#include "Selector.h"

bool Selector::run()	//This node will only allow one of its children to return true, 
{
	for (Node* child : getChildren()) //runs through each child until one is true, then returns and the rest will not run
	{
		if (child->run()) 
		{
			std::cout << std::endl;
			return true;
		}
	}
	std::cout << std::endl;	
	return false;	//if none return true the selector will return false
}
