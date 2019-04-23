#include "WindowManager.h"

WindowManager* WindowManager::_instance = 0;
sf::RenderWindow* WindowManager::GameWindow = 0;

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
}

void WindowManager::CreateWindow(int width, int height, std::string name)
{
	GameWindow = new sf::RenderWindow();
	GameWindow->create(sf::VideoMode(width, height), name);
	GameWindow->setPosition(sf::Vector2i((1920 / 2) - (width / 2), (1080 / 2) - (height / 2)));
}

void WindowManager::CreateWindow(int width, int height, std::string name, int frameRateLimit)
{
	GameWindow = new sf::RenderWindow();
	GameWindow->create(sf::VideoMode(width, height), name);
	GameWindow->setPosition(sf::Vector2i((1920 / 2) - (width / 2), (1080 / 2) - (height / 2)));
	GameWindow->setFramerateLimit(frameRateLimit);
}


