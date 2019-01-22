#include "stdafx.h"
#include "ReloadState.h"


ReloadState::ReloadState(DataStore* datastore) //Constructor for the class that takes a pointer for a datastore
{
	DS = datastore; //Sets the DS value as the datastore given when the class is created.
}


ReloadState::~ReloadState()
{
}

void ReloadState::Enter()
{
	cout << "Reload:" << endl; // used as a title for the state in the console
}

void ReloadState::Execute()
{
	filamentLeft = DS->filamentAmt; //Sets the fillamentLeft in this class to the same as the fillamentAmt in the datastore Class
	if (filamentLeft != 3000) //If the fillament is not already at its max capacity
	{
		if (filamentLeft > 1000) //if the filament is over 1000 it will call this so that it can be shown in kilograms
		{
			float tempKG = filamentLeft; //Creates a temporary value that will be divided to show kg not g
			tempKG = tempKG / 1000; //divides tempkg by 1000 to turn it from g to kg
			cout << "you had " << tempKG << "kg of filament Left" << endl; //outputs to console, tells the user how much filament they had in KG
		}
		else //If the filament is under 1000 this will be called so that it can be shown in grams
		{
			cout << "you had " << filamentLeft << "g of filament Left" << endl; //outputs to console, tells the user how much filamet they had in 
		}

		float percentComplete = 0; //creates a percentage and sets it to 0

		while (percentComplete < 100) //while the value is not 100% it will increase the value 
		{
			Sleep(100); //sleeps the program to allow the user to see whats happening
			int randomInt = GetRandomInt(10, 15); //creates a new int and sets it to a random number between 10 and 15

			if ((100 - percentComplete) < randomInt) //if the random value is bigger than the difference between the percentage it is at and 100 it will make the percentage 100
			{
				percentComplete = 100; // sets the percentage to 100
			}
			else //If the difference is bigger than the random int it will  add the random int
			{
				percentComplete += randomInt; //adds the random int to the percentage
			}

			cout << "Reloading: " << percentComplete << "%   \r"; //Prints the percentage and sets it to be rewritten
		}

		cout << "Filament Reloaded - There is now 3kg" << endl; //tells the user the printer is full

		filamentLeft = 3000; //sets the filament to 3000
		DS->filamentAmt = filamentLeft; //sets the filamentamt in datastore to the same as filament
	}
	else
	{
		cout << "No need to reload, the printer is already full" << endl; //tells the user the printer is already full
	}
}

void ReloadState::Exit()
{
	cout << endl << endl; //creates 2 empty lines
}

State * ReloadState::GetNewState()
{
	return new IdleState(DS); //returns the idle state so the user can decide what to do next
}
