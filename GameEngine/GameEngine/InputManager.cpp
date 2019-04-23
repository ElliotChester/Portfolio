#include "InputManager.h"

InputManager *InputManager::_instance = 0;

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

void InputManager::Start()	//this is called at the begginning of the game
{
	ReadInputFile();		//This calls a function that will read the input json file
	AddNewButton("Mousetest", sf::Mouse::Left);	
}

void InputManager::update()		//called each frame
{
	clearEvents();

	sf::Event event;
	while (WindowManager::GameWindow->pollEvent(event))	//this will check for input that the player gives
	{
		if (event.type == sf::Event::Closed)
		{
			WindowManager::GameWindow->close();
		}

		eventHappened = true;
		events.push_back(event);
	}
}

void InputManager::ReadInputFile()					//This function will read a json file with input inside of it, it will then create the buttons
{
	buttonList.clear();								//clears the vector of buttons so that it can be repopulated with the new ones

	std::string filePath = "Assets/Input.json";		//sets the file path of the text file that will be read

	FILE* pFile = fopen(filePath.c_str(), "rb");	//gets the text inside the json file
	char buffer[65536];
	FileReadStream is(pFile, buffer, sizeof(buffer));

	document.ParseStream<0, UTF8<>, FileReadStream>(is);	

	if (document.IsObject()) //this is a json function that will check if the document is a json object, this makes sure that the document is a ready to be used for json and not just a normal text file
	{
		ButtonAmt = 0;

		while (true)	//this will keep looping until break is called
		{
			char intString[32];
			sprintf(intString, "%d", ButtonAmt);

			char buttonName[64] = "Button";

			strcat(buttonName, intString);	//this adds an int to the button name so that a specific button can be found

			if (document.HasMember(buttonName))	//the buttonname is a specific string given to differentiate each button in the json file  
			{
				Button newButton = Button();
				if(document[buttonName].IsObject())	//if the button found is a valid button
				{
					if (document[buttonName].HasMember("Name") && document[buttonName]["Name"].IsString())	//these checks will make sure that the value being given is corerct, then adds to the button
					{
						newButton.buttonName = document[buttonName]["Name"].GetString();
					}

					if (document[buttonName].HasMember("Key1") && document[buttonName]["Key1"].IsInt())		//checks if this button is a keyboard button
					{
						newButton.inputType = KeyboardInput;
						newButton.key1 = sf::Keyboard::Key(document[buttonName]["Key1"].GetInt());
						if (document[buttonName].HasMember("Key2") && document[buttonName]["Key2"].IsInt()) 
						{
							newButton.key2 = sf::Keyboard::Key(document[buttonName]["Key2"].GetInt());
						}
						else 
						{
							newButton.key2 = sf::Keyboard::Key(-1);
						}
					}

					if (document[buttonName].HasMember("MouseButton1") && document[buttonName]["MouseButton1"].IsInt())	//checks if this button is a keyboard button
					{
						newButton.inputType = MouseInput;
						newButton.mouseButton1 = sf::Mouse::Button(document[buttonName]["MouseButton1"].GetInt());
						if (document[buttonName].HasMember("MouseButton2") && document[buttonName]["MouseButton2"].IsInt()) 
						{
							newButton.mouseButton2 = sf::Mouse::Button(document[buttonName]["MouseButton2"].GetInt());
						}
						else
						{
							newButton.mouseButton2 = sf::Mouse::Button(-1);
						}
					}
					
					ButtonAmt++;
					buttonList.push_back(newButton);
				}
			}
			else    //if there is no button with the next number it can assume there are no more buttons to read
			{
				break;
			}
		}
	}

}

void InputManager::clearEvents()	//clears the events so that it doesnt look like they are happening when they are not
{
	eventHappened = false;
	events.clear();
}

void InputManager::AddNewButton(std::string name, sf::Keyboard::Key key1, sf::Keyboard::Key key2)	//This is used to add a new button in another script, it will add it to be used and add it to the input json file, this button takes 2 keyboard keys
{
	if (!document.IsObject()) { return; }	//if the file is not a json file we dont want to try and add to it
	
	for (auto button : buttonList)	//makes sure the button doesnt already exist
	{
		if (button.buttonName == name) { 
			std::cout << "Button Already Exists" << std::endl;
			return; 
		}
	}

	std::cout << "Adding New Button" << std::endl;

	Value newButton(kObjectType);		//
	Value newButtonName(kStringType);	//create the values that will need to be added to the json file
	Value newKey1(kNumberType);			//
	Value newKey2(kNumberType);			//

	std::cout << "Created Values" << std::endl;

	newKey1 = (int)key1;
	newKey2 = (int)key2;

	//int len = name.length();

	newButtonName.SetString(name.c_str(), name.length(), document.GetAllocator());

	newButton.AddMember("Name", newButtonName, document.GetAllocator());	//
	newButton.AddMember("Key1", newKey1, document.GetAllocator());			//adds the parameters to the button
	newButton.AddMember("Key2", newKey2, document.GetAllocator());			//

	char intString[32];
	sprintf(intString, "%d", ButtonAmt);
	char buttonName[64] = "Button";
	strcat(buttonName, intString);
	std::string nameString = buttonName;

	Value memberName(kStringType);
	
	memberName.SetString(nameString.c_str(), nameString.length(), document.GetAllocator());

	document.AddMember(memberName, newButton, document.GetAllocator());	//adds the new member to the document

	std::string filePath = "Assets/Input.json";

	FILE* fp = fopen(filePath.c_str(), "wb");						//
	char writeBuffer[65536];										//
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));		//writes this to the text file
	Writer<FileWriteStream> writer(os);								//
	document.Accept(writer);										//
	fclose(fp);														//

	std::cout << "Should Be Done" << std::endl;

	ReadInputFile();												//reads the input file again so that the new button is able to be used

	/*Button newButton = Button();
	newButton.buttonName = name;
	newButton.inputType = KeyboardInput;
	newButton.key1 = key1;
	newButton.key2 = key2;

	std::cout << "New Button " << newButton.buttonName << " " << newButton.key1 << " " << newButton.key2 << std::endl;

	buttonList.push_back(newButton);*/
}

void InputManager::AddNewButton(std::string name, sf::Keyboard::Key key1)	//will make a new button with 1 key
{
	AddNewButton(name, key1, sf::Keyboard::Unknown);
}

void InputManager::AddNewButton(std::string name, sf::Mouse::Button button1, sf::Mouse::Button button2)	//will make a new button with 2 mouse buttons
{
	if (!document.IsObject()) { return; }

	for (auto button : buttonList)
	{
		if (button.buttonName == name) {
			std::cout << "Button Already Exists" << std::endl;
			return;
		}
	}

	std::cout << "Adding New Button" << std::endl;

	Value newButton(kObjectType);
	Value newButtonName(kStringType);
	Value newMousebutton1(kNumberType);
	Value newMousebutton2(kNumberType);

	std::cout << "Created Values" << std::endl;

	newMousebutton1 = (int)button1;
	newMousebutton2 = (int)button2;

	//int len = name.length();

	newButtonName.SetString(name.c_str(), name.length(), document.GetAllocator());

	newButton.AddMember("Name", newButtonName, document.GetAllocator());
	newButton.AddMember("MouseButton1", newMousebutton1, document.GetAllocator());
	newButton.AddMember("MouseButton2", newMousebutton2, document.GetAllocator());

	char intString[32];
	sprintf(intString, "%d", ButtonAmt);
	char buttonName[64] = "Button";
	strcat(buttonName, intString);
	std::string nameString = buttonName;

	Value memberName(kStringType);

	memberName.SetString(nameString.c_str(), nameString.length(), document.GetAllocator());

	document.AddMember(memberName, newButton, document.GetAllocator());

	std::string filePath = "Assets/Input.json";

	FILE* fp = fopen(filePath.c_str(), "wb");
	char writeBuffer[65536];
	FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	Writer<FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(fp);

	std::cout << "Should Be Done" << std::endl;
	ButtonAmt++;
	ReadInputFile();

	/*Button newMouseButton = Button();
	newMouseButton.buttonName = name;
	newMouseButton.inputType = MouseInput;
	newMouseButton.mouseButton1 = button1;
	newMouseButton.mouseButton2 = button2;

	//std::cout << "New Button " << newMouseButton.buttonName << " " << newMouseButton.mouseButton1 << " " << newMouseButton.mouseButton2 << std::endl;

	buttonList.push_back(newMouseButton);*/
}

void InputManager::AddNewButton(std::string name, sf::Mouse::Button button1)	//creates a button with 1 mouse button
{
	AddNewButton(name, button1, sf::Mouse::Button(-1));
}

bool InputManager::getButton(std::string buttonName)	//checks if a button is currently being pressed
{
	for (auto button : buttonList)
	{
		if (button.buttonName == buttonName) {
			if (button.inputType == KeyboardInput) {
				if (sf::Keyboard::isKeyPressed(button.key1) 
					|| sf::Keyboard::isKeyPressed(button.key2))
				{
					return true;
				}
				else 
				{
					return false;
				}
			}

			if (button.inputType == MouseInput) {
				if (sf::Mouse::isButtonPressed(button.mouseButton1) 
					|| sf::Mouse::isButtonPressed(button.mouseButton2))
				{
					return true;
				}
				else 
				{
					return false;
				}
			}

		}
	}
	return false;
}

bool InputManager::getButton(sf::Keyboard::Key key)	//checks is a key is currently being pressed
{
	if (sf::Keyboard::isKeyPressed(key))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::getButton(sf::Mouse::Button button)	//checks if a mouse button is currently being pressed
{
	if (sf::Mouse::isButtonPressed(button))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool InputManager::getButtonDown(std::string buttonName)	//checks if a button has been pressed in the frame
{
	if (!eventHappened) { return false; }
	for (auto event : events) 
	{
		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed) {
			for (auto button : buttonList)
			{
				if (button.buttonName == buttonName)
				{
					if (button.inputType == KeyboardInput && event.type == sf::Event::KeyPressed)
					{
						if (event.key.code == button.key1 || event.key.code == button.key2)
						{
							return true;
						}
						else
						{
							return false;
						}
					}

					if (button.inputType == MouseInput && event.type == sf::Event::MouseButtonPressed)
					{
						if (event.mouseButton.button == button.mouseButton1 || event.mouseButton.button == button.mouseButton2)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
			return false;
		}
		else
		{
			return false;
		}
	}
}

bool InputManager::getButtonDown(sf::Keyboard::Key key)	//checks if a keyboard key has been pressed in this frame
{
	if (!eventHappened) { return false; }
	for (auto event : events)
	{
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == key)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

bool InputManager::getButtonDown(sf::Mouse::Button button)	//checks if a mouse button was pressed in this frame
{
	if (!eventHappened) { return false; }
	for (auto event : events)
	{
		if (event.type == sf::Event::MouseButtonReleased) {
			if (event.mouseButton.button == button)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

bool InputManager::getButtonUp(std::string buttonName)	//checks if a button was released in this frame
{
	if (!eventHappened) { return false; }
	for (auto event : events)
	{
		if (event.type == sf::Event::KeyReleased || event.type == sf::Event::MouseButtonReleased) {
			for (auto button : buttonList)
			{
				if (button.buttonName == buttonName)
				{
					if (button.inputType == KeyboardInput && event.type == sf::Event::KeyReleased)
					{
						if (event.key.code == button.key1 || event.key.code == button.key2)
						{
							return true;
						}
						else
						{
							return false;
						}
					}

					if (button.inputType == MouseInput && event.type == sf::Event::MouseButtonReleased)
					{
						if (event.mouseButton.button == button.mouseButton1 || event.mouseButton.button == button.mouseButton2)
						{
							return true;
						}
						else
						{
							return false;
						}
					}
				}
			}
			return false;
		}
		else
		{
			return false;
		}
	}
}

bool InputManager::getButtonUp(sf::Keyboard::Key key)	//checks if a key was released in this frame
{
	if (!eventHappened) { return false; }
	for (auto event : events)
	{
		if (event.type == sf::Event::KeyReleased) {
			if (event.key.code == key)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

bool InputManager::getButtonUp(sf::Mouse::Button button)	//checks if a mouse button was realeased in this frame
{
	if (!eventHappened) { return false; }
	for (auto event : events)
	{
		if (event.type == sf::Event::MouseButtonReleased) {
			if (event.mouseButton.button == button)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
}

