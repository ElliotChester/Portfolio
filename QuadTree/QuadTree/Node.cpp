#include "Node.h"

std::vector<Node*> Node::allNodes = std::vector<Node*>();

Node::Node(sf::Vector2f position, float newSize, std::vector<sf::Vector2f> targets, int level)
{
	thisLevel = level;

	switch (thisLevel)	//decides which sprite should be used for this node
	{
	case(0):
		texture.loadFromFile("Assets/Square.png");
	break;
	case(1):
		texture.loadFromFile("Assets/Square1.png");
		break;
	case(2):
		texture.loadFromFile("Assets/Square2.png");
		break;
	case(3):
		texture.loadFromFile("Assets/Square3.png");
		break;
	case(4):
		texture.loadFromFile("Assets/Square4.png");
		break;
	case(5):
		texture.loadFromFile("Assets/Square5.png");
		break;
	case(6):
		texture.loadFromFile("Assets/Square6.png");
		break;
	}

	
	texture.setSmooth(false);
	sf::Texture::bind(&texture);
	sprite.setTexture(texture);

	size = newSize;

	setPosition(position);
	sprite.setPosition(position);

	sprite.setScale(size / texture.getSize().x, size / texture.getSize().y);	//sets the size of the node

	targetPositions = targets;

	//std::cout << "Made Node, position is " << getPosition().x << " " << getPosition().y << " and scale is " << size/800 << std::endl;
}

Node::~Node()
{
}

void Node::CheckForTarget()
{
	for (sf::Vector2f targetPosition : targetPositions)
	{
		//std::cout << "Checking For target" << std::endl;
		if (targetPosition.x > getPosition().x && targetPosition.x <= this->getPosition().x + size)		//checks if the target is within the x bounds of the node
		{
			if (targetPosition.y > getPosition().y && targetPosition.y <= this->getPosition().y + size)	//checks if the target is within the y bounds of the node
			{
				//std::cout << "Target found" << std::endl;
				CreateChildren();
			}
		}
	}
}

void Node::CreateChildren()	//This function will check if there is a target within this node, and creates 4 nodes within this one
{
	if (thisLevel >= 6) { return; }

	int nextLevel = thisLevel + 1;

	Children = std::vector<Node*>();	

	Node* child1 = new Node(getPosition(), size/2, targetPositions, nextLevel);											//Creates a new node with half the width and height						
	Children.push_back(child1);

	Node* child2 = new Node(getPosition() + sf::Vector2f(size / 2, 0), size / 2, targetPositions, nextLevel);			//does the same for each node but sets its position so that together they fill the parent node
	Children.push_back(child2);

	Node* child3 = new Node(getPosition() + sf::Vector2f(0, size / 2), size / 2, targetPositions, nextLevel);
	Children.push_back(child3);

	Node* child4 = new Node(getPosition() + sf::Vector2f(size / 2, size / 2), size / 2, targetPositions, nextLevel);
	Children.push_back(child4);

	for (Node* node : Children)			//loops through all of the new nodes and checks if they need children
	{
		node->CheckForTarget();
		Node::allNodes.push_back(node);	//then adds them to the list of all the nodes so that they can be drawn
	}
}
