#include "TimeManager.h"

float TimeManager::deltaTime = 0;
float TimeManager::timeScale = 0;

TimeManager::TimeManager()
{
	timeScale = 1.0f;
}

TimeManager::~TimeManager()
{
}

void TimeManager::Update() //called every frame to get the delta time
{
	sf::Time elapsedTime = clock.getElapsedTime();

	TimeManager::deltaTime = elapsedTime.asSeconds() * TimeManager::timeScale;	//the delta time is multipleid by the time scale to give an effect of slow motion

	clock.restart();
}
