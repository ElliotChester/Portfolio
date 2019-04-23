#include "GameManager.h"
#include "PlayerCharacter.h"
#include "GroundObject.h"


GameManager *GameManager::_instance = 0;
double GameManager::gravity = 0;


b2Vec2 Gravity(0.f, 30.0f);	//This is the gravity that will be used in the game
b2World world(Gravity);		//This sets up the wrld for the game, every object in the world will have the same properties applied to it.

GameManager::GameManager()
{
	gravity = 9.8;
}

GameManager::~GameManager()
{

}

void GameManager::Start() //this is called when the game starts
{

	SM = new SceneManager();
	LoadScene(Level2Path);
}

void GameManager::Update()	//called every frame
{
	for (GameObject* GO : gameObjects)	//goes througha dn updates all of the game objects in the scene
	{
		GO->Update();					//calls the update function that the the user will be able to edit
		GO->UpdatePhysics();			//calls the update physics function, this is premade and shouldnt be edited
		WindowManager::GameWindow->draw(GO->sprite);	//draws the sprite so that it can be seen in the window
	}

	world.Step(1 / 60.f, 8,8);			//updates the world and all of the physics bodies in it
}

void GameManager::LoadScene(std::string SceneToLoadPath)	//when this is called all of the gameobjects in the game will be destroyed and the new ones read from the scene will be created
{
	std::cout << "Switching Scene" << std::endl;

	for (GameObject* GO : gameObjects)	//destroys each gameobject in the scene
	{
		GO->Destroy();
	}
	gameObjects.clear();				//clears the vector so the destroyed gameobjects wont try and update anymore

	SM->ReadScene(SceneToLoadPath);
	for (SceneManager::GameObjectInfo* GO : SM->GameObjects)	//creates the different objects needed to create the scene
	{
		if (GO->type == std::string("Ground"))
		{
			gameObjects.push_back(new GroundObject(GO->pos.x, GO->pos.y, GO->rot));
		}

		if (GO->type == std::string("Player"))
		{
			gameObjects.push_back(new PlayerCharacter(GO->pos.x, GO->pos.y, GO->rot, "Assets/Tank.png"));
		}
	}

	for (GameObject* GO : gameObjects)	//calls the start function in all of the new gameobjects
	{
		GO->createPhysicsBody(world, 100, 100);
		GO->Start();
	}

}
