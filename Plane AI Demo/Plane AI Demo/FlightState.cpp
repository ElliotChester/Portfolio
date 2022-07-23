#include "FlightState.h"



FlightState::FlightState(Plane* newPlane) // constructor that take a pointer to a plane so that it can be referenced throughout the state
{
	plane = newPlane;
}


FlightState::~FlightState()
{
}

void FlightState::Enter()	//this is called when the state is made
{
	plane->StateText.setString("Flying");
}

void FlightState::Execute() //this is called every frame
{
	sf::Time elapsed = plane->clock.getElapsedTime();			//This gets the deltatime
	//sf::Vector2i mousePos = sf::Mouse().getPosition();

	plane->fuelLevel -= 5 * elapsed.asSeconds();				// reduces the amount of fuel in the plane

	

#pragma region Movement
	float oldRot = plane->getRotation();						

	int targetX = plane->target[0] - plane->getPosition().x;
	int targetY = plane->target[1] - plane->getPosition().y;

	float targetRot = atan2(targetY, targetX) * (180 / M_PI);	// this gets the andle that will be needed to look at the target position

	while (targetRot > 360)										//this will take the number recieved above and turn it into a number that can be used as an angle
	{
		targetRot -= 360;
	}
	while (targetRot < 0)
	{
		targetRot += 360;
	}

	if (targetRot - oldRot < -180)								
	{			
		targetRot += 360;
	}
	else if (targetRot - oldRot > 180) {
		targetRot -= 360;
	}

	float newRot = oldRot + ((targetRot - oldRot) * plane->rotationSpeed * elapsed.asSeconds()); // this will smoothly move the angle towards the target

	while (newRot > 360) {
		newRot -= 360;
	}
	while (newRot < 0) {
		newRot += 360;
	}

	plane->velocity = sf::Vector2f(cos(plane->getRotation() * M_PI / 180), sin(plane->getRotation() * M_PI / 180)) * elapsed.asSeconds();	// this sets the velocity to forwards relative to the direction the plane is facing
	sf::Vector2f pos = plane->getPosition() + (plane->velocity * plane->movementSpeed);														

	plane->setPosition(pos);		//set the position of the plane relative to the window
	plane->setRotation(newRot);		//set the new rotation of the plane
#pragma endregion

	atTarget = false;
	if (plane->getPosition().x < plane->target[0] + 20
		&& plane->getPosition().x > plane->target[0] - 20
		&& plane->getPosition().y < plane->target[1] + 20
		&& plane->getPosition().y > plane->target[1] - 20)	// this checks if the plane is within the node it is seeking
	{
		if (plane->TargetNode != nullptr) {
			if (plane->TargetNode->Child != nullptr) // if there is a child node it sets that child node as the target node
			{
				//std::cout << "still has child" << std::endl;
				Node* nextNode = plane->TargetNode->Child;
				plane->TargetNode = nextNode;
				plane->SetTarget(plane->TargetNode->xPos, plane->TargetNode->yPos);
			}
			else 
			{
				if (plane->TargetNode->target == true) // if this node is the target, set it as the start and get a new target, and randomise the clouds
				{
					atTarget = true;
					plane->grid->AssignStartNode(plane->TargetNode->coordinates[0], plane->TargetNode->coordinates[1]);

					int NewTargetX = UtilRandom::instance()->GetRange(0, plane->grid->gridSize);
					int NewTargetY = UtilRandom::instance()->GetRange(0, plane->grid->gridSize);
					//std::cout << NewTargetX << " " << NewTargetY << std::endl;
					if (plane->fuelLevel < 30) {
						//currentState = refueling;
						plane->setPosition(plane->target[0], plane->target[1]);
					}

					plane->grid->AssignTargetNode(NewTargetX, NewTargetY);
					plane->grid->SetRandomObsticals(40);
					plane->grid->SetRandomCosts(80);
				}
			}
		}
	}
}

void FlightState::Exit() //this is called before the next state is created
{
	plane->grid->AssignStartNode(plane->TargetNode->coordinates[0], plane->TargetNode->coordinates[1]);

	int NewTargetX = UtilRandom::instance()->GetRange(0, plane->grid->gridSize);
	int NewTargetY = UtilRandom::instance()->GetRange(0, plane->grid->gridSize);
	if (plane->fuelLevel < 30) {
		plane->setPosition(plane->target[0], plane->target[1]);
	}

	plane->grid->AssignTargetNode(NewTargetX, NewTargetY);
	plane->grid->SetRandomObsticals(40);
	plane->grid->SetRandomCosts(80);
}

BaseState * FlightState::GetNewState() // this is called every frame to decided if a differnt state should be switched to
{
	if (plane->fuelLevel < 30 && atTarget) {
		return new RefuelState(plane); //Tells the state manager to go back to the IdleState, and gives idle state the pointer to the DataStore
	}

	return nullptr;
}
