#include "stdafx.h"
#include "GridCreator.h"

GridCreator::GridCreator(int x, int y)	//This will create the grid so that it can be drawn
{
	if (x <= 0 || y <= 0) {
		width = 1;
		height = 1;
	}
	else
	{
		width = x;
		height = y;
	}

	for (currentx = 0; currentx < width; currentx++)		//creates a vector that will represent the top row of nodes
	{
		vector<vector<float>> column;
		//cout << "New Column" << endl;
		for (currenty = 0; currenty < height; currenty++)	//creates vectors in each point in the top row that will be used to represent the columns
		{
			vector<float> node;			//uses a vecter of floats to represent a node in the grid
			node.push_back(currentx);	//pushes bak the position of the node to the vector
			node.push_back(currenty);

			column.push_back(node);		//adds the node to the column

			//cout << node[0] << ", " << node[1] << endl;
		}
		grid.push_back(column);			//adds the column to the grid
	}
}


GridCreator::~GridCreator()
{
}

vector<vector<vector<float>>>* GridCreator::getGrid() //used to get the grid in other scripts
{
	return &grid;
}
