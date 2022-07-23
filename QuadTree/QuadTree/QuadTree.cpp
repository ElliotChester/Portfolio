// QuadTree.cpp : Defines the entry point for the console application.
//

#include <SFML\Graphics.hpp>
#include "Node.h"

int main()
{
	std::cout << "This is a Quadtree Program created by Elliot Chester\n" <<
		"It uses SFML for the graphics\n" <<
		"This was created as part of the AI module at University\n" <<
		"In the code you can set the target points that the program will search for" <<
		std::endl << std::endl;

	sf::RenderWindow window;												//
	window.create(sf::VideoMode(800, 800), "QuadTrees");				//sets up the window
	window.setPosition(sf::Vector2i(100, 100));						//
	

	//---------------------------------------------------------------------------------------//
	//-------------Add or change points here to change the output of the program-------------//
	//---------------------------------------------------------------------------------------//
	std::vector<sf::Vector2f> targets = std::vector<sf::Vector2f>();
	targets.push_back(sf::Vector2f(10, 90));
	targets.push_back(sf::Vector2f(714, 508));
	targets.push_back(sf::Vector2f(512, 359));									
	//---------------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------------//
	//---------------------------------------------------------------------------------------//


	sf::Texture texture;												
	std::vector<sf::Sprite> targetSprites = std::vector<sf::Sprite>();	
	
	for (sf::Vector2f target : targets)									//creates and places the red dots for the targets
	{
		sf::Sprite targetSprite;

		texture.loadFromFile("Assets/target.png");
		texture.setSmooth(false);
		sf::Texture::bind(&texture);
		targetSprite.setTexture(texture);
		targetSprite.setColor(sf::Color(255, 0, 0));
		targetSprite.setPosition(target);
		targetSprite.setOrigin(texture.getSize().x, texture.getSize().y);
		targetSprite.setScale(5, 5);
		targetSprites.push_back(targetSprite);
	}

	Node RootNode(sf::Vector2f(0,0), 800.0f, targets, 0);	//creates the root node and sets its position and size
	Node::allNodes.push_back(&RootNode);
	RootNode.CheckForTarget();								//Checks if a target is within the node and creates the quad tree

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(sf::Color(0, 0, 0));

		for (Node* node : Node::allNodes)			//draws all of the nodes in the quadtree
		{
			window.draw(node->sprite);
		}
		for (sf::Sprite sprite : targetSprites)		//draws all of the targets in the window
		{
			window.draw(sprite);
		}
		
		window.display();
	}

    return 0;
}

