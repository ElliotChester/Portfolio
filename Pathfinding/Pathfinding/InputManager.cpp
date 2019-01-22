#include "stdafx.h"
#include "InputManager.h"


InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::CheckForNode(std::vector<std::vector<Node*>> grid, float x, float y, int mouseButton) 
{
	y = -y;

	for (size_t i = 0; i < grid.size(); i++)
	{
		std::vector<Node*> column = grid[i];
		for (size_t j = 0; j < column.size(); j++)
		{
			column[j]->checkClick(x, y, mouseButton);
		}
	}
}
