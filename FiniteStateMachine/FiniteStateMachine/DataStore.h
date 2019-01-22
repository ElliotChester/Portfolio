#pragma once
class DataStore // stores all of the data that will need to be used across classes.
{
public:

	int filamentAmt = 1500; //will be used by print and reload.

	float xAxisCalibration = 0; //will be used by print and calibrate.
	float yAxisCalibration = 0; //will be used by print and calibrate.
	float zAxisCalibration = 0; //will be used by print and calibrate.
};

