#pragma once
#include <iostream>
#include <vector>

//#include "Grid.h"

class Node
{
public:
	Node();

	~Node();

	bool obstruction = false;
	bool checked = false;
	bool path = false;
	bool target = false;
	bool start = false;

	int cost = 1;

	int CostSoFar = 0;
	double Priority = 0;

	float xPos;
	float yPos;

	std::vector<float> coordinates;

	void checkClick(float x, float y, int mouseButton);

	Node* Parent = nullptr;
};

