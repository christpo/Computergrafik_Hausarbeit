#include "cannon.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define SPEED 10

Cannon::Cannon()
{
}

Cannon::~Cannon()
{
}

bool Cannon::load(const char * cannonModel)
{
	if (this->cannon.load(cannonModel, true, 10, "toonVertex.glsl", "toonFrag.glsl"))
	{
		this->doDraw = false;
		this->updateObb();

		return true;
	}
	return false;
}

void Cannon::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(this->cannonMatrix);
	this->cannon.drawTriangles();
	glPopMatrix();

}

void Cannon::update(float deltaTime)
{
	Matrix trans;
	trans.translation(SPEED*deltaTime,0,0);
	this->cannonMatrix = this->cannonMatrix * trans;

	this->updateObb();

	draw();
}

float Cannon::calcDistance()
{
	Vector distance = this->cannonMatrix.translation() - this->startPosition;

	return distance.length();
}

void Cannon::updateObb() {

	Matrix transDownLeft, transDownRight, transUpLeft, transUpRight;

	transDownLeft.translation(this->cannon.boundingBox().Min.X,0, this->cannon.boundingBox().Min.Z);
	transDownLeft = this->cannonMatrix * transDownLeft;
	this->cannon.obb.downLeft = transDownLeft.translation();

	transDownRight.translation(this->cannon.boundingBox().Max.X, 0, this->cannon.boundingBox().Min.Z);
	transDownRight = this->cannonMatrix * transDownRight;
	this->cannon.obb.downRight = transDownRight.translation();

	transUpLeft.translation(this->cannon.boundingBox().Min.X, 0, this->cannon.boundingBox().Max.Z);
	transUpLeft = this->cannonMatrix * transUpLeft;
	this->cannon.obb.upLeft = transUpLeft.translation();

	transUpRight.translation(this->cannon.boundingBox().Max.X,0, this->cannon.boundingBox().Max.Z);
	transUpRight = this->cannonMatrix * transUpRight;
	this->cannon.obb.upRight = transUpRight.translation();
}
