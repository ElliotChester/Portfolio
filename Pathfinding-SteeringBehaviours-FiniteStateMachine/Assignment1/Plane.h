#pragma once
#include "BaseEntity.h"

class PlaneStateMachine;
class Plane :
	public BaseEntity
{
public:
	Plane();
	~Plane();
	Plane(Grid* grid);
	Plane(std::string file);
	Plane(std::string file, sf::Color colour);
	void Think();

public:
	static std::vector<Plane*> Renderables;
	float fuelLevel = 100;
	sf::Text StateText;
	int target[2];
	Node* TargetNode;
	Grid* grid;
	sf::Clock clock;
	PlaneStateMachine* psm;

public:
	sf::Sprite GetSprite() { return sprite; }
	void SetColour(sf::Color colour) { colourTint = colour; }
	void SetTextureFile(std::string file) { filename = file; }
	void SetTarget(int x, int y) { target[0] = x; target[1] = y; };
	void Flight();
	void Refuel();

protected:
	std::string filename;
	sf::Sprite sprite;
	sf::Texture texture;
	sf::Color colourTint;
	sf::Font font;
	
	
	
	/*enum State {
		flying,
		refueling
	};*/


public:
	/**
	* Initialize assures our texture is loaded using the assigned file name and initial colour tint
	*/
	void Initialize();

	sf::Window window;

	sf::Vector2f velocity;

	float movementSpeed = 150;
	float rotationSpeed = 6;

	sf::Vector2i windowPosition;
};



