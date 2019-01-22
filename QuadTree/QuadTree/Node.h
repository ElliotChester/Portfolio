#pragma once
#include <iostream>
#include <vector>

#include <SFML\Graphics.hpp>

class Node: sf::Transformable
{
public:

	Node(sf::Vector2f position, float newSize, std::vector<sf::Vector2f> targets, int level);
	~Node();

	sf::Texture texture;
	sf::Sprite sprite;

	int thisLevel;

	float size;

	static std::vector<Node*> allNodes;

	std::vector<sf::Vector2f> targetPositions;

	std::vector<Node*> Children;

	void CheckForTarget();

	void CreateChildren();
};

