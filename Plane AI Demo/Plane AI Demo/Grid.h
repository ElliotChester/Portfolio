#pragma once


#include <iostream>
#include <vector>
#include <queue>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "Node.h"
#include "UtilRandom.h"


class Grid
{
public:
	Grid();
	Grid(int size);

	~Grid();

	std::vector<std::vector<Node*>> Content;

	int newCoordinates;

	int gridSize = 20;

	bool targetFound = false;
	bool PathSet = false;

	void ResetTarget();
	void SetTarget();
	void SetPath(Node* node);

	void BreadthFirstSearch();
	void BreadthCheckNode(Node* node);

	void DijkstrasSearch();
	void DijkstrasCheckNode(Node* node);

	void AStarCheckNode(Node* node);
	void AStarSearch();

	double Heuristic(Node* target, Node* next);

	void ResetNodes();

	void Display();

	void AssignTargetNode(int x, int y) // This will reset all nodes and set a new target node with the coordinates given in the arguments
	{
		ResetNodes();
		ResetTarget();
		Content[x][y]->target = true;
		SetTarget();
	}

	void AssignStartNode(int x, int y) // this assigns a new start node and adds it to the next check vector and priority queue
	{
		startNode->start = false;
		startNode = Content[x][y];
		startNode->start = true;
		nextCheck.push_back(startNode);
		priorityQueue.push(startNode);
	}

	void SetRandomObsticals(int amt) // this will set random obsticals accross the grid
	{
		for (size_t i = 0; i < Content.size(); i++)
		{
			for (size_t j = 0; j < Content[0].size(); j++) // clears all current obsticals
			{
				Content[i][j]->obstruction = false;
				Content[i][j]->SetSprite();
			}
		}

		for (size_t i = 0; i < amt; i++) // creates new obsticals
		{
			int randomX = UtilRandom::instance()->GetRange(0, Content.size());
			int randomY = UtilRandom::instance()->GetRange(0, Content[0].size());
			if (!Content[randomX][randomY]->target) {
				Content[randomX][randomY]->obstruction = true;
				Content[randomX][randomY]->SetSprite();
			}
		}
	}

	void SetRandomCosts(int amt) // this function will set higher weights to random nodes accross the grid
	{
		for (size_t i = 0; i < Content.size(); i++)
		{
			for (size_t j = 0; j < Content[0].size(); j++) //sets all costs to 1
			{
				Content[i][j]->cost = 1;
				Content[i][j]->SetSprite();
			}

		}

		for (size_t i = 0; i < amt; i++) // sets random nodes costs to 3
		{
			int randomX = UtilRandom::instance()->GetRange(0, Content.size());
			int randomY = UtilRandom::instance()->GetRange(0, Content[0].size());
			Content[randomX][randomY]->cost = 3;
			Content[randomX][randomY]->SetSprite();
		}
	}

	Node* startNode;
	Node* targetNode;

	std::vector<Node*> nextCheck = std::vector<Node*>();



	//Node* getNode(int x, int y);

	struct NodeCompare
	{
		bool operator()(const Node* n1, const Node* n2) const
		{
			return n1->Priority > n2->Priority;
		}
	};

	std::priority_queue<Node*, std::vector<Node*>, NodeCompare> priorityQueue; // priority queue used for dijkstras and astar

};


