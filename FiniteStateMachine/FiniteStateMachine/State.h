#pragma once
#include <iostream>
#include <time.h>
#include <string>
#include "DataStore.h"
using namespace std;

class State //This is the bast state class, it houses everything needed to make a state work
{
public:
	virtual void Enter() = 0; //a function that will be only be called at the beginning of the state
	virtual void Execute() = 0; // this function has the main part of the state, if no state is given to switch to, this function keeps running. 
	virtual void Exit() = 0; // this function is only called at the end of the state before switching to another state.

	virtual State* GetNewState() = 0; // this will be called by the state manager and used to get the next state to switch to.

	int GetRandomInt(int low, int high) { // a random int generator used throughout the states, i put it here so that all the states can access it without having to write it in each state. it takes 2 arguments, low and high
		int range = (high - low) + 1;  // low is used to set the minimum number that can be generated and high sets the maximum number.
		int randomInt = range * (rand() / (RAND_MAX + 1.0)); // this gets a random int using rand(), then divides it by the maximum number rand() can create, this makes it a value between 0 and 1
		return randomInt; // this returns the number generated
	}
};

