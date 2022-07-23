#include "BaseEntity.h"


std::vector<BaseEntity*> BaseEntity::Renderables;

BaseEntity::BaseEntity() // this is called when no arguments are passed through so it will set some default values
{
	filename = "assets\\boid.png";
	colourTint = sf::Color::White;
	Initialize();
}

BaseEntity::BaseEntity(std::string file) : filename(file) // this is called when only a texture is given so it sets the colour to white so the texture can be seen in its own colour
{
	colourTint = sf::Color::White;
	Initialize();
}

BaseEntity::BaseEntity(std::string file, sf::Color colour) : filename(file), colourTint(colour) // this will be called when a file name and colour are given to all it has to do is initialize because there is nothing left to set
{
	Initialize();
}

BaseEntity::~BaseEntity()
{

}

void BaseEntity::Think() // this function will be called every frame, it will control the entity
{
	sf::Time elapsed = clock.getElapsedTime();

	velocity = sf::Vector2f(cos(getRotation() * M_PI / 180), sin(getRotation() * M_PI / 180)) * elapsed.asSeconds();
	sf::Vector2f pos = getPosition() + (velocity * movementSpeed);


	setPosition(pos);
	//setRotation(newRot);
	clock.restart();

	// this checks if the entity is off of the window and loops it to the other side
#pragma region ScreenWrap

	if (getPosition().x < 0) {
		setPosition(800 + getPosition().x, getPosition().y);
	}
	if (getPosition().x > 800) {
		setPosition(0 + (800 - getPosition().x), getPosition().y);
	}

	if (getPosition().y < 0) {
		setPosition(getPosition().x, 800 + getPosition().y);
	}
	if (getPosition().y > 800) {
		setPosition(getPosition().x, 0 + (800 - getPosition().y));
	}

#pragma endregion

}

// this is called when the entity is made, it will set up the important variables of the entity
void BaseEntity::Initialize()
{
	// set a default position - could be randomised.
	setPosition(500.0f, 400.0f);


	// load the texture into memory
	texture.loadFromFile(filename);
	texture.setSmooth(true);
	// and provide the sprite with a pointer to the texture object
	// if the texture object is destroyed (goes out of scope etc) then the sprite will display weirdly
	sprite.setTexture(texture);
	// set up our colour tint

	sprite.setColor(sf::Color::White);

	// select a random angle
	float angle = UtilRandom::instance()->GetRange(0, 360);
	// set our rotation value
	setRotation(angle);
	// and assign a velocity, we need to convert angle to radians so it plays nicely with cos and sin.
	velocity = sf::Vector2f(cos(angle * M_PI / 180), sin(angle * M_PI / 180));

	// add the entity to the list of renderables.
	Renderables.push_back(this);
}

// this is used to get the distance between this entity and the target entity
float BaseEntity::GetDistance(BaseEntity* target) {

	float distancex = target->getPosition().x - getPosition().x;
	float distancey = target->getPosition().y - getPosition().y;

	distancex *= distancex;
	distancey *= distancey;

	if (distancex < 0) {
		distancex *= -1;
	}

	if (distancey < 0) {
		distancey *= -1;
	}

	float distance = sqrt(distancex - distancey);
	return distance;

}

// this will get the distance between this node and a target position
float BaseEntity::GetDistance(sf::Vector2i target) {

	float distancex = target.x - getPosition().x;
	float distancey = target.y - getPosition().y;

	distancex *= distancex;
	distancey *= distancey;

	if (distancex < 0) {
		distancex *= -1;
	}

	if (distancey < 0) {
		distancey *= -1;
	}

	float distance = sqrt(distancex - distancey);
	return distance;

}
