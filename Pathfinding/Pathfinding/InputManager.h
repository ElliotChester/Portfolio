#pragma once
#include <iostream>
#include <vector>
#include "Node.h"
class InputManager
{
public:
	InputManager();
	~InputManager();

	void CheckForNode(std::vector<std::vector<Node*>> grid, float x, float y, int mouseButton);

};

