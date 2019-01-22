#pragma once
#include "Node.h"
class Inverter :
	public Node
{
private:
	Node * child;

public:
	
	void AddChild(Node* newChild); //this node onl needs 1 child
	virtual bool run() override;
};

