#include "PlaneStateMachine.h"
#include "Plane.h"

PlaneStateMachine::PlaneStateMachine(Plane* newPlane) // constructor that takes a plane pointer so that it can make changes to that plane
{
	plane = newPlane;
	currentState = new FlightState(plane);
}


PlaneStateMachine::~PlaneStateMachine()
{
}

// this is called every frame, it will tell the current state to execute and attempt to change state
void PlaneStateMachine::Update() 
{
	currentState->Execute();
	ChangeState(currentState->GetNewState());
}

// this will change state if the current state returns a state to change to
void PlaneStateMachine::ChangeState(BaseState* newState)  
{
	if (newState == nullptr) { return; }

	currentState->Exit();

	BaseState* oldState = currentState; 
	currentState = newState; 
	delete oldState; // deletes the previous current state in order to save memory.
}