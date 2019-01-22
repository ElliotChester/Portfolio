#include "stdafx.h"
#include "StateMachine.h"

StateMachine::StateMachine()
{
	DS = new DataStore; // creates a new datastore and assigns it to the pointer already created
	CurrentState = new IdleState(DS); //assigns a new idlestate as the current state and gives it the pointer to the dataStore.
}

StateMachine::~StateMachine()
{
}

void StateMachine::Update() 
{
	CurrentState->Execute(); // this calls the execute function in the active state every frame.

	ChangeState(CurrentState->GetNewState()); // this will call change state and get the state to change too from the current state.
}

void StateMachine::ChangeState(State* newState)
{
	if (newState == nullptr) {
		return; // if the current state has not given a state to change to it will leave change state to that execute can be ran again.
	}

	CurrentState->Exit(); // calls Exit in the current state before the state is destroyed.

	State* oldState = CurrentState; //creates a temporary pointer to the current state.
	CurrentState = newState; // sets the current state to the state given by the previous current state.
	delete oldState; // deletes the previous current state in order to save memory.

	CurrentState->Enter(); //calls the enter funtion in the new current state, 
}
