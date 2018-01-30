#pragma once
#include "Vector.h"

class Ball
{

private: 

	float radius;
	Vector position;
	Vector startPosition;
	int speed;

public:

	Ball();
	void draw();
	void setPosition(Vector position);
	void move(Vector velocity);
};

