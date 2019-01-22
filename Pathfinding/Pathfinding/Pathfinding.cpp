// Pathfinding.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include "stdafx.h"

#include "SDL.h"		// SDL header file
#include <GL/glew.h>	// GLEW header file which provide all the opengGL functionality

#include "Grid.h"
#include "Node.h"
#include "InputManager.h"
#include "GridCreator.h"

// Define window size and position

int width = 800;
int height = 800;
int xpos = (1920 / 2) - (width / 2);
int ypos = (1080 / 2) - (height / 2);

std::vector<float> targetPoint;

int pathfindingInt = 0;

int gridWidth = 20;
int gridHeight = 20;

bool finding = false;

SDL_Window* window = NULL;	// Create a pointer for the window

SDL_GLContext context;	// Create a context for opengGL

bool drawLines = true;	// Bool used to determine if the triangle should be drawn


void display(Grid* gridToDraw) //This Will draw the grid on the screen including all the different colours of different nodes
{
	float xOffset = -0.05 * gridToDraw->Content.size() + 0.05;		//These offsets will make sure that the grid starts from the top left of the screen
	float yOffset = -0.05 * gridToDraw->Content[0].size() + 0.05;	//

	glClear(GL_COLOR_BUFFER_BIT);	

	if (drawLines)	//allows the user to turn the drawing on or off
	{
		for (size_t i = 0; i < gridToDraw->Content.size(); i++)	//loops through all of the columns
		{
			std::vector<Node*> column = gridToDraw->Content[i];
			for (float n = 0; n < column.size(); n++)			//loops through all of the nodes in the column
			{
				Node* node = column[n];
				glBegin(GL_QUADS);								//This section will draw all of the squares needed in the grid

				
#pragma region Colours
												//This section will control all of the colours of the different types of node, e.g. target node is green, path is blue
				glColor3f(1.0, 1.0, 1.0);						

				if (node->start == true) 
				{
					glColor3f(0.8, 0.0, 0.0);
				} 
				
				if (node->obstruction == true) 
				{
					glColor3f(0.4, 0.4, 0.4);
					if (node->checked) {
						glColor3f(0.1, 0.4, 0.4);
					}
				}
				else
				if (node->cost > 1) 
				{
					glColor3f(0.4, 0.5, 0.4);
					if (node->checked) 
					{
						glColor3f(0.1, 0.6, 0.4);
					}
				}				
				else
				if (node->checked == true) 
				{
						glColor3f(0.4, 1.0, 1.0);
				}

				if (node->path == true) 
				{
					glColor3f(1.0, 0.4, 0.4);
				}

				if (node->start == true) 
				{
					glColor3f(0.8, 0.0, 0.0);
				}

				if (node->target == true) 
				{
					glColor3f(0.4, 1.0, 0.4);
				}

#pragma endregion
																															//these will draw each point of the quad
				glVertex2f(node->coordinates[0] / 10 - 0.048f + xOffset, -node->coordinates[1] / 10 + 0.048f - yOffset);	//Top Left
				node->xPos = node->coordinates[0] / 10 - 0.05f + xOffset;								  
				node->yPos = -node->coordinates[1] / 10 + 0.05f - yOffset;								  
																										  
				glVertex2f(node->coordinates[0] / 10 + 0.048f + xOffset, -node->coordinates[1] / 10 + 0.048f - yOffset);	//Top Right
				glVertex2f(node->coordinates[0] / 10 + 0.048f + xOffset, -node->coordinates[1] / 10 - 0.048f - yOffset);	//Bottom Right 
				glVertex2f(node->coordinates[0] / 10 - 0.048f + xOffset, -node->coordinates[1] / 10 - 0.048f - yOffset);	//Bottom Left
				glEnd();

				glBegin(GL_LINES);	//This section is used to draw the lines between the nodes
				glColor3f(0, 0, 0);

				glVertex2f(node->coordinates[0] / 10 - 0.05f + xOffset, -node->coordinates[1] / 10 - 0.05f - yOffset);		//
				glVertex2f(node->coordinates[0] / 10 + 0.05f + xOffset, -node->coordinates[1] / 10 - 0.05f - yOffset);		//These will draw the right and bottom lines of the 
				glVertex2f(node->coordinates[0] / 10 + 0.05f + xOffset, -node->coordinates[1] / 10 - 0.05f - yOffset);		//
				glVertex2f(node->coordinates[0] / 10 + 0.05f + xOffset, -node->coordinates[1] / 10 + 0.05f - yOffset);		//

				if (node->coordinates[0] == 0)																				//if the node is the left most in the row
				{
					glVertex2f(node->coordinates[0] / 10 - 0.05f + xOffset, -node->coordinates[1] / 10 - 0.05f - yOffset);
					glVertex2f(node->coordinates[0] / 10 - 0.05f + xOffset, -node->coordinates[1] / 10 + 0.05f - yOffset);
				}

				if (node->coordinates[1] == 0)																				//if the node is the top most in the column
				{
					glVertex2f(node->coordinates[0] / 10 - 0.05f + xOffset, -node->coordinates[1] / 10 + 0.05f - yOffset);
					glVertex2f(node->coordinates[0] / 10 + 0.05f + xOffset, -node->coordinates[1] / 10 + 0.05f - yOffset);
				}
				glEnd();
			}
		}
	}
}

void CheckMousePosition(float x, float y, std::vector<std::vector<Node*>> grid, int mouseButton)	//this will be called when the user clicks, it will call a function in all nodes to see if it has been clicked
{

	for (size_t i = 0; i < grid.size(); i++)
	{
		std::vector<Node*> column = grid[i];
		for (size_t j = 0; j < column.size(); j++)
		{
			column[j]->checkClick( x/(width/2) - 1, -(y/(height/2) - 1), mouseButton);
		}
	}
}


int main(int argc, char* args[])
{

	bool exit = false;  // Variable to control when to exit application
	SDL_Event event;	// Create an event handler. This is used to react to any user events within the SDL window

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)	// Initialize ALL features of SDL and get return value to check if this worked
	{
		std::cout << "Error Intialising SDL: " << SDL_GetError() << "\n";	// Output message if there was an error
	}
	else	// If there was no error, procedd to initialising SDL and OpneGL
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);	// Restrict maximum version of opengGL to v4
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);	// Restrict minimum version of opengGL to v4

		window = SDL_CreateWindow("Grid", xpos, ypos, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);	// Create an SDL window

		if (window == NULL)	// Output a message if the window was not created properly
		{
			std::cout << "Error Creating Window: " << SDL_GetError() << "\n";
		}
		else
		{
			context = SDL_GL_CreateContext(window);		// Create the context, this tells SDL which graphics library it is working with

			if (context == NULL)	// Output a message if there was an error with the context
			{
				std::cout << "Error Creating Context: " << SDL_GetError() << "\n";
			}
			else
			{
				GLenum error = GL_NO_ERROR;		// Create an error handler

				glMatrixMode(GL_PROJECTION);	// Make the Projection matrix active. The projection matrix defines the properties of the camera that views the objects in 3D space (zoom, aspect ratio etc)
				glLoadIdentity();				// Load the identity matrix. Because the projection matrix is currently loaded, this resets the projection matrix settings to defaults (ie clears it)

				error = glGetError();			// Check if any errors were generated by previous steps
				if (error != GL_NO_ERROR)
				{
					std::cout << "Error initializing opengGL";
				}

				glMatrixMode(GL_MODELVIEW);		// Make the Model View matrix active. The modelview matrix defines how your objects are transformed 
				glLoadIdentity();				// Load the identity matrix.  Because the model view matrix is currently loaded, this resets the model view matrix settings to defaults (ie clears it)

				error = glGetError();			// Check if any errors were generated by previous steps
				if (error != GL_NO_ERROR)
				{
					std::cout << "Error initializing opengGL";
				}
			}
		}
	}

	SDL_StartTextInput();	// Enable text input through SDL

	GridCreator* gridCreator = new GridCreator(gridWidth, gridHeight);

	Grid* grid = gridCreator->grid;

	std::cout << "This is a Pathfinding program created by Elliot Chester\n" << 
		"It uses GLEW and SDL for the graphics and window control\n"<<
		"The pathfinding system includes Breadth first search, Dijkstras, and AStar Search(not perfect)\n"
		<< std::endl << std::endl;
	std::cout << "Left Click To place a wall" << std::endl;
	std::cout << "Right Click To place the target" << std::endl;
	std::cout << "Middle Click To place a high weight node" << std::endl << std::endl;

	std::cout << "Press Space to start searching" << std::endl << std::endl;

	std::cout << "Press 1 to switch Breadth First Search" << std::endl;
	std::cout << "Press 2 to switch to Dijkstras Search" << std::endl;
	std::cout << "Press 3 to switch to A Star Search" << std::endl << std::endl;

	std::cout << "Press R to Reset" << std::endl << std::endl;
	
 	while (!exit)	// Continue until the 'exit' variable has been changed to true
	{
		while (SDL_PollEvent(&event) != 0)	// Check if there are any events to process
		{

			switch (event.type)
			{
			case SDL_QUIT:
				exit = true;
				break;



			case SDL_MOUSEBUTTONDOWN:
				if (finding == false) {
					int mouseButton = 0;
					switch (event.button.button)
					{

					case SDL_BUTTON_LEFT:
						mouseButton = 0;
						CheckMousePosition(event.motion.x, event.motion.y, grid->Content, mouseButton);
						break;

					case SDL_BUTTON_RIGHT:
						grid->ResetTarget();
						mouseButton = 1;
						CheckMousePosition(event.motion.x, event.motion.y, grid->Content, mouseButton);
						grid->SetTarget();
						break;

					case SDL_BUTTON_MIDDLE:
						mouseButton = 2;
						CheckMousePosition(event.motion.x, event.motion.y, grid->Content, mouseButton);
						break;
					}
				}
				break;

			case SDL_KEYDOWN:	//in here, the key press functions are added

				switch (event.key.keysym.sym) 
				{

					case 't':	
						drawLines = !drawLines;
					break;

					case ' ':	//if space is pressed we want to start or continue finding the target node 
						if (grid->targetNode != nullptr)
						{
							if (!grid->PathSet) {
								finding = !finding;
							}
							else {
								grid->ResetNodes();
								finding = true;
							}
						}
					break;

					case 'r':
						finding = false;
						grid->ResetNodes();
					break;

					case '1':
						if (finding == false)
						{
							pathfindingInt = 0;
						}
					break;
					
					case '2':
						if (finding == false)
						{
							pathfindingInt = 1;
						}					
					break;
					
					case '3':
						if (finding == false)
						{
							pathfindingInt = 2;
						}					
					break;
				}
			}
		}

		if (finding && !grid->PathSet && pathfindingInt == 0) //chooses which pathfinding method to use
		{
			grid->BreadthFirstSearch();
		}
		if (finding && !grid->PathSet && pathfindingInt == 1)
		{
			grid->DijkstrasSearch();
		}
		if (finding && !grid->PathSet && pathfindingInt == 2)
		{
			grid->AStarSearch();
		}

		display(grid);
		// Execute the display() function on each loop of the main game loop

		SDL_GL_SwapWindow(window);	// Refresh the window to show any changes made in the previous display() function
	}

	SDL_StopTextInput();	// Stop text input

	SDL_DestroyWindow(window);  // Destroy the window
	window = NULL;				// Free up the memory used by the window

	SDL_Quit();				// Shutdown SDL

	return 0;
}

