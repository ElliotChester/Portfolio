#include "stdafx.h"
#include "Inverter.h"

void Inverter::AddChild(Node* newChild) //used to set the child of this node
{
	child = newChild;
}

bool Inverter::run() //This node is used to invert the result of any given node
{
	if (child != nullptr) //makes sure there is a child set
	{
		//std::cout << "Inverting" << std::endl;
		return !child->run();
	}
	else 
	{
		std::cout << "There is no child in the inverter" << std::endl;
		return false;
	}
}
