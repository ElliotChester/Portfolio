#include "stdafx.h"
#include "IdleState.h"

IdleState::IdleState(DataStore* datastore)
{
	DS = datastore; //Sets the DS pointer to the Datastore given by the last State
}


IdleState::~IdleState()
{
}

void IdleState::Enter()
{
	cout << "Idle:" << endl; //used as a title for the state in the console
}

void IdleState::Execute()
{
	cout << "Please select a function" << endl; //Asks the user to pick what they would like to do with the program
	cout << "1 = Calibrate" << endl;
	cout << "2 = Print" << endl;
	cout << "3 = Reload Filament" << endl;

	cin >> inputChar; //Takes in input from the user

	if (inputChar == '2') //if the user inputs 2, the program asks the gives the user another choice
	{
		cout << endl;
		cout << "Do you need to preheat the Print bed?" << endl;
		cout << "1 = Yes" << endl;
		cout << "2 = No" << endl;

		cin >> inputChar; //waits for users input and changes the inputchar to it

		if (inputChar == '1') // if the user chose 1 it will set the input char to 4 so that it selects the correct state to go to. 
		{
			inputChar = '4';
		}
	}

}

void IdleState::Exit()
{
	cout << endl << endl; //Creates 2 empty lines
}

State* IdleState::GetNewState()
{
	if (inputChar == '1') //Takes what the user input and opens a state depending on what they entered
	{ 
		return new CalibrationState(DS);
	}
	else if (inputChar == '2') 
	{
		return new PrintState(DS);
	}
	else if (inputChar == '3') 
	{
		return new ReloadState(DS);
	}
	else if (inputChar == '4') 
	{
		return new PreheatState(DS);
	}
	else
		return nullptr; //if what the user entered wasnt 1, 2, 3 or 4, it will return null and the execute function will be called again.


	
}
