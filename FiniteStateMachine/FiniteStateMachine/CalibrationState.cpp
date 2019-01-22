#include "stdafx.h"
#include "CalibrationState.h"


CalibrationState::CalibrationState(DataStore* datastore)
{
	DS = datastore; // sets the pointer to a datastore to the datastore given by the previous current state.
}


CalibrationState::~CalibrationState()
{
}

void CalibrationState::Enter()
{
	cout << "Calibration:" << endl; //used to title the state currently running.
}

void CalibrationState::Execute()
{
	if (DS->xAxisCalibration == 0 && DS->yAxisCalibration == 0 && DS->zAxisCalibration == 0) //Checks if the axes actually need calibrating
	{ 
		cout << "Printer Already Calibrated" << endl; //if not it will not run the calibration and will tell the user it does not need calibrating
	}
	else
	{	
		int xValue = DS->xAxisCalibration; //Gets the xaxiscalibration int from datastore and sets it to xValue. 

		while (xValue != 0) //While the x axis is not calibrated, Calibrate it
		{
			Sleep(100); //Sleeps the program for 100ms so that the user can actually see whats happening
			if (xValue < 0) //If the x is below 0, the program needs to increase x to get it to 0
			{
				int adjustAmount = GetRandomInt(3, 5); //creates an int and assigns it a random number between 3 and 5 that will be added to the xValue
				xValue += adjustAmount; //adds the adjusted amount to the xValue
			}
			else // if the x is below 0, the program needs to increase x to get it to 0
			{
				int adjustAmount = GetRandomInt(3, 5); //creates an int and assigns it a random number between 3 and 5 that will be taken from the xValue
				xValue -= adjustAmount; //takes the adjusted amount from the xValue
			}
			cout << "Calibrating X Axis: " << xValue << "   \r"; //tells the user calibration is happening and shows how close to 0 the axis is.
		}
		DS->xAxisCalibration = xValue; //Sets the xAxisCalibration int in DataStore to the new value
		cout << "X Axis Calibrated       " << endl; //Tells the user this axis ia calibrated

		int yValue = DS->yAxisCalibration; //The Method is the same for both the y and z axes
		while (yValue != 0)
		{
			Sleep(100);
			if (yValue < 0)
			{
				int adjustAmount = GetRandomInt(3, 5);
				yValue += adjustAmount;
			}
			else
			{
				int adjustAmount = GetRandomInt(3, 5);
				yValue -= adjustAmount;
			}
			cout << "Calibrating X Axis: " << yValue << "   \r";
		}
		DS->yAxisCalibration = yValue;
		cout << "Y Axis Calibrated       " << endl;

		int zValue = DS->zAxisCalibration;
		while (zValue != 0)
		{
			Sleep(100);
			if (zValue < 0)
			{
				int adjustAmount = GetRandomInt(3, 5);
				zValue += adjustAmount;
			}
			else
			{
				int adjustAmount = GetRandomInt(3, 5);
				zValue -= adjustAmount;
			}
			cout << "Calibrating X Axis: " << zValue << "   \r";
		}
		DS->zAxisCalibration = zValue;
		cout << "Z Axis Calibrated       " << endl;
	}
}

void CalibrationState::Exit()
{
	cout << endl << endl; //Creates 2 empty lines 
}

State* CalibrationState::GetNewState()
{
	return new IdleState(DS); //returns the idle state so that the user can be asked for input again.
}
