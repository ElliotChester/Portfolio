#include "Grid.h"


Grid::Grid()
{
}

Grid::Grid(int size)
{
	if (size < 4)																// i wanted the minimum size of the grid to be 4x4 so if you try to make the grid with a size of less than 4, it will set the size to 4 
	{
		gridSize = 4;
	}
	else
	{
		gridSize = size;
	}

	// this creates the grid for use in the program
	for (int currentx = 0; currentx < gridSize; currentx++)						// this will loop through and create all of the columns needed in this grid
	{
		std::vector<Node*> column;
		
		for (int currenty = 0; currenty < gridSize; currenty++)					//this will loop through and create each node in the column
		{
			Node* node = new Node();
			if (currentx == 2 && currenty == 2) 
			{
				startNode = node;
			}

			node->coordinates.push_back(currentx);								// sets the grid coordinates of the node
			node->coordinates.push_back(currenty);
			node->initTextures();
			column.push_back(node);												// adds the new node to the column

			node->setOrigin(50, 50);											// sets the centre of the sprite to the centre of the node
			node->setScale(0.01 * (800 / gridSize), 0.01 * (800 / gridSize));	// sets the scale of the node so that the sprite will display at the correct size


			//cout << node[0] << ", " << node[1] << endl;
		}
		Content.push_back(column);												// adds the column to the grid
	}

	targetNode = nullptr;
	startNode->start = true;
	nextCheck.push_back(startNode); //ads the start node to the next check vector and the priority queue so that the pathfinding knows where to start
	priorityQueue.push(startNode);
	//std::cout << "madeGrid" << std::endl;

}

Grid::~Grid()
{
}

void Grid::Display()
{
	float nodeSize = (800 / gridSize) / 2; // node size is used to set the position of the node as the centre of it rather than the top left

	for (size_t i = 0; i < Content.size(); i++)
	{
		std::vector<Node*> column = Content[i];
		for (float n = 0; n < column.size(); n++)
		{
			Node* node = column[n];

			node->xPos = (node->coordinates[0] / gridSize) * 800 + nodeSize;		//this sets the position values of the node that will be used to draw it to the window
			node->yPos = (node->coordinates[1] / gridSize) * 800 + nodeSize + 75;	//the +75 here is used to offset the grid from the top of the screen so that the text is visible

			node->setPosition(node->xPos, node->yPos);								//sets the transform position of the node;

			node->setSpriteColor();													//this function is used to show the different stages of pathfinding

			/* I used quads before sprites because it allowed me to see the grid without having any assets

			glBegin(GL_QUADS);

			glColor3f(0.3, 0.8, 1.0);

			if (node->start == true) {
			glColor3f(0.8, 0.0, 0.0);
			}

			if (node->obstruction == true) {
			glColor3f(0.4, 0.4, 0.4);
			if (node->checked) {
			glColor3f(0.4, 0.4, 0.4);
			}
			}
			else
			if (node->cost > 1) {
			glColor3f(1.0, 1.0, 1.0);
			if (node->checked) {
			glColor3f(1.0, 1.0, 1.0);
			}
			}
			else
			if (node->checked == true)
			{
			glColor3f(0.2, 0.7, 0.9);
			}

			if (node->path == true) {
			glColor3f(0.3, 0.8, 1.0);
			}



			if (node->start == true) {
			glColor3f(0.8, 0.0, 0.0);
			}

			if (node->target == true) {
			glColor3f(0.4, 1.0, 0.4);
			}



			glVertex2f(node->xPos - nodeSize, node->yPos + nodeSize); //Top Left
			glVertex2f(node->xPos + nodeSize, node->yPos + nodeSize); //Top Right
			glVertex2f(node->xPos + nodeSize, node->yPos - nodeSize); //Bottom Right
			glVertex2f(node->xPos - nodeSize, node->yPos - nodeSize);//Bottom Left
			glEnd();*/

		}
	}
}

//This sets all nodes in the grid to not be the target
void Grid::ResetTarget()									
{
	for (size_t i = 0; i < Content.size(); i++)
	{
		for (size_t j = 0; j < Content[i].size(); j++)
		{
			Content[i][j]->target = false;
		}
	}
}

// searches through all of the nodes in the grid to find the one marked as target, then sets the target node pointer to that node
void Grid::SetTarget() {
	for (size_t i = 0; i < Content.size(); i++)
	{
		for (size_t j = 0; j < Content[i].size(); j++)
		{
			if (Content[i][j]->target) {
				targetNode = Content[i][j];
				break;
			}
		}
	}
}

// This is used to set the path when the target has been found from any pathfinding method
void Grid::SetPath(Node* node)
{
	node->path = true;
	if (node->Parent != nullptr) // it goes up through the parents of each node, sets them to path, and sets the child so that it can be used for steeringbehavious
	{

		SetPath(node->Parent);
		node->Parent->Child = node;

	}
	else
	{
		PathSet = true;			// if there are no more parents then that is the end of the path
	}
}

// this fundtion loops through all of the nodes in next check and calls the function to check their naighbours
void Grid::BreadthFirstSearch()
{
	std::vector<Node*>  tempCheck = nextCheck;
	nextCheck = std::vector<Node*>();
	if (targetFound)
	{
		SetPath(targetNode);
	}
	else
	{
		for (size_t i = 0; i < tempCheck.size(); i++) //loops through and checks the neighbours of all of the nodes in the next check vector
		{
			if (targetFound) { break; }

			BreadthCheckNode(tempCheck[i]);
		}
	}

}

// this function is used to check all of the neighbours of a node to see if it has already been checked, and if it is obstructed
void Grid::BreadthCheckNode(Node* node) 
{

#pragma region SettingNeighbours
	// creates a vector and stores all the neighbours of the node in it
	std::vector<Node*> neighbours;
	float newCoordinatesX = node->coordinates[0] + 0;
	float newCoordinatesY = node->coordinates[1] + -1;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size()) // checks that the node is actually in the grid
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + 1;
	newCoordinatesY = node->coordinates[1] + 0;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + 0;
	newCoordinatesY = node->coordinates[1] + 1;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + -1;
	newCoordinatesY = node->coordinates[1] + 0;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}
#pragma endregion

	for (size_t i = 0; i < neighbours.size(); i++)	//loops through all of the neighbours
	{
		Node* Next = neighbours[i];
		node->checked = true;

		if (!Next->checked || !Next->obstruction)			//checks to see if the next node has already been checked, or is an obstruction, if it is then it returns and ignores the node
		{
			return;
		}

		if (Next->target == true)							// if the next node is the target it set the target node to that node and change the targetfound bool to true so that the path can be made
		{													
			//std::cout << "found Target" << std::endl;
			node->checked = true;
			targetNode = Next;
			targetNode->Parent = node;
			targetFound = true;
		}
		else
		{													// if the node is not checked, not an obstruction, and not the target, it will set the parent to the current node, and add the next node to next check
			Next->Parent = node;
			bool alreadyInNextCheck = false;
			for (size_t j = 0; j < nextCheck.size(); j++)
			{
				Node* checkNode = nextCheck[j];
				if (checkNode == Next) {
					alreadyInNextCheck = true;
				}
			}

			if (alreadyInNextCheck == false) 
			{
				nextCheck.push_back(Next);
			}
		}
		
	}
}

// Dijkstras works much like breadth first, except instead of searching all nodes in a vector, it will only search the node at the top of the priority queue
void Grid::DijkstrasSearch()
{
	if (targetFound)
	{
		SetPath(targetNode);
	}
	else
	{
		if (priorityQueue.size() == 0) {
			DijkstrasCheckNode(startNode);
		}
		else
		{
			Node* firstNode = priorityQueue.top();
			priorityQueue.pop();
			DijkstrasCheckNode(firstNode);
		}
	}
}

// checking a dijkstras node is the same as breadth first, except instead of adding neighbours to the nextcheck vector it adds them to the priority queue
void Grid::DijkstrasCheckNode(Node* node)
{

#pragma region SettingNeighbours
	std::vector<Node*> neighbours;
	float newCoordinatesX = node->coordinates[0] + 0;
	float newCoordinatesY = node->coordinates[1] + -1;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + 1;
	newCoordinatesY = node->coordinates[1] + 0;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + 0;
	newCoordinatesY = node->coordinates[1] + 1;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + -1;
	newCoordinatesY = node->coordinates[1] + 0;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}
#pragma endregion

	for (size_t i = 0; i < neighbours.size(); i++)
	{
		Node* Next = neighbours[i];

		if (!Next->obstruction && !Next->start)
		{
			if (Next->target == true)
			{
				//std::cout << "found Target" << std::endl;
				targetNode = Next;
				targetNode->Parent = node;
				targetFound = true;
			}
			else
			{
				int newCost = node->CostSoFar + Next->cost;
				if (Next->CostSoFar == 0 || newCost < Next->CostSoFar)		//this will find the neighbour with the lowest costsofar and add it to the priority queue
				{
					Next->CostSoFar = newCost;
					Next->Priority = newCost;
					priorityQueue.push(Next);
					Next->Parent = node;

					//std::cout << "Set a Node, setting cost to " << newCost << std::endl;
				}
			}
			neighbours[i]->checked = true;
		}
	}
}

// astar search is the same as dijkstras but it calls astar check node rather than dijkstras check node
void Grid::AStarSearch()
{

	if (targetFound)
	{
		SetPath(targetNode);
	}
	else
	{
		Node* firstNode = priorityQueue.top();
		priorityQueue.pop();
		AStarCheckNode(firstNode);
	}
}

// astar works much like dijkstras, except it will find the heuristic and add it to the costsofar to make a priority value that will be used in the priority queue
void Grid::AStarCheckNode(Node* node)
{
	std::vector<Node*> neighbours;

	//astar will also check its diagonal neighbours
#pragma region SettingNeighbours
	float newCoordinatesX = node->coordinates[0] + 0;
	float newCoordinatesY = node->coordinates[1] + -1;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + 1;
	newCoordinatesY = node->coordinates[1] + -1;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + 1;
	newCoordinatesY = node->coordinates[1] + 0;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + 1;
	newCoordinatesY = node->coordinates[1] + 1;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + 0;
	newCoordinatesY = node->coordinates[1] + 1;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + -1;
	newCoordinatesY = node->coordinates[1] + 1;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + -1;
	newCoordinatesY = node->coordinates[1] + 0;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + -1;
	newCoordinatesY = node->coordinates[1] + -1;
	if (newCoordinatesX >= 0 && newCoordinatesY >= 0 && newCoordinatesX < Content.size() && newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}
#pragma endregion

	for (size_t i = 0; i < neighbours.size(); i++)
	{
		if (!neighbours[i]->obstruction && !neighbours[i]->start)
		{
			Node* Next = neighbours[i];

			if (Next->target == true)
			{
				//std::cout << "found Target" << std::endl;
				targetNode = Next;
				targetNode->Parent = node;
				targetFound = true;
			}
			else
			{
				int newCost = node->CostSoFar + Next->cost;
				if (Next->CostSoFar == 0 || newCost < Next->CostSoFar)
				{
					Next->CostSoFar = newCost;
					Next->Priority = newCost + Heuristic(targetNode, Next); // the heuristic value is the distance between the node we are checking and the target. using this allows the program to be more efficient as it wont have to flood search

					Next->Parent = node;
					Next->checked = true;
					priorityQueue.push(Next);
					//std::cout << "Set a Node, setting cost to " << newCost << std::endl;
				}
			}
		}
	}
}

// a function that returns the huristic value of 2 nodes
double Grid::Heuristic(Node* target, Node* next)
{

	return abs(target->coordinates[0] - next->coordinates[0]) + abs(target->coordinates[1] - next->coordinates[1]);
}

// this will reset all of the nodes to unchecked so that the grid can pathfind again
void Grid::ResetNodes()
{
	//std::cout << "Reset" << std::endl;
	nextCheck.clear();
	nextCheck.push_back(startNode);
	priorityQueue = std::priority_queue<Node*, std::vector<Node*>, NodeCompare>();
	priorityQueue.push(startNode);
	//std::cout << priorityQueue.size() << std::endl;
	targetFound = false;
	PathSet = false;

	for (size_t i = 0; i < Content.size(); i++)
	{
		for (size_t j = 0; j < Content[i].size(); j++)
		{
			Content[i][j]->checked = false;
			Content[i][j]->CostSoFar = 0;
			Content[i][j]->path = false;
			targetFound = false;
			Content[i][j]->Parent = nullptr;
			Content[i][j]->Child = nullptr;
		}
	}
}
