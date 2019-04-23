#include "SceneManager.h"


SceneManager::SceneManager()
{
	
}


SceneManager::~SceneManager()
{
}

void SceneManager::ReadScene(std::string filePath) //this function is used to load a scene from a json file
{
	
	GameObjects.clear();	//clears the current gameobjects so that they wont be shown anymore

	FILE* pFile = fopen(filePath.c_str(), "rb");	//loads the text frm the file into the program
	char buffer[65536];
	FileReadStream is(pFile, buffer, sizeof(buffer));

	Document document;
	document.ParseStream<0, UTF8<>, FileReadStream>(is);	//loads the file and reads it as json
	if (document.IsObject())								//if the file is a json file
	{
		int i = 0;
		
		while (true)	//will run until break is called
		{
			char intString[32];
			sprintf(intString, "%d", i);

			char objectName[64] = "Object";	

			strcat(objectName, intString);	//adsd the int to the string so that it knows what it has to look for in the json file

			if (document.HasMember(objectName))	//if there is a member with that name and number
			{
				if (document[objectName].IsObject())	//if that object is a valid object
				{
					GameObjectInfo* newGO = new GameObjectInfo;

					if (document[objectName].HasMember("Type") && document[objectName]["Type"].IsString()) 
					{						
						newGO->type = document[objectName]["Type"].GetString();
						//std::cout << "Type:	" << newGO->type << std::endl;
					}

					if(document[objectName].HasMember("Name") && document[objectName]["Name"].IsString())	//gets the name of the object 
					{
						newGO->name = document[objectName]["Name"].GetString();
						//std::cout << "Name:	" << newGO->name << std::endl;
					}

					if (document[objectName].HasMember("Position") && document[objectName]["Position"].IsArray())	//gets the position of the object
					{
						//std::cout << "Pos:	";

						const Value& positionArray = document[objectName]["Position"];

						int x = positionArray[0].GetInt();
						int y = positionArray[1].GetInt();

						newGO->pos = sf::Vector2f(x, y);

						//std::cout << x << ", " << y << std::endl;
					}

					if (document[objectName].HasMember("Rotation") && document[objectName]["Rotation"].IsInt())	//gets teh rotation of the object
					{
						newGO->rot = document[objectName]["Rotation"].GetInt();
						//std::cout << "Rot:	" << newGO->rot << std::endl;
					}

					GameObjects.push_back(newGO);//adds the object to the vector so that it will be displayed and updated if needed
				}
				i++;
				//std::cout << std::endl;				
			}
			else  //if the member is not found it can assume there are no more objoects to find so it breaks
			{
				break;
			}
		}
	}
}
