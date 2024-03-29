#include "stdafx.h"

#include <iostream>
#include <vector>
#include <string>

#include <glew.h>
#include <glut.h>					// glut is used to do all of the window creation and rendering
#include <IL/ilut.h>				// ilut is only needed to load in the texture for the cube
#include "textfile.h"

#pragma region Variables

GLfloat currentRotation = 0;															// Current y rotation of the cube
float rotationSpeed = 60.0f;															// Speed of the rotation of the cube
GLint screenWidth = 800;																// Width of the window in pixels
GLint screenHeight = 800;																// Height of the window in pixels
GLint windowID;																			// The ID number of the window
int menu_id, submenu1_id, submenu2_id, submenu3_id;										// The ID of the submenu within the main me
int menuValue = 1, menuValue1 = 1;														// The value given by the sub menu when a button is pressed. It is set to default to 1 so that it automatically renders textures 
float DeltaTime;																		// This will hold the amount of time since the last and current frame
float oldElapsedTime = 0;																// This is the amount of time that has passed in the program from the start to the last frame, it is used to calculate the deltatime

double fov = 60.0, minFOV = 10, maxFOV = 60;											// This is the value that will be used to set the field of view of the camera.

double lastLookAtX, lastLookAtY, lookAtX, lookAtY = 0;									// These variables store the last lookat position of the camera as well as the new target
float lookspeed = 0.5f, zoomSpeed = 1.0f;												// These variables control the speed of the camera transformations
float minX = -5.5, maxX = 5.5, miny = -3, maxy = 3;										// These variables hold the upper and lower limits of the camera lookat transformations

float scale = 1;																		// This holds the scale multiplier that will be applied to the 3d models in the scene;

struct Triangle																			// This struct will store all of the variables needed to draw a single triangle
{
	int verts[3];																		
	int uvs[3];
	int normals[3];
};

struct Model																			// This will hold a single model by storing all of the information taken from the models obj file.
{
	
	std::vector<std::vector<float>> vertexPoints = std::vector<std::vector<float>>();	//
	std::vector<std::vector<float>> vertexNormals = std::vector<std::vector<float>>();	// These 3 vectors store all of the vertex coordinates, normals, and uv coordinates taken from the model
	std::vector<std::vector<float>> vertexUVs = std::vector<std::vector<float>>();		//
	
	std::vector<Triangle> TriangleList = std::vector<Triangle>();						// This vector holds all of the triangles of the model

};

Model FidgetSpinner;																	// 
GLuint SpinnerTexture;																	// These variables will store each model needed 
Model SpaceShip;																		// as well as the texture that will be applied
GLuint ShipTexture;																		//

GLuint shader, vbo = 0, vao = 0, vaoHandle;;															// This stores the different values needed to use shaders

float positionData[] =																	// This stores all of the points that will be used in the shaders
{
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,

	-0.5f, -0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};

float colorData[] = {																	// This stores all of the colours that will be used in the shaders
	0.8f, 0.19f, 1.0f,
	0.39f, 0.19f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.8f, 0.19f, 1.0f,
	0.0f, 0.0f, 1.0f,
	0.39f, 0.19f, 1.0f,
};

char *vertexFile = NULL;																		// These point to the text files that will be used in the shaders
char *fragmentFile = NULL;																		//		

#pragma endregion
 
Model LoadOBJ(const char* fileName)																	// This takes a const char, and returns a model, this will allow me to load in a model by passing it the file name of the model later in the code
{
	std::cout << "Loading model with name " << fileName << std::endl;

	Model newModel;																					// Creates a new temporaty model that will be used to load the values read from the file into 

	char ch = 'l';																					// These will be used to get the first character in the lines within the obj file
	char ch2 = 'l';																					//

	FILE *modelFile;																				// a pointer to a file that will be read from
	
	int read;																						// This will hold all of the information taken from each line as it is read
	
	GLfloat x, y, z;																				// this will be used to store the x,y and z values taken from the vertex points, uvs and normals 
	
	int v1, v2, v3, v4;																				//
	int uv1, uv2, uv3, uv4;																			// These will be used to store the ID of each vertex point, uv and normal
	int n1, n2, n3, n4;																				//

	modelFile = fopen(fileName, "r");																// This will open the obj file containing the model with the name of the vile given above

	if (!modelFile)																					//
	{																								// 
		std::cout << "Cant read file " << fileName << std::endl;									// A debug cout that will show if the file was not found, and stop the program from trying to read from nothing
		return Model();																				//
	}																								//

	while (!(feof(modelFile)))																		// while we are not at the end of the file
	{																								
		read = fscanf(modelFile, "%c %f %f %f [^/n]", &ch, &x, &y, &z);								// This will read the line of the file that we are on, and record a char, then ignore a space, the read a float, space, float, space, float, then end the line
		//std::cout << ch << " " << x << " " << y << " " << z << std::endl;							
		if (read == 4 && ch == 'v')																	// If it read all of the chars and floats correctly and the char is a v, it will record the vertex
		{																							
			//std::cout << fileName << ": found the point" << std::endl;							
			std::vector<float> newPoint = std::vector<float>();										// Creates a new vectore that will store a vertex point that will be added to the temp model
			newPoint.push_back(x);																	// 
			newPoint.push_back(y);																	// Adds x, y and z to the vector
			newPoint.push_back(z);																	//
			newModel.vertexPoints.push_back(newPoint);												// pushes the new point to the back of the vertex points vector in the temp model
		}																		
	}																			

	rewind(modelFile);																				// Rewinds the file so that it can be read again

	while (!(feof(modelFile))) {																	// reads through the file to search for uv coordinates and normals
		read = fscanf(modelFile, "%c%c %f %f %f [^/n]", &ch, &ch2, &x, &y, &z);						// this is the same as the last read except it finds 2 letters at the beginning of the line
																									
		if (read == 5 && ch == 'v'  && ch2 == 'n')													 
		{																							
			//std::cout << "found the normal" << std::endl;											
			//std::cout << ch << ch2 << " " << x << "/" << y << "/" << z << std::end				
			std::vector<float> newNormal = std::vector<float>();									
			newNormal.push_back(x);																	
			newNormal.push_back(y);																	
			newNormal.push_back(z);																	
			newModel.vertexNormals.push_back(newNormal);											
		}																							
		else if (read == 4 && ch == 'v' && ch2 == 't')												
		{																							
			//std::cout << "found the UV" << std::endl;												
			//std::cout << ch << ch2 << " " << x << "/" << y << "/" << z << std::endl;				
			std::vector<float> newUV = std::vector<float>();										
			newUV.push_back(x);																		
			newUV.push_back(y);																		
			newModel.vertexUVs.push_back(newUV);													
		}																							
	}																								
																									
	rewind(modelFile);

	while (!(feof(modelFile))) {																	
		read = fscanf(modelFile, "%c %i/%i/%i %i/%i/%i %i/%i/%i %i/%i/%i\n]", &ch, 					// this takes in the face data, first the 1st char of the line, then 4 sets of 3 ints, with a / between each int in a set of 3
					  &v1, &uv1, &n1, 																// face 1
					  &v2, &uv2, &n2, 																// face 2
					  &v3, &uv3, &n3, 																// face 3
					  &v4, &uv4, &n4);																// face 4

		if (read == 13 && ch == 'f')																// This checks for a quad, it will convert it into 2 triangles so that the gpu is less stressed later on
		{																							
			//std::cout << "found the Quad" << std::endl;											
			//std::cout << ch << " " << v1 << " " << v2 << " " << v3 << " " << v4 << std::endl;		
			Triangle newTriangle;																	// creates a new temporary triangle
			newTriangle.verts[0] = v1;																// 
			newTriangle.verts[1] = v2;																//
			newTriangle.verts[2] = v3;																//
			newTriangle.normals[0] = n1;															//
			newTriangle.normals[1] = n2;															// sets all of the data in the triangle with the first 3 points of the data taken from the line
			newTriangle.normals[2] = n3;															//
			newTriangle.uvs[0] = uv1;																//
			newTriangle.uvs[1] = uv2;																//
			newTriangle.uvs[2] = uv3;																//

			newModel.TriangleList.push_back(newTriangle);											//pushes that triangle to the back of the triangle list in the temp model
																									

			newTriangle.verts[0] = v1;																//
			newTriangle.verts[1] = v3;																//
			newTriangle.verts[2] = v4;																//
			newTriangle.normals[0] = n1;															//
			newTriangle.normals[1] = n3;															// overwrites all of the data in the triangle with points 1, 3 and 4 of the data taken from the line
			newTriangle.normals[2] = n4;															//
			newTriangle.uvs[0] = uv1;																//
			newTriangle.uvs[1] = uv3;																//
			newTriangle.uvs[2] = uv4;																//

			newModel.TriangleList.push_back(newTriangle);											// pushes the triangle to the back of the list
		}																							
																									
		if (read == 10 && ch == 'f')																// Checks for a triangle
		{																							
			//std::cout << "found the Triangle" << std::endl;										
			//std::cout << ch << " " << v1 << " " << v2 << " " << v3 << " " << v4 << std::endl;		
			Triangle newTriangle;																	//
			newTriangle.verts[0] = v1;																//
			newTriangle.verts[1] = v2;																//
			newTriangle.verts[2] = v3;																//
			newTriangle.normals[0] = n1;															//
			newTriangle.normals[1] = n2;															// This is the same method as the quads except it doesnt need to split it up
			newTriangle.normals[2] = n3;															//
			newTriangle.uvs[0] = uv1;																//
			newTriangle.uvs[1] = uv2;																//
			newTriangle.uvs[2] = uv3;																//
																									//
			newModel.TriangleList.push_back(newTriangle);											//
		}																							
	}																								
																									
	return newModel;																				// returns the temp model so that it can be drawn later
}																									

void InitTextures(void)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);						// Sets the texture to repeat if there is empty space around it, the S in "GL_TEXTURE_WRAP_S" means that it is the x axis of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);						// Sets the texture to repeat if there is empty space around it, the T in "GL_TEXTURE_WRAP_T" means that it is the y axis of the texture


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);					// Sets the texture to filter linearly so that the image doesnt look pixelated
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);	// Sets the mipmap to filter with nearest which means it will be more pixelated however it will only be shown when far from the camera so it wont be noticable

	SpinnerTexture = ilutGLLoadImage("Fidget_Spinner.bmp");								// Loads an image and applys it to my texture variable
	ShipTexture = ilutGLLoadImage("SpaceShip.jpg");

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND_DST);						// Sets the render environment, GL_REPLACE is set so that it ignores the colour given to the cube and only shows the colour in the texture, this only matters if the user has texture mode selected																			
}

void DrawMode()											//This function controls which render modes the program uses when drawing 3d models 
{

	switch (menuValue)
	{
		case 1:											// This case will show only textures
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);							
		glPolygonMode(GL_FRONT, GL_FILL);				// this is called simply to make sure it is rendered properly
		glColor3f(1.0f, 1.0f, 1.0f);
		break;

		case 2:											// This case will show only lighting
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);								
		glPolygonMode(GL_FRONT, GL_FILL);
		break;

		case 3:											// This case will show only colours
		glDisable(GL_LIGHTING);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_COLOR_MATERIAL);					
		glPolygonMode(GL_FRONT, GL_FILL);
		glColor3f(0.8f, 0.4f, 0.4f);
		break;

		case 4:											// This case will show only textures and lighting
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT, GL_FILL);
		break;

		case 5:											// This case will enable everthing, but lighting doesnt actually apear because colour material overrides it 
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT, GL_FILL);
		glColor3f(0.8f, 0.4f, 0.4f);
		break;

		case 6:											// This case will show the 3d model as a wireframe
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glColor3f(1.0f, 1.0f, 1.0f);
		break;

		case 8:											// This case will show only the vertices of the model
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glColor3f(1.0f, 1.0f, 1.0f);
		break;

		default:
		break;
	}
}

void DrawModel(Model model)														// Function used to create a cube and set the correct render style for it.
{			
	GLfloat DiffuseMaterial[] = { 0.6, 0.6, 0.6 };								// These set the variables that will be used to set colours
	GLfloat AmbientMaterial[] = { 0.2, 0.2, 0.2 };								//

	glMaterialfv(GL_FRONT, GL_DIFFUSE, DiffuseMaterial);						// These use the colours that were created and set them to the diffuse and ambient materials of the model
	glMaterialfv(GL_FRONT, GL_AMBIENT, AmbientMaterial);						//

	glBegin(GL_TRIANGLES);														// The model will be drawn with only triangles
																				
	for (int i = 0; i < model.TriangleList.size(); i++)							// This will loop through all of the triangles in the model
	{
		//std::cout << "drawing " << i << std::endl;
		int v1 = model.TriangleList[i].verts[0] - 1;							//
		int v2 = model.TriangleList[i].verts[1] - 1;							// These get the verticies that will be used in making the triangle
		int v3 = model.TriangleList[i].verts[2] - 1;							//

		int uv1 = model.TriangleList[i].uvs[0] - 1;								//
		int uv2 = model.TriangleList[i].uvs[1] - 1;								// These get the uv coordinates that will be used in making the triangle
		int uv3 = model.TriangleList[i].uvs[2] - 1;								//
																				
		int n1 = model.TriangleList[i].normals[0] - 1;							//
		int n2 = model.TriangleList[i].normals[1] - 1;							// These get the normals that will be used in making the triangle
		int n3 = model.TriangleList[i].normals[2] - 1;							//

		//std::cout << " " << v1 << " " << v2 << " " << v3 << std::endl;
		//std::cout << " " << uv1 << " " << uv2 << " " << uv3 << std::endl;		
		//std::cout << " " << n1 << " " << n2 << " " << n3 << std::endl;

		glTexCoord2f(model.vertexUVs[uv1][0], model.vertexUVs[uv1][1]);			// This gets the uv coordinates from the list of all the uv coordinates in the model using the uv set above, and sets it for the next vertex

		glNormal3f(model.vertexNormals[n1][0],									// 
				   model.vertexNormals[n1][1], 									// This gets the normal from the list of all the normal in the model using the normal set above, and sets it for the next vertex 
				   model.vertexNormals[n1][2]);									// 

		glVertex3f(model.vertexPoints[v1][0] * scale,							//
				   model.vertexPoints[v1][1] * scale, 							// This gets the vertex coordinates from the list of all the vertex coordinates in the model using the vertex set above, this will then be multplied by the scale, this is where the actual vertex is drawn
				   model.vertexPoints[v1][2] * scale);							//


		glTexCoord2f(model.vertexUVs[uv2][0], model.vertexUVs[uv2][1]);			//
																				//
		glNormal3f(model.vertexNormals[n2][0], 									//
				   model.vertexNormals[n2][1], 									//
				   model.vertexNormals[n2][2]);									//
																				//
		glVertex3f(model.vertexPoints[v2][0] * scale, 							//
				   model.vertexPoints[v2][1] * scale, 							//
				   model.vertexPoints[v2][2] * scale);							//
																				// These are the same as above, however they build the next 2 verticies that make up the triangle
																				//
		glTexCoord2f(model.vertexUVs[uv3][0], model.vertexUVs[uv3][1]);			//
																				//
		glNormal3f(model.vertexNormals[n3][0], 									//
				   model.vertexNormals[n3][1], 									//
				   model.vertexNormals[n3][2]);									//
																				//
		glVertex3f(model.vertexPoints[v3][0] * scale, 							//
				   model.vertexPoints[v3][1] * scale, 							//
				   model.vertexPoints[v3][2] * scale);							//
	}

	glEnd();
}

void GetDeltaTime()									// Function used to get deltatime 
{
	float ElapsedTime = glutGet(GLUT_ELAPSED_TIME);	// Gets the amount of time since the program started
	DeltaTime = (ElapsedTime - oldElapsedTime)/1000;// Calculates the time between the last and current frame
	oldElapsedTime = ElapsedTime;					// Sets the oldElapsedTime to the current elapsed time so that it can be used next frame 
}

void PrintText(std::string text, int x, int y)						// This function will print text on the screen with a given
{						
	glMatrixMode(GL_PROJECTION);									// switches to projection mode so that i can switch to orthographic camera
	glPushMatrix();													// saves the current matrix values so that they can be retrieved later
	glLoadIdentity();												// resets the matrix
	gluOrtho2D(0, 800, 0, 800);										// sets the camera to a 2d orthographic camera so taht the text can be rendered properly
																	
	glDisable(GL_TEXTURE_2D);										// disables textures and lighting so that the text is just a plain colour and not effected by anything else in the scene
	glDisable(GL_LIGHTING);											// 
	glColor3f(0.8f, 0.8f, 0.8f);									// sets the colour of the text to almost white
																	
	glMatrixMode(GL_MODELVIEW);										// enter modelview mode so that the text can be drawn
	glPushMatrix();													// pushe the matrix so that it can be retrieved later
	glLoadIdentity();												// reset the view
	glRasterPos2i(x, y);											// sets the position of the text
	
	for (size_t i = 0; i < text.size(); i++)						// for each letter in the string given when printtext is called
	{																
		char letter = text[i];										// 
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, letter);		// show the text on the screen
	}																
																	
	glPopMatrix();													// pop the matrix so that it is put back to how it was before the text was edited
	glPopMatrix();													// 
}

void KeyboardInput(int key, int x, int y)				// glut function that will allow me to use keyboard input
{
	switch (key)										// switch that checks the key pressed
	{		
		case GLUT_KEY_UP:								// up arrow key
		//std::cout << "up" << std::endl;				// 
		if(lookAtY < maxy)								// 
			lookAtY = lastLookAtY + lookspeed;			// changes the value that pans the camera up
		break;											// 
														// 
		case GLUT_KEY_DOWN:								// down arrow key
		//std::cout << "down" << std::endl;				// 
		if (lookAtY > miny)								// 
			lookAtY = lastLookAtY - lookspeed;			// changes the value that pans the camera down
		break;											// 
														// 
		case GLUT_KEY_LEFT:								// left arrow key
		//std::cout << "left" << std::endl;				// 
		if (lookAtX > minX)								// 
			lookAtX = lastLookAtX - lookspeed;			// changes the value that pans the camera left
		break;											// 
														// 
		case GLUT_KEY_RIGHT:							// right arrow key
		//std::cout << "right" << std::endl;			// 
		if (lookAtX < maxX)								// 
			lookAtX = lastLookAtX + lookspeed;			// changes the value that pans the camera right
		break;											// 
														// 
		case GLUT_KEY_PAGE_UP:							// page up key
		//std::cout << "zoom+" << std::endl;			// 
		if (fov > minFOV)								// 
			fov -= zoomSpeed;							// changes the value that zooms the camera in
		break;											// 
														// 
		case GLUT_KEY_PAGE_DOWN:						// page down key
		//std::cout << "zoom-" << std::endl;			// 
		if (fov < maxFOV)								// 
			fov += zoomSpeed;							// changes the value that zooms the camera out
		break;											// 
														// 
		case GLUT_KEY_HOME:								// home key
		rotationSpeed = 1500;							// changes the value that sets the speed of the fidget spinner
		break;											// 
	}													// 
	glutPostRedisplay();								// 
}														// 

void Shaders()
{
	shader = glCreateProgram();															// This creates the shader program so that shaders can be created and used
																						
	GLuint vertID = glCreateShader(GL_VERTEX_SHADER);									// This creates a new vertex shader
	GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);									// This creates a new fragment shader
																						
	vertexFile = textFileRead("Vertex.vert");											// This reads the vertex file and assigns the information to a char*
	fragmentFile = textFileRead("Fragment.frag");										// This reads the fragment file and assigns the information to a char*
																						
	glBindAttribLocation(shader, 0, "VertexPosition");									// This creates a new attribute for the shader, this will store vertex positions
																						   
	glBindAttribLocation(shader, 1, "VertexColor");										// This creates a new attribute for the shader, this will store vertex colours
																						   
	GLuint vboHandles[2];																// This lets the shader know there are 2 attributes that will be used
	glGenBuffers(2, vboHandles);														// This creates the buffers that will be used
	GLuint posBufferHandle = vboHandles[0];												// These split up the vbo handles into 2 individual ints
	GLuint colorBufferHandle = vboHandles[1];											// 
																						

	glBindBuffer(GL_ARRAY_BUFFER, posBufferHandle);										// these lines populate the position buffer with the position data variable at the top of the page
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);	// 
																						
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);									// these lines populate the colour buffer with the colour data variable at the top of the page
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), colorData, GL_STATIC_DRAW);		// 
																						
	glGenVertexArrays(1, &vaoHandle);													// this creates a vertex array and is given the buffers made before
	glBindVertexArray(vaoHandle);														// 
																						
	glEnableVertexAttribArray(0); 														// these enables the vertex array, both of the buffers
	glEnableVertexAttribArray(1); 														//
																						
																					
	glBindBuffer(GL_ARRAY_BUFFER, posBufferHandle);										// sets up the position buffer to be used
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);				// 
																						
	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);									// sets up the colour buffer to be used
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);				// 
																						
																						
	glShaderSource(vertID, 1, &vertexFile, NULL);										// sets the sources of the shader as the text files loaded earlier
	glShaderSource(fragID, 1, &fragmentFile, NULL);										// 
																						
	glCompileShader(vertID);															// compiles the shader
	glCompileShader(fragID);															// 
																					
	glAttachShader(shader, vertID);														// attatches the new shader to the shaders to the shader program
	glAttachShader(shader, fragID);														// 
																					
	glLinkProgram(shader);																// finishes setting up the program
}

double DLerpTowards(double current, double target, double speed)	// Function that will lerp a double between 2 points, it takes a double, target double and speed
{																	
	if (current == target)											// If the current is the same as the target there is no need to change anything
		return target;												
																	 
	double out = target - current;									// Creates a temp double that will be changed to make the output value, it is set as the distance between the 2 points given
																	// 
	if (speed > 1) 													// I wanted the speed input value to be between 0 and 1, 0 being infinite lerp, and 1 being no lerp, so if the speed is over 1 it sets it to 1
		speed = 1;													// 
																	// 
	out *= speed;													// Multiply the distance between the points by the speed
	out *= DeltaTime;												// multiply that by deltatime so that it doesnt change speed depending on frame rate
																	
	out += current;													// adds the current value to the new out value to ive it its new
	return out;														 
}

double DMoveTowards(double current, double target, double speed)	// function that will smoothly move between 2 doublesat a constant speed, takes the current value, target value and 
{																	
	if (current == target)											// If the current is the same as the target there is no need to change anything
		return target;												
																	
	bool over = (target > current);									// check if the target is a higher value than the current position 
																	
	double out = 0;													// creates a double that will be returned

	if (over) {														// if the target is over the current value, out is set to speed
		out = speed;												
	}																
	else															// else, out is set to -speed
	{																
		out = -speed;									   
	}																
																	
	out *= DeltaTime;												// multiply out by deltatime to make the result of the movetowards a constant speed with varying framerates
																	
	out += current;													// add the current 
	if (over) {														// if the target is over the current
		if (out > target) 
		{											
			out = target;											// if the new value is over the target value, make the new value the target value
		}															
	}																
	else 															// if the target is under the current
	{																
		if (out < target)											// if the out is less than the target it means it has gone past the point it wants to get to so it is set to the point
		{											
			out = target;											 
		}															
	}																
																	
	return out;														
}

void Display()															// This will be called every frame to display everything you see
{
	GetDeltaTime();
	glMatrixMode(GL_PROJECTION);										// Switch to projection mode so that I can change setting related to the camera such as field of view, clipping planes and aspect ratio
	glLoadIdentity();													// This resets the view matrix to its default values
	gluPerspective(fov, 1.0, 1.0, 100);									// This sets the field of view, aspect ratio, near clipping plane and far clipping plan

	glMatrixMode(GL_MODELVIEW);											// Switch back to modelview so that I can build the cube
	glLoadIdentity();													// Resets to default matrix values

	double newx = DMoveTowards(lastLookAtX, lookAtX, 2);
	lastLookAtX = newx;
	double newy = DMoveTowards(lastLookAtY, lookAtY, 2);
	lastLookAtY = newy;

	gluLookAt(0.0, 10.0, 0.0, newx, newy, -20.0, 0, 1, 0);				// Sets the position of the camera, aswell as where it is looking at, and which direction is up, I set the camera to be higher than the cube so it will be viewed from above

	//gluLookAt(0.0, 10.0, 0.0, lookAtX, lookAtY, -20.0, 0, 1, 0);

	glViewport(0, 0, screenWidth, screenHeight);						// Create the viewport with the screen width and height variables set at the top of the program
	glScissor(0, 0, screenWidth, screenHeight);							// Define scissor area, nothing beyond this will be rendered this is set as the width and height of the window so that things will only be rendered if they are inside the window
	glEnable(GL_SCISSOR_TEST);											// Enables GL_SCISSOR_TEST which applies the scissor set above

	glClearColor(0.0, 0.0, 0.0, 0.0);									// This sets the colour that will be used when clearing the buffer
	glClear(GL_COLOR_BUFFER_BIT);										// This clears everything from the buffer and gives it the colour set above, this effectivly sets the background colour of the window
	glClear(GL_DEPTH_BUFFER_BIT);										// Clears the buffers depth so that everything will be rendered correctly in the next frame

	glTranslatef(0.0f, 0.0f, -20.0f);									// Pushes everything back from the camera by -5.0f, this number has to be the same as the one in the gliLookat function from earlier in order for the cube to be in the center of the screen

	DrawMode();
	
	switch (menuValue1)
	{
		case 1:

		currentRotation = currentRotation + rotationSpeed * DeltaTime;	// Increases the rotation of the cube

		if (rotationSpeed > 0) {
			rotationSpeed -= (100 * DeltaTime);
		}
		if (rotationSpeed < 0) {
			rotationSpeed = 0;
		}

		glPushMatrix();													// Save the current state of the world, so that I can restore it later
		glRotatef(currentRotation, 0.0, 1.0, 0.0);						// Applys rotation to the world, however, because I am only drawing the cube before poping the matrix i saved it will look like only the cube is rotating. 
		
		glBindTexture(GL_TEXTURE_2D, SpinnerTexture);
		DrawModel(FidgetSpinner);										// Draws the cube with the new rotation and postiton set

		glPopMatrix();

		PrintText("Press Home to spin the Fidget Spinner", 10, 720);

		break;

		case 2:
		glUseProgram(shader);
		glBindVertexArray(vaoHandle);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glUseProgram(0);
		break;

		case 3:
		currentRotation += 10 * DeltaTime;
		glPushMatrix();													// Save the current state of the world, so that I can restore it later
		glRotatef(currentRotation, 0.0, 1.0, 0.0);						// Applys rotation to the world, however, because I am only drawing the cube before poping the matrix i saved it will look like only the cube is rotating. 
		
		glBindTexture(GL_TEXTURE_2D, ShipTexture);
		DrawModel(SpaceShip);											// Draws the cube with the new rotation and postiton set

		glPopMatrix();
		break;

		default:
		break;
	}

	PrintText("Right click to open the menu", 10, 780);
	PrintText("Arrow Keys to look around", 10, 760);
	PrintText("PgUp and PgDn to zoom in and out", 10, 740);

	glDisable(GL_SCISSOR_TEST);											// switches off scissor test, we are not creating any more graphics so it is not needed anymore

	glutSwapBuffers();													// swap the front/back buffers, this allows me to render one frame while displaying another, this removes flicker that would be introduced if there was only 1 frame as the screen would go black while rendering

	glutPostRedisplay();												// Forces the current active window to display the buffer
}

void InitGL()												// Clears the window, and sets up the light that will be used throughout the scene
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// Clears the buffer and sets the background colour to black
	glClearDepth(1.0f);										// Clears the depth from the buffer, this needs a value
	glEnable(GL_DEPTH_TEST);								// This ensures that each side created is drawn over eachother in the correct order, depending on how far from the camera they are. closer faces should be rendered lsat so that they are visible over faces further away
	glDepthFunc(GL_LEQUAL);									// This checks if a pixel needs to be drawn by checking if the depth is less than or equal to the item behing/infront of it, if it isnt, it will not be drawn
	glMatrixMode(GL_PROJECTION);							// Enables projection mode so that camera settings can be changed

	glLoadIdentity();										// Loads the default matrix values

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);	// This tells the render to use the normals from each face on the cube to determine how lighting is calculated, this is slower than not using the normals but makes it look more realistic

	glEnable(GL_LIGHT0);									// This creates a light that will be used to light the scene, if this is not here the scene is just black

	GLfloat AmbientLight[] = { 0.8, 0.8, 0.8 };				// Creates values for setting up the ambient light that will be used to light the scene
	GLfloat DiffuseLight[] = { 1.2, 1.2, 1.2 };				// Creates values for setting up the diffuse light that will be used to light the scene
	GLfloat SpecularLight[] = { 1.0, 1.0, 1.0 };			// Creates values for setting up the specular light that will be used to light the scene
	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientLight);			// Sets the ambient light values of the light created earlier using the values created ealier
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);			// Sets the diffuse light values of the light created earlier using the values created ealier
	glLightfv(GL_LIGHT0, GL_SPECULAR, SpecularLight);		// Sets the specular light values of the light created earlier using the values created ealier

	GLfloat LightPosition[] = { 1.0, 2.0, 0.0, 1.0 };		// Set the value that will be used to set the position of the light in the scene
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);		// Sets the position of the light using the value made in the previous line

	InitTextures();											// Sets the textures that will be used when making the cube
	FidgetSpinner = LoadOBJ("Fidget_Spinner.obj");			// assigns the fidgetspinner its information
	SpaceShip = LoadOBJ("SpaceShip.obj");					// assigns the spaceship its information
}

void Menu(int num)						// Called when a button in the menu is pressed, given a number that each button has been given
{
	if (num == 0)						// The only button with a value of 0 is the quit button so if that button is pressed we want the program to close 
	{
		glutDestroyWindow(windowID);	// Close the current active window
		exit(0);						// Close the program
	}
	else if (num == 7) 							// All of the other buttons have been given numbers, we assign the number given by any button that isnt the quit button to the menu value variable, this can then be used when drawing the cube
	{
		
	}
	else
	{
		menuValue = num;				// Set menuValue to the value given by the button
	}
}

void ScaleMenu(int num)						// Called when a button in the menu is pressed, given a number that each button has been given
{
	switch (num) // depending on the button pressed, it will set the scale value
	{
		case 1:
		scale = 0.5f;
		break;

		case 2:
		scale = 0.75f;
		break;
		
		case 3:
		scale = 1.0f;
		break;
		
		case 4:
		scale = 1.25f;
		break;
		
		case 5:
		scale = 1.5f;
		break;

		case 6:
		scale = 2.0f;
		break;

		default:
		break;
	}	
}

void OptionMenu(int num)						// Called when a button in the menu is pressed, given a number that each button has been given
{
	menuValue1 = num;
}

void createMenu(void)											// Function called to create a menu that will apear when the user right clicks
{
	submenu1_id = glutCreateMenu(Menu);							// The submenu needs to be created before the main menu so that it can be attatched to the main menu when it is created, it is given the function that is called when a button is clicked
	glutAddMenuEntry("Textures", 1);							//  
	glutAddMenuEntry("Lighting", 2);							// 
	glutAddMenuEntry("Colour Material", 3);						// 
	glutAddMenuEntry("Texture & Lighting", 4);					// These will change 
	glutAddMenuEntry("All of the above", 5);					// 
	glutAddMenuEntry("WireFrame", 6);							//
	glutAddMenuEntry("Dots", 8);								//

	submenu2_id = glutCreateMenu(ScaleMenu);					// The submenu needs to be created before the main menu so that it can be attatched to the main menu when it is created, it is given the function that is called when a button is clicked
	glutAddMenuEntry("0.5x", 1);								// 
	glutAddMenuEntry("0.75x", 2);								//
	glutAddMenuEntry("1.0x", 3);								// these buttons will change the scale of the model on the screen
	glutAddMenuEntry("1.25x", 4);								//
	glutAddMenuEntry("1.5x", 5);								//
	glutAddMenuEntry("2.0x", 6);								//

	submenu3_id = glutCreateMenu(OptionMenu);					// The submenu needs to be created before the main menu so that it can be attatched to the main menu when it is created, it is given the function that is called when a button is clicked
	glutAddMenuEntry("Fidget Spinner", 1);						// This will change between the shaders demo and 2 different models
	glutAddMenuEntry("Shader", 2);								
	glutAddMenuEntry("SpaceShip", 3);

	menu_id = glutCreateMenu(Menu);								// This creates the menu and sets the function called when the button is pressed to the menu function
	glutAddSubMenu("Show", submenu3_id);							
	glutAddSubMenu("Render Mode (Models Only)", submenu1_id);	// Adds the submenu to the menu
	glutAddSubMenu("Scale", submenu2_id);						// Adds the submenu to the menu
	glutAddMenuEntry("Quit", 0);								// Adds a button that will be used to quit

	glutAttachMenu(GLUT_RIGHT_BUTTON);							// This is sets is to that the menu opens when the right mouse button is pressed
}

int main(int argc, char** argv) 
{
	std::cout << "This is a 3D model loader created by Elliot Chester\n" <<
		"It uses GLUT and ILUT to render the models\n" <<
		"This was a Graphics Programming Assignment for my 2nd year of university" <<
		std::endl << std::endl;

	glutInit(&argc, argv);								// Initialises glut so that i can use all of the variables and functions from it
	glutInitDisplayMode(GLUT_DOUBLE);					// Tells glut that i would like to use double buffering
	glutInitWindowSize(screenWidth, screenHeight);		// Creates a window with the width and height set at the top of the program
	glutInitWindowPosition(50, 50);						// Sets the position of the window on the screen
	glutCreateWindow("Elliot Chester - Assignment 2");	// Sets the title of the window created
	createMenu();										// Creates the menu that will apear when the user right clicks

	ilInit();											// Used to initialise the il library so that i can use functions and variables from it
	iluInit();											// Used to initialise the ilu library so that i can use functions and variables from it
	ilutRenderer(ILUT_OPENGL);							// Tells ILUT that i am using openGL
	glewInit();

	glEnable(GL_DEPTH_TEST);							// Enables depth so that the cube will be drawn correctly

	InitGL();											// Calls the function that sets up lighting and renders the first frame

	Shaders();											// this will show the shaders

	glutSpecialFunc(KeyboardInput);
	glutDisplayFunc(Display);							// Tells glut that the function that needs to be called every frame is display
	glutMainLoop();										// Calls the glut loop, this only needs to be called once as it runs by itself
	return 0;
}

