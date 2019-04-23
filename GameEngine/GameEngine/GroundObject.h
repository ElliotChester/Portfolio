#pragma once
#include "GameObject.h"
class GroundObject :
	public GameObject
{
public:
	GroundObject();
	GroundObject(int PosX, int PosY);
	GroundObject(int PosX, int PosY, int Rot);
	GroundObject(int PosX, int PosY, int Rot, std::string filePath);
	~GroundObject();

	void GameObject::Start();
	void GameObject::Update();
};

