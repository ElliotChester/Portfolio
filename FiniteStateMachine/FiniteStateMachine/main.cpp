// FiniteStateMachine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "StateMachine.h"

int main()
{
	std::cout << "Finite state machine demo made by Elliot Chester" << std::endl << std::endl;

	cout << "3D Printer Simulator" << endl << endl; //Creates a title for the program that will only be shown at the beginning

	StateMachine stateMachine; //Creates a new state machine that will control all of the states.

	while (true)//this will call the update function in statemachine forever until the program is closed.
	{
		stateMachine.Update();
	}

	return 0;
}

