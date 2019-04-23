#pragma once
#include <SFML\Graphics.hpp>

#include <iostream>
#include <fstream>
#include <document.h>
#include <filereadstream.h>

using namespace rapidjson;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	void ReadScene(std::string name);

	std::string SceneName;

	sf::Vector2f sceneGravity;

	struct GameObjectInfo
	{
		std::string type;
		std::string name;
		sf::Vector2f pos;
		float rot;
	};

	std::vector<GameObjectInfo*> GameObjects;
};

