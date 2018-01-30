#pragma once

#include "BaseState.h"
#include "FlightState.h"
#include "RefuelState.h"

class Plane;
class PlaneStateMachine
{
public:
	PlaneStateMachine(Plane* newPlane);
	~PlaneStateMachine();

	void Update();

	void ChangeState(BaseState* newState);


	BaseState* currentState;
	Plane* plane;
};

