#include "cannonMgr.h"
#include <iostream>

#define MAXDISTANCE 10
#define BULLETCOUNT 10


void CannonMgr::setIdX(int idx)
{
	this->idx = idx;
}

void CannonMgr::addCannon(int bulletCount)
{
	this->bulletCount = bulletCount;
	for (int i = 0; i < this->bulletCount; i++) {
		this->cannons.push_back(new Cannon());
		this->cannons[i]->load("models//BulletVer.obj");
	}
}

void CannonMgr::fireCannon(Matrix& mat)
{
	Matrix trans;
	trans.translation(Vector(1,0.55,0));
	
	this->cannons[this->idx]->startPosition = mat.translation();
	this->cannons[this->idx]->cannonMatrix = mat * trans;
	
	this->cannons[this->idx]->doDraw = true;
	this->cannons[this->idx++]->updateObb();
	if (this->idx >= this->bulletCount) {
		this->idx = 0;
	}
}

void CannonMgr::update(float deltaTime)
{
	for (Cannon* cannon: this->cannons) {
		if (cannon->doDraw) {
			if (cannon->calcDistance()>MAXDISTANCE) {
				cannon->doDraw = false;
			}
			else {
				cannon->update(deltaTime);
			}
		}
	}
}

void CannonMgr::reset()
{
	for (Cannon* cannon : this->cannons) 
	{
		if (cannon->doDraw)
		{
			cannon->doDraw = false;
		}
	}

	this->setIdX(0);
}


CannonMgr::CannonMgr()
{
}


CannonMgr::~CannonMgr()
{
	if (this->cannons.size() > 0) {
		for (Cannon* cannon : this->cannons) {
			delete(cannon);
		}
	}
}
