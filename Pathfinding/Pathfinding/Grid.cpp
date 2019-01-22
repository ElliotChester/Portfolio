#include "stdafx.h"
#include "Grid.h"


Grid::Grid()
{
}

Grid::~Grid()
{
}

void Grid::ResetTarget()	//used hen removing the target node so that another one can be set
{
	for (size_t i = 0; i < Content.size(); i++)
	{
		for (size_t j = 0; j < Content[i].size(); j++)
		{
			Content[i][j]->target = false;
		}
	}
}

void Grid::SetTarget()		//this sets the target node that will be searched for when pathfinding
{
	for (size_t i = 0; i < Content.size(); i++)
	{
		for (size_t j = 0; j < Content[i].size(); j++)
		{
			if (Content[i][j]->target) 
			{
				targetNode = Content[i][j];
				break;
			}
		}
	}
}

void Grid::SetPath(Node* node)	//once the target node has been found this will be called to set the path.
{
	node->path = true;
	if (node->Parent != nullptr) {
		SetPath(node->Parent);
	}
	else {
		PathSet = true;
	}
}

void Grid::BreadthFirstSearch()	//this is called every frame until the target is found, breadth first search will not take into account the weights of the nodes, or the position of the target, and will search outwards from the start node.
{
	std::vector<Node*>  tempCheck = nextCheck;
	nextCheck = std::vector<Node*>();
	if (targetFound) 
	{
		SetPath(targetNode);
	}
	else
	{
		for (size_t i = 0; i < tempCheck.size(); i++)	//will loop through all of the nodes that need to be checked
		{
			if (targetFound){break;}					//if the target was found since the last node it will stop searching

			BreadthCheckNode(tempCheck[i]);	
		}
	}
	
}

void Grid::BreadthCheckNode(Node* node) //this is the function that will actually perform the search for the target node
{
	
#pragma region SettingNeighbours	
																			//this section will find all of the neighbours that will be needed to perform the next check
	std::vector<Node*> neighbours;
	float newCoordinatesX = node->coordinates[0] + 0;						//finds the grid position of the node above the one being checked
	float newCoordinatesY = node->coordinates[1] - 1;						//

	if (newCoordinatesY >= 0)												//makes sure the position found is actually within the grid
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);	//if within range, it will add the node to the neighbours
	}

	newCoordinatesX = node->coordinates[0] + 1;								//Right node
	newCoordinatesY = node->coordinates[1] + 0;								//
	if (newCoordinatesX < Content.size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + 0;								//Bottom Node
	newCoordinatesY = node->coordinates[1] + 1;								//
	if (newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + -1;							//Left Node
	newCoordinatesY = node->coordinates[1] + 0;								//
	if (newCoordinatesX >= 0 )
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}
#pragma endregion

	for each (Node* Next in neighbours)						//Loops through and checks all of the neighbours
	{
		node->checked = true;
		if (!Next->checked && !Next->obstruction)			//Checks if the node being checked has already been checked or an obstruction
		{
			if (Next->target == true)						//if the node being checked is the target, the target will be set as true so that the search will no longer run
			{
				std::cout << "found Target" << std::endl;
				node->checked = true;
				targetNode = Next;
				targetNode->Parent = node;					//the parent is set so that the path can be found
				targetFound = true;
			}
			else											//if the node is not the target, it will 
			{
				Next->Parent = node;
				bool alreadyInNextCheck = false;			//this had to be added otherwise the node would be put in next check which caused a problem of the same node being added to the vector thousands of times resulting in millions of nodes being in the vector, using lots of memory and bringing the program to a halt
				for each (Node* checkNode in nextCheck)		
				{										
					if (checkNode == Next)				
					{	
						alreadyInNextCheck = true;
					}
				}

				if (alreadyInNextCheck == false) 
				{
					nextCheck.push_back(Next);				//if the node isnt already in the vector, it will be added to be checked
				}
			}
		}
		
	}
}

void Grid::DijkstrasSearch()	//Dijkstras search will take into account the weights of the nodes it checks, then each frame will check the node that has the least weight path. It doesnt take into account the position of the target
{	

	if (targetFound)			//if the target has already been found it will get the path 
	{
		SetPath(targetNode);
	}
	else
	{
		if (priorityQueue.size() == 0)		//a priorty queue is used to find the lowest cost path,
		{
			DijkstrasCheckNode(startNode);	//if this is the first run through it will start checking from the start node
		}
		else
		{
			Node* firstNode = priorityQueue.top();	//if there is already something in the priority queue, it will take the one from the top, which has the lowest cost
			priorityQueue.pop();					//it will then remove this from the queue so it wont be checked again
			DijkstrasCheckNode(firstNode);			//it then searches from the node that was removed from the queue
		}
	}
}

void Grid::DijkstrasCheckNode(Node* node)
{
	
#pragma region SettingNeighbours
																			//finds the neightbours the same way as breadth first
	std::vector<Node*> neighbours;
	float newCoordinatesX = node->coordinates[0] + 0;						
	float newCoordinatesY = node->coordinates[1] - 1;						

	if (newCoordinatesY >= 0)												
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);	
	}

	newCoordinatesX = node->coordinates[0] + 1;								
	newCoordinatesY = node->coordinates[1] + 0;								
	if (newCoordinatesX < Content.size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + 0;								
	newCoordinatesY = node->coordinates[1] + 1;								
	if (newCoordinatesY < Content[newCoordinatesX].size())
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}

	newCoordinatesX = node->coordinates[0] + -1;							
	newCoordinatesY = node->coordinates[1] + 0;								
	if (newCoordinatesX >= 0)
	{
		neighbours.push_back(Content[newCoordinatesX][newCoordinatesY]);
	}
#pragma endregion
	
	for (size_t i = 0; i < neighbours.size(); i++) //loops through the neighbours
	{
		Node* Next = neighbours[i];

		if (!Next->obstruction && !Next->start)
		{
			if (Next->target == true)
			{
				std::cout << "found Target" << std::endl;
				targetNode = Next;
				targetNode->Parent = node;
				targetFound = true;
			}
			else 
			{
				int newCost = node->CostSoFar + Next->cost;				//this calculates the cost of the path includeing the next node

				if (Next->CostSoFar == 0 || newCost < Next->CostSoFar)	//checks if the node has already been checked, if it has been given a cost higher than this new one, 
				{														//or not been given one at all, it will be given this new cost and added to the priority queue
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

void Grid::AStarSearch()	//Astar search will take into account the weight of a node aswell as the direction of the target
{

	if (targetFound)
	{
		SetPath(targetNode);
	}
	else
	{
		Node* firstNode = priorityQueue.top();	//also uses a priority queue
		priorityQueue.pop();
		AStarCheckNode(firstNode);
	}
}

void Grid::AStarCheckNode(Node* node)
{
#pragma region SettingNeighbours 
														//finds neighbours like the other searches, however takes into account diagonal neighbors
	std::vector<Node*> neighbours;
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
				std::cout << "found Target" << std::endl;
				targetNode = Next;
				targetNode->Parent = node;
				targetFound = true;
			}
			else
			{
				int newCost = node->CostSoFar + Next->cost;
				if (Next->CostSoFar == 0 || newCost < Next->CostSoFar)		//does the same check as dijkstras
				{
					Next->CostSoFar = newCost;
					Next->Priority = newCost + Heuristic(Next,targetNode);	//adds the new cost of the path, aswell as the cost given by the distance between the target and next node
					
					Next->Parent = node;
					Next->checked = true;
					priorityQueue.push(Next);
				}
			}
		}
	}
}

double Grid::Heuristic(Node* target, Node* next)	//function used to get the huristic value between 2 nodes
{
	return abs(target->coordinates[0] - next->coordinates[0]) + abs(target->coordinates[1] - next->coordinates[1]);
}

void Grid::ResetNodes()	//function used to reset the nodes in the grid, aswell as everything to do with searching, this allows the search to begin again, or switch search method
{
	std::cout << "Reset" << std::endl << std::endl;
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
		}
	}
}
