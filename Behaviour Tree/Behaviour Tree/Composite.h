#pragma once
#include "Node.h"
class Composite :
	public Node
{
private:
	std::vector<Node*> children;

public:

	//This class is a template for other classes that need to have multiple children
	const std::vector<Node*>& getChildren() const { return children; }
	void addChild(Node* child) { children.push_back(child); }
};

