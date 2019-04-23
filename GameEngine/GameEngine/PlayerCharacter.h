#pragma once
#include "GameObject.h"
#include "GameManager.h"
class PlayerCharacter :
	public GameObject
{
public:
	PlayerCharacter();
	PlayerCharacter(int xPos, int yPos);
	PlayerCharacter(int xPos, int yPos, int Rot);
	PlayerCharacter(int xPos, int yPos, int Rot, std::string filePath);

	~PlayerCharacter();

	void GameObject::Start();
	void GameObject::Update();
};

