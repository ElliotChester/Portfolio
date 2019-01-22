#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include "Node.h"
#include "Grid.h"

class GridCreator
{
public:
	GridCreator(float x, float y);
	~GridCreator();

	Grid* grid = new Grid();

	Node* firstNode;

private:
	float currentx;
	float currenty;

	int width = 10;
	int height = 10;
};

