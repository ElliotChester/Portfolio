// GameEngine.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <SFML/Graphics.hpp>

#include "InputManager.h"
#include "GameManager.h"
#include "WindowManager.h"
#include "TimeManager.h"


int main()
{
	WindowManager* WiM = WindowManager::instance();	//
	WiM->CreateWindow(1280, 720, "GameEngine", 60);	//creates a window and 

	TimeManager* TM = new TimeManager();			//
	InputManager* IM = InputManager::instance();    //
	IM->Start();								    //Creates all of the scripts that will be needed for the game to run
												    //
	GameManager* GM = GameManager::instance();	    //
	GM->IM = IM;								    //
	GM->Start();

	WindowManager::GameWindow->setKeyRepeatEnabled(false);	//ensures that keys dont repeat when not wanted

	while (WindowManager::GameWindow->isOpen())	//while the window is open we want to update everything in the game and on the screen
	{
		WindowManager::GameWindow->clear(sf::Color(100,100,255));
		TM->Update();

		IM->update();		

		GM->Update();

		WindowManager::GameWindow->display();
	}

	return 0;
}

