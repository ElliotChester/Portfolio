#pragma once
#include <SFML\Graphics.hpp>

class TimeManager
{
public:
	TimeManager();
	~TimeManager();

	static float timeScale;
	static float deltaTime;

	sf::Clock clock = sf::Clock();

	void Update();
};

