#pragma once
#include "vector.h"
#include "Model.h"
#include "Matrix.h"
#include "Camera.h"
#include "cannonMgr.h"

class Tank
{
public:
	Tank();
	~Tank();
	bool load(const char* ChassisModel, const char* CononModel, const Vector& StartPos, int bulletCount);
	void steer(float ForwardBackward, float LeftRight, float factor);
	void aim(unsigned int MouseX, unsigned int MouseY, Camera& g_Camera);
	void draw();
	void update(float deltaTime);
	Vector getPosition();
	void aimEnemy(float x, float y);
	void updateObb();
	void setShootingDelay(float delay);
	float getShootingDelay();
	void setZPosition(int xPosition, int zPosition);
	

	Model conon;
	Matrix cononMatrix;
	Vector cononForward;
	Vector viewPoint;
	Model cassis;
	Matrix cassisMatrix;
	Vector Position;
	float angle;
	float speed;
	CannonMgr cannonMgr;
	bool hasNotShoot = true;
	float shootingDelay = 0.0f;
	bool isActive = false;
	void setIsActive(bool isActive);
	bool getIsActive();
	bool isFaster = false;


	Model dl;
	Matrix dlMtx;
	Model dr;
	Matrix drMtx;
	Model ul;
	Matrix ulMtx;
	Model ur;
	Matrix urMtx;
};

