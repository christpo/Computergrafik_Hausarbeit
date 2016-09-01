#include "tank.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define TANKSPEED 6;


Tank::Tank()
{
}


Tank::~Tank()
{
}

void Tank::setShootingDelay(float delay)
{
	this->shootingDelay = delay;
}

float Tank::getShootingDelay()
{
	return this->shootingDelay;
}



bool Tank::load(const char* ChassisModel, const char* CononModel, const Vector& StartPos, int bulletCount)
{
	if (this->cassis.load(ChassisModel, false, 100, "toonVertex.glsl", "toonFrag.glsl") && this->conon.load(CononModel, false, 100, "phongVertex.glsl", "phongFrag.glsl"))
	{
		this->cassisMatrix.translation(StartPos);
		this->cononMatrix.translation(StartPos);
		this->cannonMgr.addCannon(bulletCount);
		this->updateObb();

		return true;
	}
	return false;
}


void Tank::steer(float ForwardBackward, float LeftRight, float factor)
{
	this->speed = ForwardBackward * TANKSPEED;
	this->angle = LeftRight * TANKSPEED;

	this->speed *= factor;

	if (this->isFaster)
	{
		this->speed *= 2;
	}

}


void Tank::aim(unsigned int MouseX, unsigned int MouseY, Camera& g_Camera)
{
	
	//cout << x << " : " << y << endl;
	float normalizedx = ((float)MouseX - (g_WindowWidth / 2)) / (g_WindowWidth / 2);
	float normalizedy = -((float)MouseY - (g_WindowHeight / 2)) / (g_WindowHeight / 2);
	//cout <<"normX: " << normalizedx << ", normY: " << normalizedy << endl;

	Vector normalizedXY(normalizedx, normalizedy, 0);

	Matrix invertProjection = g_Camera.getProjectionMatrix();
	invertProjection.invert();

	Matrix invertView = g_Camera.getViewMatrix();
	invertView.invert();

	Vector dirCamRoom = invertProjection * normalizedXY;
	Vector dir = invertView.transformVec3x3(dirCamRoom.normalize());

	Vector o = g_Camera.getPosition();
	float s;
	if (o.triangleIntersection(dir, Vector(-10, 0, -10), Vector(-10, 0, 10), Vector(0, 0, 10), s)) {
		//cout << "hit" << endl;
	}

	this->viewPoint = o + (dir * s);
}

void Tank::aimEnemy(float x, float y) {
	this->viewPoint = Vector(x, 0, y);
}

void Tank::draw()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(this->cassisMatrix);
	this->cassis.drawTriangles();
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(this->cononMatrix);
	this->conon.drawTriangles();
	glPopMatrix();

}

void Tank::update(float deltaTime) {
	
	
	Matrix rot, trans;
	trans.translation(this->speed * deltaTime, 0, 0);
	rot.rotationY(this->angle * deltaTime);
	this->cassisMatrix = this->cassisMatrix * rot *trans;

	Vector forward = this->viewPoint - this->cassisMatrix.translation();
	
	try {
		forward.normalize();
	}
	catch (exception e) {
		forward = Vector(0, 0, 0);
	}
	
	Vector up(0, 1, 0);
	this->cononMatrix = this->cassisMatrix;

	this->cononMatrix.forward(forward);
	this->cononMatrix.up(up);
	this->cononMatrix.right(up.cross(forward));

	Matrix rotConon;
	rotConon.rotationY(-M_PI / 2);
	this->cononMatrix = this->cononMatrix *rotConon;
	this->updateObb();

	draw();
	this->cannonMgr.update(deltaTime);
}

Vector Tank::getPosition() {
	return this->cassisMatrix.translation();
}

void Tank::setZPosition(int xPosition, int zPosition)
{
	this->cassisMatrix.m03 = xPosition;
	this->cassisMatrix.m23 = zPosition;	
}

void Tank::setIsActive(bool isActive)
{
	this->isActive = isActive;
}

bool Tank::getIsActive()
{
	return this->isActive;
}


void Tank::updateObb() {

	Matrix transDownLeft, transDownRight, transUpLeft, transUpRight;

	transDownLeft.translation(this->cassis.boundingBox().Min.X, 0, this->cassis.boundingBox().Min.Z);
	transDownLeft = this->cassisMatrix * transDownLeft;
	this->cassis.obb.downLeft = transDownLeft.translation();
	//cout << "dl x:" << this->cassis.obb.downLeft.X << ", z:" << this->cassis.obb.downLeft.Z << endl;

	transDownRight.translation(this->cassis.boundingBox().Max.X, 0 , this->cassis.boundingBox().Min.Z);
	transDownRight = this->cassisMatrix * transDownRight;
	this->cassis.obb.downRight = transDownRight.translation();
	//cout << "dr x:" << this->cassis.obb.downRight.X << ", z:" << this->cassis.obb.downRight.Z << endl;

	transUpLeft.translation(this->cassis.boundingBox().Min.X, 0, this->cassis.boundingBox().Max.Z);
	transUpLeft = this->cassisMatrix * transUpLeft;
	this->cassis.obb.upLeft = transUpLeft.translation();
	//cout << "ul x:" << this->cassis.obb.upLeft.X << ", z:" << this->cassis.obb.upLeft.Z << endl;

	transUpRight.translation(this->cassis.boundingBox().Max.X, 0, this->cassis.boundingBox().Max.Z);
	transUpRight = this->cassisMatrix * transUpRight;
	this->cassis.obb.upRight = transUpRight.translation();
	//cout << "ur x:" << this->cassis.obb.upRight.X << ", z:" << this->cassis.obb.upRight.Z << endl;
}

