#include "GroundObject.h"



GroundObject::GroundObject()	//there are different constructors dependent on how much the user wants to specify when creating the objects in the game manager
{
	SetSprite("Assets/Ground.png");
	sprite.setPosition(500, 500);
	startPos = b2Vec2(500, 500);
	startRot = 0;
	physicsEnabled = false;
}

GroundObject::GroundObject(int PosX, int PosY)
{
	SetSprite("Assets/Ground.png");
	sprite.setPosition(PosX, PosY);
	startPos = b2Vec2(PosX, PosY);
	startRot = 0;
	physicsEnabled = false;
}

GroundObject::GroundObject(int PosX, int PosY, int Rot)
{
	SetSprite("Assets/Ground.png");
	sprite.setPosition(PosX, PosY);
	startPos = b2Vec2(PosX, PosY);
	startRot = Rot;
	physicsEnabled = false;
}

GroundObject::GroundObject(int PosX, int PosY, int Rot, std::string filePath)
{
	SetSprite(filePath);
	sprite.setPosition(PosX, PosY);
	startPos = b2Vec2(PosX, PosY);
	startRot = Rot;
	physicsEnabled = false;
}

GroundObject::~GroundObject()
{
}

void GroundObject::Start()	//this object does not use start or update because it has nothing that it needs to update at the start or each frame
{

}

void GroundObject::Update()
{

}
