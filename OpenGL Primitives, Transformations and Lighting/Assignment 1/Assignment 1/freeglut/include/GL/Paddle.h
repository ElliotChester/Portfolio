#pragma once
#include "Vector.h"
#include <IL/ilut.h>

class Paddle
{

private:

	Vector position;
	Vector size;
	int speed;	
	GLint giXRes = 800;
	GLint giYRes = 800;
	GLuint myImage;

public:

	Paddle();
	void draw();
	void move(Vector velocity);
	Vector Getposition();
	void SetPosition(Vector position);
	void initTextures();
};