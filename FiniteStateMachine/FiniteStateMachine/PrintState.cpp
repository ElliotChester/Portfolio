#include "stdafx.h"
#include "PrintState.h"



PrintState::PrintState(DataStore* datastore)
{
	DS = datastore;
}


PrintState::~PrintState()
{
}

void PrintState::Enter()
{
	cout << "Print:" << endl;
}

void PrintState::Execute()
{
	if ( //Checks if the printer is calibrated enough to print
		DS->xAxisCalibration > -10
		&& DS->xAxisCalibration < 10
		&& DS->yAxisCalibration > -10
		&& DS->yAxisCalibration < 10
		&& DS->zAxisCalibration > -10
		&& DS->zAxisCalibration < 10
		) {

		if (DS->filamentAmt >= 500) //Checks if there is enough filament in the printer to print
		{
			percentage = 0; //resets the percentage to 0 
			while (percentage < 100) //Checks if percentage is less than 100 so that it can add some percentage. 
			{
				Sleep(100); //Sleeps the program for 100ms so that the user can see what happens.
				int randomInt = GetRandomInt(2, 7); //creates an int called random int and assigns it a random number between 2 and 7

				if ((100 - percentage) < randomInt) { //If the random int is more than the difference between the percentage and 100 it will just set the percentage to 100 so it doesnt say it is over 100% complete
					percentage = 100; //Sets the percentage to 100
				}
				else //If the difference is not more than the random int it will simply add the random int to the percentage
				{
					percentage += randomInt; //adds the random int to the percentage
				}

				cout << "Printing: " << percentage << "%   \r"; //prints to console, printing: and the percentage complete the print is. used /r to tell allow it to be written over when it is updated
			}
			DS->filamentAmt -= 500; //Takes 500 from the filamentleft in the DataStore

			DS->xAxisCalibration += (GetRandomInt(0, 10) - 5); //changes the xAxisCalibration value in the DataStore class
			DS->yAxisCalibration += (GetRandomInt(0, 10) - 5); //changes the yAxisCalibration value in the DataStore class
			DS->zAxisCalibration += (GetRandomInt(0, 10) - 5); //changes the zAxisCalibration value in the DataStore class

			cout << "Print Complete!          " << endl; //Prints to console, print complete
		}
		else //If there is not enough filament it will not print
		{ 
			cout << "There is not enough filament left to complete this print" << endl; //tell the user there is not enough filament
		}
	}
	else //If the axes are not calibrated enough the printer will not print. 
	{
		cout << "The printer cannot print as the head is mis-aligned, try re-calibrating" << endl; //Tells the user it cannot print
	}
}

void PrintState::Exit()
{
	cout << endl << endl; //Creates 2 empty lines
}

State * PrintState::GetNewState()
{
	return new IdleState(DS); //Tells the state manager to go back to the IdleState, and gives idle state the pointer to the DataStore
}
