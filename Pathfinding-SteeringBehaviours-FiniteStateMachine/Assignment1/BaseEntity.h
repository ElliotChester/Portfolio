#pragma once

/**
* BaseEntity, the basic building block of any AI agent in the framework.
*
* This is the parent class for any agents desired to be added to the system,
* it inherits from sf::Transformable to give us access to set / get position and related functionality.
*/


#include <string>
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

// we need to tell the compiler that it's ok to use the default defines from math.h, if we don't, we don't get access to M_PI!
#define _USE_MATH_DEFINES

#include <math.h>
#include "UtilRandom.h"

#include "Grid.h"
#include "Node.h"

class BaseEntity : public sf::Transformable
{
public:
	static std::vector<BaseEntity*> Renderables;
public:
	explicit BaseEntity();
	explicit BaseEntity(std::string file);
	explicit BaseEntity(std::string file, sf::Color colour);

	virtual ~BaseEntity();

	virtual void Think();

	sf::Sprite GetSprite() { return sprite; }

	void SetColour(sf::Color colour) { colourTint = colour; }

	void SetTextureFile(std::string file) { filename = file; }

protected:
	std::string filename;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Color colourTint;

	sf::Vector2f velocity;

protected:
	virtual void Initialize();

	virtual float GetDistance(BaseEntity* target);
	virtual float GetDistance(sf::Vector2i target);

	sf::Clock clock;

	float movementSpeed = 100;
	float rotationSpeed = 500;
};




