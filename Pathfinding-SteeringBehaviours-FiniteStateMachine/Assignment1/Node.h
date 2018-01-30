#pragma once


#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

//#include "Grid.h"

class Node :
	public sf::Transformable
{
public:
	Node();

	~Node();

	bool obstruction = false;
	bool checked = false;
	bool path = false;
	bool target = false;
	bool start = false;

	int cost = 1;

	int CostSoFar = 0;
	double Priority = 0;

	float xPos;
	float yPos;

	sf::Sprite GetSprite() { return sprite; }

	std::vector<float> coordinates;

	Node* Parent = nullptr;
	Node* Child = nullptr;

	sf::Sprite sprite;
	sf::Texture cloudTexture;
	sf::Texture darkCloudTexture;
	sf::Texture defaultTexture;
	sf::Texture airportTexture;

	void initTextures() {
		defaultTexture.loadFromFile("assets\\Sky.png");
		defaultTexture.setSmooth(false);
		cloudTexture.loadFromFile("assets\\Cloud.png");
		cloudTexture.setSmooth(false);
		darkCloudTexture.loadFromFile("assets\\DarkCloud.png");
		darkCloudTexture.setSmooth(false);
		airportTexture.loadFromFile("assets\\Airport.png");
		airportTexture.setSmooth(false);
	}

	void SetSprite()
	{
		sprite.setTexture(defaultTexture);

		if (cost > 1) {
			sprite.setTexture(cloudTexture);
		}
		if (obstruction) {
			sprite.setTexture(darkCloudTexture);
		}
		if (start || target) {
			sprite.setTexture(airportTexture);
		}
	}

	void setSpriteColor() {
		if (checked) {
			sprite.setColor(sf::Color(220,220,220));
		}
		else {
			sprite.setColor(sf::Color::White);
		}
	}
};


