#pragma once

#include "vector.h"
#include "Matrix.h"
#include "Model.h"
#include <iostream>

using namespace std;

class Cannon
{
public:

	Vector startPosition;
	Vector direction;
	Model cannon;
	Matrix cannonMatrix;
	bool doDraw;

	Cannon();
	~Cannon();

	bool load(const char* cannonModel);
	void draw();
	void update(float deltaTime);
	float calcDistance();
	void updateObb();
};

