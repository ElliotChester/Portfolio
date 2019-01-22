#include "stdafx.h"
#include "PreheatState.h"


PreheatState::PreheatState(DataStore* datastore)
{
	DS = datastore;
}


PreheatState::~PreheatState()
{
}

void PreheatState::Enter()
{
	cout << "Preheat:" << endl; //Used as a title for the state in the console
}

void PreheatState::Execute()
{
	float temp = GetRandomInt(5,20); //Creates a float called temp and gives it a random value between 2 and 20
	float targetTemp = 60; //creates a float called target temp and gives it a value of 60
	while (temp < targetTemp) //while the temp value is less than the targettemp value the program needs to increase the temp value
	{
		Sleep(100); //Sleeps the program for 100ms so that the user can see whats happening

		int randomInt = GetRandomInt(1, 5); //Creates an int called random int and gives it a random value between 1 and 5

		if ((targetTemp - temp) < randomInt) //if the difference between the temprature and target temprature is less than the random int, it will set the temprature to the target temprature 
		{ 
			temp = targetTemp;
		}
		else //if the difference is not more than the randomint then it will add the randomint to the temprature 
		{
			temp += randomInt;
		}
		cout << "Preheating: "<< temp << "C   \r"; //prints to console, Preheating: and the temp value followed by C to show it as a temprature
	}
}

void PreheatState::Exit()
{
	cout << "Preheated             " << endl; //Tells the user the printer is preheated 
	cout << endl << endl; //Creates 2 empty lines
}

State * PreheatState::GetNewState()
{
	return new PrintState(DS); //returns the print state and gives it the pointer to the dataStore.
}
