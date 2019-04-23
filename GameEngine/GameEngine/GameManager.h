#pragma once
#include <iostream>

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include "InputManager.h"
#include "SceneManager.h"
#include "GameObject.h"

#include <document.h>

class GameManager
{
public:
	GameManager();
	~GameManager();

	InputManager* IM;

	SceneManager* currentScene;

	SceneManager* SM;

	sf::Sprite sprite;

	static double gravity;

	sf::Texture GroundTexture;
	sf::Texture BoxTexture;

	void Start();
	void Update();

	void LoadScene(std::string SceneToLoadPath);

	std::vector<GameObject*> gameObjects = std::vector<GameObject*>();

	const float scale = 30.f;

	std::string Level1Path = "Assets/Level1.json";
	std::string Level2Path = "Assets/Level2.json";
	std::string Level3Path = "Assets/Level3.json";


	static GameManager* _instance;

	static GameManager* instance()
	{
		if (!_instance)
			_instance = new GameManager;
		return _instance;
	}
};

