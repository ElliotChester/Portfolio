#include "PlayerCharacter.h"


PlayerCharacter::PlayerCharacter()
{
	SetSprite("Assets/Square.png");
	sprite.setPosition(100, 100);
	startPos = b2Vec2(100, 100);
	startRot = 0;
	physicsEnabled = true;
	//createPhysicsBody(100, 100);
}

PlayerCharacter::PlayerCharacter(int xPos, int yPos)
{
	SetSprite("Assets/Square.png");
	sprite.setPosition(xPos, yPos);
	startPos = b2Vec2(xPos, yPos);
	startRot = 0;
	physicsEnabled = true;
	//createPhysicsBody(xPos, yPos);
} 

PlayerCharacter::PlayerCharacter(int xPos, int yPos, int Rot)
{
	SetSprite("Assets/Square.png");
	sprite.setPosition(xPos, yPos);
	startPos = b2Vec2(xPos, yPos);
	startRot = Rot;
	physicsEnabled = true;
	//createPhysicsBody(xPos, yPos);
}

PlayerCharacter::PlayerCharacter(int xPos, int yPos, int Rot, std::string filePath)
{
	SetSprite(filePath);
	sprite.setPosition(xPos, yPos);
	startPos = b2Vec2(xPos, yPos);
	startRot = Rot;
	physicsEnabled = true;
	//createPhysicsBody(xPos, yPos);
}

PlayerCharacter::~PlayerCharacter()
{
}

void PlayerCharacter::Start()
{

}

int LevelSelectint = 0;

void PlayerCharacter::Update()
{
	if (input->getButton("Left"))							//These inputs move the player
	{
		velocity = PhysicsBody->GetLinearVelocity();
		velocity.x = -15;
		sprite.setScale(sf::Vector2f(-1, 1));
	}
	else
	if (input->getButton("Right"))
	{
		velocity = PhysicsBody->GetLinearVelocity();
		velocity.x = 15;
		
		sprite.setScale(sf::Vector2f(1, 1));
	}
	else
	{
		velocity = PhysicsBody->GetLinearVelocity();
		velocity.x = 0;
	}

	if (input->getButtonDown("Jump"))
	{
		velocity.y = -15;
	}

	if (input->getButtonDown("Reset"))	//used to reset the scene with the next scene
	{
		GameManager* GM = GameManager::instance();
		if (LevelSelectint < 2 ) {
			LevelSelectint++;
		}
		else 
		{
			LevelSelectint = 0;
		}

		switch (LevelSelectint)
		{
		case 0:
			GM->LoadScene(GM->Level1Path);
			break;

		case 1:
			GM->LoadScene(GM->Level2Path);
			break;

		case 2:
			GM->LoadScene(GM->Level3Path);
			break;
		}
	}

	MoveTowards(sf::Vector2f(6,5), sf::Vector2f(0, 0), 1);

	if (input->getButtonDown("Mousetest2"))
	{
		std::cout << "Working" << std::endl;
	}
	

	ApplyVelocity();
}


