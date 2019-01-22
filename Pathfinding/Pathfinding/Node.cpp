#include "stdafx.h"
#include "Node.h"


Node::Node()
{
}

Node::~Node()
{
}

void Node::checkClick(float x, float y, int mouseButton) //function used to check if the node has been clicked on
{
	//std::cout << x << "x, " << y << "y" << std::endl;
	
	if (x >= xPos && x < (xPos + 0.1f)) 
	{
		if (y <= yPos && y > (yPos - 0.1f)) 
		{
			if (mouseButton == 0 && target != true)		//if the click was left click, this will set an obstruction
			{
				obstruction = !obstruction;
			}
			else
			if(mouseButton == 1)						//if the mouse button was right click// this will set the target
			{
				target = true;
				obstruction = false;
			}
			else										//if it wasnt left or right click it must be middle click, this will set the weight
			{
				if (cost == 3) 
				{
					cost = 0;
				}
				else
				{
					cost = 3;
				}
			}
			//std::cout << "Clicked " << coordinates[0] << " " << coordinates[1] << std::endl;
		}
	}
}
	


