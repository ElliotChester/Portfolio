#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <queue>

#include "Node.h"

class Grid
{
public:
	Grid();
	~Grid();

	std::vector<std::vector<Node*>> Content;

	int newCoordinates;

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

	std::priority_queue<Node*, std::vector<Node*>, NodeCompare> priorityQueue;

};

