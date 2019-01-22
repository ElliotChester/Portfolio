#pragma once
#include <iostream>
#include <windows.h>

#include "State.h"
#include "IdleState.h"
#include "CalibrationState.h"
#include "PrintState.h"
#include "ReloadState.h"

class PreheatState :
	public State
{
public:
	PreheatState( DataStore* datastore); //called when the idle state is created and takes a datastore pointer so that it can use data from the data store and pass the datastore to the next state.
	~PreheatState();

	void Enter(); //Called when the class starts, you would use this instead of the contructor to ensure that the state machine class is ready. 
	void Execute(); //Called every frame, this is where the main part of the state are kept 
	void Exit(); //Called right before the current script in the state machine class is changed and this state is destroyed

	State* GetNewState(); //This will be used to get the next state to change too.

	DataStore* DS; //The datastore is used to store data that needs to be remembered and used between classes
};

