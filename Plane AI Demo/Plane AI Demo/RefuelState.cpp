#include "RefuelState.h"

RefuelState::RefuelState(Plane* newPlane) // constructor that takes a pointer to a plane so that it can make changes
{
	plane = newPlane;
}


RefuelState::~RefuelState()
{

}

void RefuelState::Enter() // this is called just after the state is created
{
	std::cout << "Refueling" << std::endl;
	
}

void RefuelState::Execute() // this is called every frame
{
	sf::Time elapsed = plane->clock.getElapsedTime();

	plane->fuelLevel += 20 * elapsed.asSeconds();
	plane->StateText.setString("Refueling");
}

void RefuelState::Exit() // called before the next state is created
{
	std::cout << std::endl << std::endl; //Creates 2 empty lines
}


BaseState * RefuelState::GetNewState() // this will only return a state if the fuel is over 100, and will return nullptr so that the state will not change
{
	if (plane->fuelLevel > 100) {
		plane->fuelLevel = 100;
		return new FlightState(plane); //Tells the state manager to go back to the IdleState, and gives idle state the pointer to the DataStore
	}
	return nullptr;

}
