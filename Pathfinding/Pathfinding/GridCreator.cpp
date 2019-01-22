#include "stdafx.h"
#include "GridCreator.h"

GridCreator::GridCreator(float x, float y)	//this will create an empty grid, with only a start node, it takes 2 floats that will be set as the width and height of the grd
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

	for (currentx = 0; currentx < width; currentx++)		//goes through the columns that will need to be added to the grid
	{
		std::vector<Node*> column;							//creates a vector of nodes that will be in a column
		//cout << "New Column" << endl;
		for (currenty = 0; currenty < height; currenty++)	//loops the same amount as the height of the node
		{
			Node* node = new Node();						//creates a node that will be added to the column
			if (currentx == 2 && currenty == 2)				//sets the start node of the grid
			{
				firstNode = node;
			}
			node->coordinates.push_back(currentx);			//sets the coordinates of the node
			node->coordinates.push_back(currenty);			//

			column.push_back(node);							//Adds the new node to the column

			//cout << node[0] << ", " << node[1] << endl;
		}
		grid->Content.push_back(column);					//adds the column to the grid
	}

	/*//Temporarily used to create obsticals until mouse clicking is implimented
	for (size_t i = 0; i < 15; i++)
	{
		grid->Content[i][3]->obstruction = true;
	}
	for (size_t i = 0; i < 13; i++)
	{
		grid->Content[15][i + 3]->obstruction = true;
	}
	for (size_t i = 0; i < 13; i++)
	{
		grid->Content[i + 3][15]->obstruction = true;
	}*/

											//sets the values needed to start searching
	grid->startNode = firstNode;
	grid->targetNode = nullptr;
	firstNode->start = true;
	grid->nextCheck.push_back(firstNode);
	grid->priorityQueue.push(firstNode); 
}

GridCreator::~GridCreator() {}
