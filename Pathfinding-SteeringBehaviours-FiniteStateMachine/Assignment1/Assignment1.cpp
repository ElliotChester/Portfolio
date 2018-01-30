#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <random>

#include "BaseEntity.h"
#include "Plane.h"
#include "UtilRandom.h"

#include "Grid.h"
#include "Node.h"

int windowWidth = 800;
int windowHeight = 875;

bool finding = false;
bool drawLines = true;

enum pathfindingMethod 
{ 
	BreadthFirst,
	Dijkstras,
	Astar
};

pathfindingMethod currentMethod = Dijkstras;

int main()
{
	// construct a window that uses a resolution of 800 x 600
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Steering Behaviours");

	// because we're running as a console application, we still get access to the console, so can use std::cout to print to it
	//std::cout << "Constructed SFML Window" << std::endl;	

	Grid* grid = new Grid(16);

	Plane *plane = new Plane(grid);

	//Creates 2 text objects that will be used to show the anound of fuel and instructions for the program
	sf::Text helpText;
	sf::Font font;
	font.loadFromFile("Assets/Gravity-Regular.otf");
	helpText.setFont(font);
	helpText.setCharacterSize(20);
	helpText.setFillColor(sf::Color::White);

	sf::Text fuelText;
	fuelText.setFont(font);
	fuelText.setPosition(700, 0);
	fuelText.setString(std::to_string(plane->fuelLevel));
	fuelText.setCharacterSize(20);
	fuelText.setFillColor(sf::Color::White);

	// everything in this while loop will be called in every frame of our program
	while (window.isOpen())
	{
		// this will be used for user input
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			// handles user keyboard input
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{

				case sf::Keyboard::Num1:
					currentMethod = BreadthFirst;
					helpText.setString(" Breadth First will activate next time the plane lands");
					break;

				case sf::Keyboard::Num2:
					currentMethod = Dijkstras;
					helpText.setString(" Dijkstras will activate next time the plane lands");
					break;

				case sf::Keyboard::Num3:
					currentMethod = Astar;
					helpText.setString(" A* will activate next time the plane lands");
					break;
				}
			}
		}

		// clear the window before drawing anything to it, this will ensure that the last frame in the buffer is not shown aswell as the new one
		window.clear();

		// while there is no path set, it will run through this to search through the grid and find the path to the target
		while (!grid->PathSet)
		{
			//This switch will be used to decide which pathfinding method to use
			switch (currentMethod) {

			case BreadthFirst:
				grid->BreadthFirstSearch(); 
				helpText.setString(" Currently Using Breadth First \n Press 2 to use Dijkstras \n Press 3 to use A*");
				break;

			case Dijkstras:
				grid->DijkstrasSearch();
				helpText.setString(" Currently Using Dijkstras \n Press 1 to use Breadth First \n Press 3 to use A*");
				break;

			case Astar:
				grid->AStarSearch();
				helpText.setString(" Currently Using A* \n Press 1 to use Breadth First \n Press 2 to use Dijkstras");
				break;
			}
		}

		grid->Display(); // this function will set the positions of the nodes in the grid, and also change their sprites to represent their weight and obsticals

		// 2 forloops used to loop through all of the nodes and draw them to the window
		for (size_t i = 0; i < grid->gridSize; i++)
		{
			for (size_t j = 0; j < grid->gridSize; j++)
			{
				window.draw(grid->Content[i][j]->sprite, grid->Content[i][j]->getTransform()); // this works by getting the sprite that needs to be drawn, then gets the transform so that it can be put at the right position, with the right rotation and scale
			}
		}

		//Calling the think function of the plane allows it to be controlled continuously
		plane->Think();

		// Draws the plane to the window
		window.draw(plane->GetSprite(), plane->getTransform());
		

		// sets the fuel string to the value of the fuel with some text to give the number context
		fuelText.setString("Fuel: " + std::to_string((int)plane->fuelLevel) + "%");

		// these draw all of the text in the window
		window.draw(fuelText);
		window.draw(helpText);
		window.draw(plane->StateText);

		//swaps the buffers in the window
		window.display();
	}

	return 0;
}


