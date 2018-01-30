#pragma once
#include "BaseState.h"
#include "RefuelState.h"
#include "Plane.h"

class FlightState :
	public BaseState
{
public:
	FlightState(Plane* newPlane);
	~FlightState();

	void Enter(); //Called when the class starts, you would use this instead of the contructor to ensure that the state machine class is ready. 
	void Execute(); //Called every frame, this is where the main part of the state are kept 
	void Exit(); //Called right before the current script in the state machine class is changed and this state is destroyed

	BaseState* GetNewState(); //This will be used to get the next state to change too.

	Plane* plane;

	bool atTarget = false;
};

