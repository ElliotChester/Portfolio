#pragma once

#include "IdleState.h" //this is needed because it will be used as the first state
#include "DataStore.h" //this is needed to create the datastore and give it to the first state

class StateMachine //This class controls all of the state changing and running.
{
public:
	StateMachine();
	~StateMachine();

	void Update(); //will be called every frame 

private:

	State* CurrentState; //this will be the state that is currently running 

	DataStore* DS; //this is a pointer that will be assigned to a datastore that will be created at the begginning

	void ChangeState(State* newState); //Change state will be used to switch to another state in the program, it will get the state needed from the current state.
};