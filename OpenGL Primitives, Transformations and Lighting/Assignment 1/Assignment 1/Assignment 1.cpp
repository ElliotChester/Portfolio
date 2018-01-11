#include "stdafx.h"

#include <glut.h>					// glut is used to do all of the window creation and rendering
#include <IL/ilut.h>				// ilut is only needed to load in the texture for the cube

#pragma region Variables

GLfloat currentRotation = 0;		// Current y rotation of the cube
GLfloat rotationSpeed = 0.05f;		// Speed of the rotation of the cube
GLint screenWidth = 800;			// Width of the window in pixels
GLint screenHeight = 800;			// Height of the window in pixels
GLint windowID;						// The ID number of the window
int menu_id;						// The ID of the main menu when you right click
int submenu_id;						// The ID of the submenu within the main menu  
int menuValue = 1;					// The value given by the sub menu when a button is pressed. It is set to default to 1 so that it automatically renders textures 
GLuint texture;						// The texture that will be used for the top and bottom of the cube
GLuint texture2;					// This is a different texture that will be used for the sides of the cube
float deltatime;					// This will hold the amount of time since the last and current frame
float oldElapsedTime = 0;			// This is the amount of time that has passed in the program from the start to the last frame, it is used to calculate the deltatime

#pragma endregion

void drawCube()												// Function used to create a cube and set the correct render style for it.
{															// menuvalue is set by the user when they press buttons in the menu within the program
	if (menuValue == 1)										// Mode 1 enables only textures and disables everything that is not needed
	{
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);							// This enables textures for the cube so that you can see an image instead of just a solid colour
	}
	else if(menuValue == 2)									// Mode 2 requires only lighting to be enabled so it enables lighting and nothing else
	{
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);								// This enables lighting for the cube so that it has a consistent colour and looks different depending on which way it is rotated
	}
	else if (menuValue == 3)								// Mode 3 is only colour material so it enables that and disables everything else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);						// This enables materials so that each side of the cube as a different colour that is not effected by lighting
	}
	else if (menuValue == 4)								// Mode 4 has lighting and textures working together so it enables them and disables color material
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
	}
	else if (menuValue == 5)								// Mode 5 has everything turned on so it enables all the different modes included and doesnt disable anything
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}
	
	glBindTexture(GL_TEXTURE_2D, texture);					// This sets the texture that we want to be used on the top and bottom of the cube
	glBegin(GL_QUADS);										// I'm using quads so that each side of the cube can be made with only 4 vertices instead of having to use 2 triangles per side

	GLfloat DiffuseMaterial[] = { 0.6, 0.6, 0.6 };
	GLfloat AmbientMaterial[] = { 0.2, 0.2, 0.2 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseMaterial);
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientMaterial);
	glColor3f(1.0f, 1.0f, 0.8f);

	// Top face 
	if (menuValue == 3 || menuValue == 5)
		glColor3f(0.0f, 0.8f, 0.8f);						// Sets the colour for this side, this will only be seen if colour material render mode is selected

	glTexCoord2f(1.0, 0.0);									// This sets a point for the texture to map to
	glNormal3f(0, 1, 0);									// This sets the normal for the face so that you it will reflect and diffuse properly, if this is taken away the whole cube is shaded the same colour like it is 1 flat face
	glVertex3f(1.0f, 1.0f, -1.0f);							// This sets the point for the corner of the side

	glTexCoord2f(1.0, 1.0);			
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0f, 1.0f, 1.0f);							// Last point of the first side 

	// Bottom face
	if (menuValue == 3 || menuValue == 5)
		glColor3f(0.8f, 0.0f, 0.0f);						// Each side has a different colour so that they are easy to differentiate

	glTexCoord2f(1.0, 0.0);
	glNormal3f(0, -1, 0);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, texture2);					// This sets the texture that we want to be used on the sides of the cube
	glBegin(GL_QUADS);

	// Front face
	if (menuValue == 3 || menuValue == 5)
		glColor3f(0.8f, 0.8f, 0.0f);

	glTexCoord2f(1.0, 1.0);
	glNormal3f(0, 0, 1);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0f, -1.0f, 1.0f);

	// Back face
	if (menuValue == 3 || menuValue == 5)
		glColor3f(0.0f, 0.8f, 0.0f);

	glTexCoord2f(0.0, 0.0);
	glNormal3f(0, 0, -1);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0f, 1.0f, -1.0f);

	// Left face
	if (menuValue == 3 || menuValue == 5)
		glColor3f(0.0f, 0.0f, 0.8f);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);


	// Right face
	if (menuValue == 3 || menuValue == 5)
		glColor3f(0.8f, 0.0f, 0.8f);
	
	glNormal3f(1, 0, 0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0f, 1.0f, -1.0f);

	glTexCoord2f(0.0, 1.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0, 0.0);
	glVertex3f(1.0f, -1.0f, 1.0f);

	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0f, -1.0f, -1.0f);

	glEnd();

	/*glDisable(GL_TEXTURE_2D);								// If the texture mode is enabled this will end it
	glDisable(GL_LIGHTING);									// If the lighting mode is enabled this will end it
	glDisable(GL_COLOR_MATERIAL);*/							// If the color material mode is enabled this will end it
}

void InitTextures(void)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);						// Sets the texture to repeat if there is empty space around it, the S in "GL_TEXTURE_WRAP_S" means that it is the x axis of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);						// Sets the texture to repeat if there is empty space around it, the T in "GL_TEXTURE_WRAP_T" means that it is the y axis of the texture

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);					// Sets the texture to filter linearly so that the image doesnt look pixelated
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	// Sets the mipmap to filter with nearest which means it will be more pixelated however it will only be shown when far from the camera so it wont be noticable

	texture = ilutGLLoadImage("box.bmp");												// Loads an image and applys it to my texture variable
	texture2 = ilutGLLoadImage("boxSide.bmp");

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_DST);							// Sets the render environment, GL_REPLACE is set so that it ignores the colour given to the cube and only shows the colour in the texture, this only matters if the user has texture mode selected																			
}

void GetDeltaTime()									// Function used to get deltatime 
{
	float ElapsedTime = glutGet(GLUT_ELAPSED_TIME);	// Gets the amount of time since the program started
	deltatime = ElapsedTime - oldElapsedTime;		// Calculates the time between the last and current frame
	oldElapsedTime = ElapsedTime;					// Sets the oldElapsedTime to the current elapsed time so that it can be used next frame 
}

void display()														// This will be called every frame to display everything you see
{
	GetDeltaTime();
	glMatrixMode(GL_PROJECTION);									// Switch to projection mode so that I can change setting related to the camera such as field of view, clipping planes and aspect ratio
	glLoadIdentity();												// This resets the view matrix to its default values
	gluPerspective(60.0, 1.0, 1.0, 11);								// This sets the field of view, aspect ratio, near clipping plane and far clipping plan

	glMatrixMode(GL_MODELVIEW);										// Switch back to modelview so that I can build the cube
	glLoadIdentity();												// Resets to default matrix values

	gluLookAt(0.0, 2.0, 0.0, 0.0, 0.0, -5.0, 0, 1, 0);				// Sets the position of the camera, aswell as where it is looking at, and which direction is up, I set the camera to be higher than the cube so it will be viewed from above

	glViewport(0, 0, screenWidth, screenHeight);					// Create the viewport with the screen width and height variables set at the top of the program
	glScissor(0, 0, screenWidth, screenHeight);						// Define scissor area, nothing beyond this will be rendered this is set as the width and height of the window so that things will only be rendered if they are inside the window
	glEnable(GL_SCISSOR_TEST);										// Enables GL_SCISSOR_TEST which applies the scissor set above

	glClearColor(0.0, 0.0, 0.0, 0.0);								// This sets the colour that will be used when clearing the buffer
	glClear(GL_COLOR_BUFFER_BIT);									// This clears everything from the buffer and gives it the colour set above, this effectivly sets the background colour of the window
	glClear(GL_DEPTH_BUFFER_BIT);									// Clears the buffers depth so that everything will be rendered correctly in the next frame

	currentRotation = currentRotation + rotationSpeed * deltatime;	// Increases the rotation of the cube

	glTranslatef(0.0f, 0.0f, -5.0f);								// Pushes everything back from the camera by -5.0f, this number has to be the same as the one in the gliLookat function from earlier in order for the cube to be in the center of the screen

	glPushMatrix();													// Save the current state of the world, so that I can restore it later
	glRotatef(currentRotation, 0.0, 1.0, 0.0);						// Applys rotation to the world, however, because I am only drawing the cube before poping the matrix i saved it will look like only the cube is rotating. 
		
	drawCube();														// Draws the cube with the new rotation and postiton set
	glPopMatrix();													// Restores the matrix we saved earlier so that anything we build from now is not effected by the rotation

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glDisable(GL_SCISSOR_TEST);										// switches off scissor test, we are not creating any more graphics so it is not needed anymore
				
	glutSwapBuffers();												// swap the front/back buffers, this allows me to render one frame while displaying another, this removes flicker that would be introduced if there was only 1 frame as the screen would go black while rendering

	glutPostRedisplay();											// Forces the current active window to display the buffer
}

void initGL()												// Clears the window, and sets up the light that will be used throughout the scene
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// Clears the buffer and sets the background colour to black
	glClearDepth(1.0f);										// Clears the depth from the buffer, this needs a value
	glEnable(GL_DEPTH_TEST);								// This ensures that each side created is drawn over eachother in the correct order, depending on how far from the camera they are. closer faces should be rendered lsat so that they are visible over faces further away
	glDepthFunc(GL_LEQUAL);									// This checks if a pixel needs to be drawn by checking if the depth is less than or equal to the item behing/infront of it, if it isnt, it will not be drawn
	glMatrixMode(GL_PROJECTION);							// Enables projection mode so that camera settings can be changed

	glLoadIdentity();										// Loads the default matrix values

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);	// This tells the render to use the normals from each face on the cube to determine how lighting is calculated, this is slower than not using the normals but makes it look more realistic
	
	glEnable(GL_LIGHT0);									// This creates a light that will be used to light the scene, if this is not here the scene is just black

	GLfloat AmbientLight[] = { 0.8, 0.8, 0.8};				// Creates values for setting up the ambient light that will be used to light the scene
	GLfloat DiffuseLight[] = { 1.2, 1.2, 1.2};				// Creates values for setting up the diffuse light that will be used to light the scene
	GLfloat SpecularLight[] = { 1.0, 1.0, 1.0};				// Creates values for setting up the specular light that will be used to light the scene
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);			// Sets the ambient light values of the light created earlier using the values created ealier
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);			// Sets the diffuse light values of the light created earlier using the values created ealier
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);		// Sets the specular light values of the light created earlier using the values created ealier
										  
	GLfloat LightPosition[] = { 1.0, 2.0, 0.0, 1.0};		// Set the value that will be used to set the position of the light in the scene
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);		// Sets the position of the light using the value made in the previous line
}

void menu(int num)						// Called when a button in the menu is pressed, given a number that each button has been given
{
	if (num == 0)						// The only button with a value of 0 is the quit button so if that button is pressed we want the program to close 
	{
		glutDestroyWindow(windowID);	// Close the current active window
		exit(0);						// Close the program
	}
	else								// All of the other buttons have been given numbers, we assign the number given by any button that isnt the quit button to the menu value variable, this can then be used when drawing the cube
	{
		menuValue = num;				// Set menuValue to the value given by the button
	}
}

void createMenu(void)							// Function called to create a menu that will apear when the user right clicks
{
	submenu_id = glutCreateMenu(menu);			// The submenu needs to be created first so that it can be attatched to the main menu when it is created, it is given the function that is called when a button is clicked
	glutAddMenuEntry("Textures", 1);			// This button will be in the sub menu and will be used to switch to texture mode, it is given a value of 1 in order to do this
	glutAddMenuEntry("Lighting", 2);			// This is given a different value to the last button so that it can be used to do a different thing
	glutAddMenuEntry("Colour Material", 3);		// This is the last button in the sub menu, they will apear in the order they were created
	glutAddMenuEntry("Texture & Lighting", 4);	// This will enable textures and lighting so that you can have parts of the box more lit than others
	glutAddMenuEntry("All", 5);					// this will enable everything, just to see what it would be like

	menu_id = glutCreateMenu(menu);				// This creates the menu and sets the function called when the button is pressed to the menu function
	glutAddSubMenu("Render Mode", submenu_id);	// Adds the submenu to the menu
	glutAddMenuEntry("Quit", 0);				// Adds a button that will be used to quit

	glutAttachMenu(GLUT_RIGHT_BUTTON);			// This is sets is to that the menu opens when the right mouse button is pressed
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);								// Initialises glut so that i can use all of the variables and functions from it
	glutInitDisplayMode(GLUT_DOUBLE);					// Tells glut that i would like to use double buffering
	glutInitWindowSize(screenWidth, screenHeight);		// Creates a window with the width and height set at the top of the program
	glutInitWindowPosition(50, 50);						// Sets the position of the window on the screen
	glutCreateWindow("Elliot Chester - Assignment 1");	// Sets the title of the window created
	createMenu();										// Creates the menu that will apear when the user right clicks

	ilInit();											// Used to initialise the il library so that i can use functions and variables from it
	iluInit();											// Used to initialise the ilu library so that i can use functions and variables from it
	ilutRenderer(ILUT_OPENGL);							// Tells ILUT that i am using openGL

	glEnable(GL_DEPTH_TEST);							// Enables depth so that the cube will be drawn correctly

	InitTextures();										// Sets the textures that will be used when making the cube
	initGL();											// Calls the function that sets up lighting and renders the first frame
	
	glutDisplayFunc(display);							// Tells glut that the function that needs to be called every frame is display
	glutMainLoop();										// Calls the glut loop, this only needs to be called once as it runs by itself
	return 0;
}

