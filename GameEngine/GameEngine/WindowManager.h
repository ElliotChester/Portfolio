#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>

class WindowManager
{
public:
	WindowManager();
	~WindowManager();
	
	void CreateWindow(int width, int height, std::string name);

	void CreateWindow(int width, int height, std::string name, int frameRateLimit);

	static WindowManager* _instance;

	static sf::RenderWindow* GameWindow;

	static WindowManager* instance()
	{
		if (!_instance)
			_instance = new WindowManager;
		return _instance;
	}
};

