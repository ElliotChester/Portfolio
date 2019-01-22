#include "stdafx.h"
#include "State.h"
#include <iostream>


void State::Enter() 
{

}

void State::Execute() 
{
	std::cout << "State!" << std::endl;
}

void State::Exit() 
{

}

State* State::GetNewState() 
{
	return nullptr;
}
