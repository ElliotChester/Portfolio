#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>

using namespace std;

class GridCreator
{
public:
	GridCreator(int x, int y);
	~GridCreator();

	vector<vector<vector<float>>>* getGrid();

private:

	vector<vector<vector<float>>> grid;

	float currentx;
	float currenty;

	int width = 10;
	int height = 10;
};

