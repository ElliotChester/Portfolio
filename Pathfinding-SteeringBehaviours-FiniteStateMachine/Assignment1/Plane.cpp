#include "Plane.h"
#include "PlaneStateMachine.h"

std::vector<Plane*> Plane::Renderables;

Plane::Plane(Grid* tempGrid) //this takes a grid as an argument so that it can get the nodes needed to seek
{
	filename = "assets\\boid.png";
	colourTint = sf::Color::White;

	grid = tempGrid;
	TargetNode = grid->startNode;
	psm = new PlaneStateMachine(this);
	Initialize();
}

Plane::~Plane()
{
}

void Plane::Initialize() // called when the plane is created, this will setup the plane so it is ready to be used
{
	setPosition(0, 0);
	setOrigin(17, 17);
	texture.loadFromFile(filename);
	texture.setSmooth(false);
	sprite.setTexture(texture);
	sprite.setColor(colourTint);

	grid->AssignStartNode(TargetNode->coordinates[0], TargetNode->coordinates[1]);

	font.loadFromFile("Assets/Gravity-Regular.otf");
	StateText.setFont(font);
	StateText.setPosition(700, 30);
	StateText.setString("Flying");
	StateText.setCharacterSize(20);
	StateText.setFillColor(sf::Color::White);

	int NewTargetX = UtilRandom::instance()->GetRange(0, grid->gridSize);
	int NewTargetY = UtilRandom::instance()->GetRange(0, grid->gridSize);
	//std::cout << NewTargetX << " " << NewTargetY << std::endl;
	grid->AssignTargetNode(NewTargetX, NewTargetY);
	grid->SetRandomObsticals(30);
	grid->SetRandomCosts(120);

	SetTarget(TargetNode->xPos, TargetNode->yPos);

	float angle = 135;
	setRotation(angle);
	velocity = sf::Vector2f(cos(angle * M_PI / 180), sin(angle * M_PI / 180));

	Renderables.push_back(this);
}

void Plane::Think()
{

	/*switch (currentState)//state machine
	{
	case flying:
		Flight();
		break;
	case refueling:
		Refuel();
		break;
	}*/

	psm->Update(); //tells the state machine class to update the state machine
	


	clock.restart(); // restarts the clock so that we can get the delta time
}

