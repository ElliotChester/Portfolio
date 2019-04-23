#pragma once
#include <iostream>
#include <math.h>

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include "InputManager.h"
#include "TimeManager.h"

class GameObject: 
	public sf::Transformable
{
public:
	GameObject();
	~GameObject();

	virtual void Start() = 0;
	virtual void Update() = 0;

	b2Vec2 startPos;
	int startRot;
	

	bool physicsEnabled = false;

	InputManager* input = InputManager::instance();
	sf::Texture texture;
	sf::Sprite sprite;

	b2Vec2 velocity;

	b2Body* PhysicsBody;

	double DegToRad = (double)(b2_pi / 180);

	const float scale = 30.f;

	void SetSprite(std::string filePath) //used to set the sprite of this game object
	{
		texture.loadFromFile(filePath);
		texture.setSmooth(false);
		sf::Texture::bind(&texture);
		sprite.setTexture(texture);
		sprite.setOrigin(sf::Vector2f(texture.getSize().x / 2, texture.getSize().y / 2));
	}

	void Destroy() //used to destroy this physics body so that it isnt updated anmore
	{
		PhysicsBody->GetWorld()->DestroyBody(PhysicsBody);
	}

	void ApplyVelocity() //applys velocity to the game object
	{
		PhysicsBody->SetLinearVelocity(velocity);
	}

	void ApplyGravity()		//applys gravity with box2d
	{
		float xVel = velocity.x;
		float yVel = velocity.y;

		float targetYVel = 30;

		float newYVel = yVel + ((targetYVel - yVel) / 0.5) * TimeManager::deltaTime;

		velocity = b2Vec2(xVel , newYVel);
	}

	sf::Vector2f MoveTowards(sf::Vector2f origin, sf::Vector2f target, float speed) //move towards function that will smoothly move between 2 vectors at a given speed
	{
		sf::Vector2f difference = target - origin;
		float length = sqrt((difference.x * difference.x) + (difference.y * difference.y));
		sf::Vector2f normalized = difference / length;
		//std::cout << normalized.x << " " << normalized.y << std::endl;

		return normalized * speed;

	}

	void screenWrapX() //wraps the object onto the screen on the x axis
	{
		if (sprite.getPosition().x > WindowManager::GameWindow->getSize().x) {
			sprite.setPosition(sprite.getPosition().x - WindowManager::GameWindow->getSize().x, sprite.getPosition().y);
		}

		if (sprite.getPosition().x < 0) 
		{
			sprite.setPosition(sprite.getPosition().x + WindowManager::GameWindow->getSize().x, sprite.getPosition().y);
		}
	}

	void screenWrapY()  //wraps the object onto the screen on the y axis
	{
		if (sprite.getPosition().y > WindowManager::GameWindow->getSize().y) {
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - WindowManager::GameWindow->getSize().y);
		}

		if (sprite.getPosition().y < 0)
		{
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + WindowManager::GameWindow->getSize().y);
		}
	}


	void screenWrap()  //wraps the object onto the screen on the x and y axis
	{
		if (sprite.getPosition().x > WindowManager::GameWindow->getSize().x) {
			sprite.setPosition(sprite.getPosition().x - WindowManager::GameWindow->getSize().x, sprite.getPosition().y);
		}

		if (sprite.getPosition().x < 0)
		{
			sprite.setPosition(sprite.getPosition().x + WindowManager::GameWindow->getSize().x, sprite.getPosition().y);
		}

		if (sprite.getPosition().y > WindowManager::GameWindow->getSize().y) {
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y - WindowManager::GameWindow->getSize().y);
		}

		if (sprite.getPosition().y < 0)
		{
			sprite.setPosition(sprite.getPosition().x, sprite.getPosition().y + WindowManager::GameWindow->getSize().y);
		}
	}

	void createPhysicsBody(b2World& World, int PosX, int PosY) //creates a physics body to be used with this object
	{
		b2BodyDef BodyDef;
		BodyDef.position = b2Vec2(startPos.x / scale, startPos.y / scale);
		BodyDef.angle = startRot * DegToRad;
		BodyDef.fixedRotation = true;
		if (physicsEnabled) {
			BodyDef.type = b2_dynamicBody;
		}
		else 
		{
			BodyDef.type = b2_staticBody;
		}
		b2Body* Body = World.CreateBody(&BodyDef);
		PhysicsBody = Body;
		b2PolygonShape Shape;
		Shape.SetAsBox((texture.getSize().x / 2) / scale, (texture.getSize().y / 2) / scale);
		b2FixtureDef FixtureDef;
		FixtureDef.density = 1.f;
		FixtureDef.friction = 0.0f;
		FixtureDef.shape = &Shape;
		Body->CreateFixture(&FixtureDef);
	}

	void UpdatePhysics() //updates the physics for the physics body
	{
		sprite.setPosition(sf::Vector2f(PhysicsBody->GetPosition().x * scale, PhysicsBody->GetPosition().y * scale) );
		sprite.setRotation(180 / b2_pi * PhysicsBody->GetAngle());
		velocity = PhysicsBody->GetLinearVelocity();
	}
};

