// GridDrawer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>

#include "GridCreator.h"

#include "SDL.h"	// SDL header file
#include <glew.h>	// GLEW header file which provide all the opengGL functionality

using namespace std;

// Define window size and position

int width = 800;
int height = 800;
int xpos = (1920 / 2) - (width / 2);
int ypos = (1080 / 2) - (height / 2);

vector<float> targetPoint;

int gridWidth = 10;
int gridHeight = 10;

SDL_Window* window = NULL;	// Create a pointer for the window

SDL_GLContext context;	// Create a context for opengGL

bool drawLines = true;	// Bool used to determine if the triangle should be drawn

							////////////////////////////////////////////////////////////////////////////
							//
							// Function to display opengGL primitives, this is called from withinmain()
							//
							////////////////////////////////////////////////////////////////////////////

void display(vector<vector<vector<float>>>* gridToDraw)	//This is used to draw the grid in the window
{

	float xOffset = -0.05 * gridToDraw->size() + 0.05;			//these offsets will be used to make sure the grid is in the correct position
	float yOffset = -0.05 * gridToDraw[0][0].size() + 0.05;

	glClear(GL_COLOR_BUFFER_BIT);								// Clear colour buffer

	if (drawLines)
	{
		for (size_t i = 0; i < gridToDraw->size(); i++)			//loops through all of the columns in the gird
		{
			vector<vector<float>> column = (*gridToDraw)[i];
			for (float n = 0; n < column.size(); n++)			// loops through all of the nodes in the column
			{
				vector<float> node = column[n];

				glBegin(GL_QUADS);								//draws a quad for each node in the column
				glColor3f(0.3, 0.3, 0.3);
																								//uses the x and y value of the node to give its position in the window, aswell as the offset made earlier, otherwise the top left of the grid would be in the centre of the window
				glVertex2f(node[0] / 10 - 0.045f + xOffset, -node[1] / 10 + 0.045f - yOffset);	//Top Left
				glVertex2f(node[0] / 10 + 0.045f + xOffset, -node[1] / 10 + 0.045f - yOffset);	//Top Right

				glVertex2f(node[0] / 10 + 0.045f + xOffset, -node[1] / 10 - 0.045f - yOffset);	//Bottom Right 
				glVertex2f(node[0] / 10 - 0.045f + xOffset, -node[1] / 10 - 0.045f - yOffset);	//Bottom Left

				glEnd();

				glBegin(GL_LINES);																//draws the outline of each node in the grid
				glColor3f(1, 1, 1);

				glVertex2f(node[0] / 10 - 0.05f + xOffset, -node[1] / 10 - 0.05f - yOffset);	//
				glVertex2f(node[0] / 10 + 0.05f + xOffset, -node[1] / 10 - 0.05f - yOffset);	//
																								// Draws the bottom and right border of each node
				glVertex2f(node[0] / 10 + 0.05f + xOffset, -node[1] / 10 - 0.05f - yOffset);	//
				glVertex2f(node[0] / 10 + 0.05f + xOffset, -node[1] / 10 + 0.05f - yOffset);	//

				if (node[0] == 0)																//If the node is the furthest left in its row, draw the left border
				{
					glVertex2f(node[0] / 10 - 0.05f + xOffset, -node[1] / 10 - 0.05f - yOffset);
					glVertex2f(node[0] / 10 - 0.05f + xOffset, -node[1] / 10 + 0.05f - yOffset);
				}

				if (node[1] == 0)																//If the node is the top in its column, draw the top border
				{
					glVertex2f(node[0] / 10 - 0.05f + xOffset, -node[1] / 10 + 0.05f - yOffset);
					glVertex2f(node[0] / 10 + 0.05f + xOffset, -node[1] / 10 + 0.05f - yOffset);
				}
				glEnd();

				bool selected = false;															
				
				if (node[0] == targetPoint[0] && node[1] == targetPoint[1])						//if the node is the target node given by the user, set it to selected
				{
					selected = true;
				}

				if (selected)																	//if the node is selected it will draw a red square inside the node
				{
					glBegin(GL_LINES);
					glColor3f(1, 0, 0);

					glVertex2f(node[0] / 10 - 0.04f + xOffset, -node[1] / 10 - 0.04f - yOffset);
					glVertex2f(node[0] / 10 + 0.04f + xOffset, -node[1] / 10 - 0.04f - yOffset);

					glVertex2f(node[0] / 10 + 0.04f + xOffset, -node[1] / 10 - 0.04f - yOffset);
					glVertex2f(node[0] / 10 + 0.04f + xOffset, -node[1] / 10 + 0.04f - yOffset);

					glVertex2f(node[0] / 10 - 0.04f + xOffset, -node[1] / 10 - 0.04f - yOffset);
					glVertex2f(node[0] / 10 - 0.04f + xOffset, -node[1] / 10 + 0.04f - yOffset);

					glVertex2f(node[0] / 10 - 0.04f + xOffset, -node[1] / 10 + 0.04f - yOffset);
					glVertex2f(node[0] / 10 + 0.04f + xOffset, -node[1] / 10 + 0.04f - yOffset);

					glEnd();
				}
			}
		}
	}
}

int main(int argc, char* args[])
{
	int inputInt;

	cout << "Grid Creator" << endl << endl;
	cout << "This is a grid creator made by Elliot Chester as a part of a pathfinding system\n" << 
		"I used GLEW and SDL to create the application\n\n" <<
		"Please note a grid above 20 x 20 will not fit in the window correctly\n" <<
		"You can also select a target point that will demonstrate the coordinates working correctly" << 
		endl << endl;

	cout << "Input Grid Width" << endl;
	cin >> inputInt;
	gridWidth = inputInt;

	cout << "Input Grid Height" << endl;
	cin >> inputInt;
	gridHeight = inputInt;

	cout << "Input Target x" << endl;
	cin >> inputInt;
	targetPoint.push_back(inputInt - 1);

	cout << "Input Target y" << endl;
	cin >> inputInt;
	targetPoint.push_back(inputInt - 1);

	bool exit = false;  // Variable to control when to exit application
	SDL_Event event;	// Create an event handler. This is used to react to any user events within the SDL window

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)	// Initialize ALL features of SDL and get return value to check if this worked
	{
		cout << "Error Intialising SDL: " << SDL_GetError() << "\n";	// Output message if there was an error
	}
	else	// If there was no error, procedd to initialising SDL and OpneGL
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);	// Restrict maximum version of opengGL to v4
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);	// Restrict minimum version of opengGL to v4

		window = SDL_CreateWindow("Grid", xpos, ypos, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);	// Create an SDL window

		if (window == NULL)	// Output a message if the window was not created properly
		{
			cout << "Error Creating Window: " << SDL_GetError() << "\n";
		}
		else
		{
			context = SDL_GL_CreateContext(window);		// Create the context, this tells SDL which graphics library it is working with

			if (context == NULL)	// Output a message if there was an error with the context
			{
				cout << "Error Creating Context: " << SDL_GetError() << "\n";
			}
			else
			{
				GLenum error = GL_NO_ERROR;		// Create an error handler

				glMatrixMode(GL_PROJECTION);	// Make the Projection matrix active. The projection matrix defines the properties of the camera that views the objects in 3D space (zoom, aspect ratio etc)
				glLoadIdentity();				// Load the identity matrix. Because the projection matrix is currently loaded, this resets the projection matrix settings to defaults (ie clears it)

				error = glGetError();			// Check if any errors were generated by previous steps
				if (error != GL_NO_ERROR)
				{
					cout << "Error initializing opengGL";
				}

				glMatrixMode(GL_MODELVIEW);		// Make the Model View matrix active. The modelview matrix defines how your objects are transformed 
				glLoadIdentity();				// Load the identity matrix.  Because the model view matrix is currently loaded, this resets the model view matrix settings to defaults (ie clears it)

				error = glGetError();			// Check if any errors were generated by previous steps
				if (error != GL_NO_ERROR)
				{
					cout << "Error initializing opengGL";
				}
			}
		}
	}

	SDL_StartTextInput();	// Enable text input through SDL	

	GridCreator* gridCreator = new GridCreator(gridWidth, gridHeight);

	vector<vector<vector<float>>>* pGrid = gridCreator->getGrid();

	while (!exit)	// Continue until the 'exit' variable has been changed to true
	{
		while (SDL_PollEvent(&event) != 0)	// Check if there are any events to process
		{

			switch (event.type)
			{
				case SDL_QUIT:
					exit = true;
					break;

				case SDL_TEXTINPUT:
					if (event.text.text[0] == 't')
					{
						drawLines = !drawLines;
					}
					break;
			}
		}

		display(pGrid);
	// Execute the display() function on each loop of the main game loop

		SDL_GL_SwapWindow(window);	// Refresh the window to show any changes made in the previous display() function
	}

	SDL_StopTextInput();	// Stop text input

	SDL_DestroyWindow(window);  // Destroy the window
	window = NULL;				// Free up the memory used by the window

	SDL_Quit();				// Shutdown SDL

	return 0;
}

