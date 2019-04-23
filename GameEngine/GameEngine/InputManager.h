#pragma once
#include <iostream>

#include <filereadstream.h>
#include <filewritestream.h>
#include <fstream>
#include <string>

#include <SFML\Graphics.hpp>
#include <document.h>
#include <ostreamwrapper.h>
#include <writer.h>

#include "WindowManager.h"

using namespace rapidjson;

class InputManager
{
public:

	InputManager();
	~InputManager();

	static InputManager* _instance;

	enum InputType
	{
		KeyboardInput,
		MouseInput
	};

	struct Button
	{
		std::string buttonName;
		InputType inputType;
		sf::Keyboard::Key key1;
		sf::Keyboard::Key key2;
		sf::Mouse::Button mouseButton1;
		sf::Mouse::Button mouseButton2;
	};

	std::vector<Button> buttonList;

	std::vector<sf::Keyboard::Key> keysDownThisFrame;
	std::vector<sf::Keyboard::Key> keysDownLastFrame;
	
	std::vector<sf::Event> events;

	bool eventHappened = false;

	Document document;

	void clearEvents();
	void Start();
	void update();

	int ButtonAmt;

	void ReadInputFile();

	void AddNewButton(std::string name, sf::Keyboard::Key key1, sf::Keyboard::Key key2);

	void AddNewButton(std::string name, sf::Keyboard::Key key1);

	void AddNewButton(std::string name, sf::Mouse::Button button1, sf::Mouse::Button button2);

	void AddNewButton(std::string name, sf::Mouse::Button button1);
	
	bool getButton(std::string buttonName);
	bool getButton(sf::Keyboard::Key key);
	bool getButton(sf::Mouse::Button button);

	bool getButtonDown(std::string buttonName);
	bool getButtonDown(sf::Keyboard::Key key);
	bool getButtonDown(sf::Mouse::Button button);

	bool getButtonUp(std::string buttonName);
	bool getButtonUp(sf::Keyboard::Key key);
	bool getButtonUp(sf::Mouse::Button button);

	static InputManager* instance()
	{
		if (!_instance)
			_instance = new InputManager;
		return _instance;
	}
};

