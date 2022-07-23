#pragma once

#include <iostream>

class BaseState
{
public:
	virtual void Enter() = 0; //a function that will be only be called at the beginning of the state
	virtual void Execute() = 0; // this function has the main part of the state, if no state is given to switch to, this function keeps running. 
	virtual void Exit() = 0; // this function is only called at the end of the state before switching to another state.

	virtual BaseState* GetNewState() = 0; // this will be called by the state manager and used to get the next state to switch to.
};

