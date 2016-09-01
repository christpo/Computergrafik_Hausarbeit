#define _USE_MATH_DEFINES

#include "Tankmanager.h"
#include <math.h>


Tankmanager::Tankmanager()
{
}


Tankmanager::~Tankmanager()
{
}

void Tankmanager::setEnemyCount(int count)
{
	this->enemyCount = count;
}

int Tankmanager::getEnemyCount()
{
	return this->enemyCount;
}

void Tankmanager::setMaxRange(int range)
{
	this->maxRange = range;
}

int Tankmanager::getMaxRange()
{
	return this->maxRange;
}

void Tankmanager::setMaxX(int x)
{
	this->maxX = x;
}

int Tankmanager::getMaxX()
{
	return this->maxX;
}

void Tankmanager::setInvert(bool invert)
{
	this->invert = invert;
}

bool Tankmanager::getInvert()
{
	return this->invert;
}

void Tankmanager::setStartPointTank(int point)
{
	this->startPointTank = point;
}

int Tankmanager::getStartPointTank()
{
	return this->startPointTank;
}

bool Tankmanager::seeTheEnemy(const Vector& userPosition, int count)
{
	if ((this->enemys[count]->getPosition() - userPosition).length() >= 0 && (this->enemys[count]->getPosition() - userPosition).length() <= this->getMaxRange())
	{
		return true;
	}

	return false;
}

bool Tankmanager::shootRange(const Vector& userPosition, int count)
{
	if ((this->enemys[count]->getPosition() - userPosition).length() >= 0 && (this->enemys[count]->getPosition() - userPosition).length() <= (this->getMaxRange() / 2))
	{
		return true;
	}

	return false;
}

void Tankmanager::driving(const Vector& userPosition, float Deltatime, int numberOfWafes)
{
	float speed = 0.0f;

	for (unsigned int i = 0; i < this->enemys.size(); i++)
	{
		
		this->enemys[i]->setShootingDelay(this->enemys[i]->getShootingDelay() + Deltatime);
		this->aim(this->enemys[i]->getPosition().X, this->enemys[i]->getPosition().Z - 1,i);

		speed = 0.3f  * (1 + (numberOfWafes / 10));

		this->enemys[i]->steer(1, 0, speed);

		if (this->seeTheEnemy(userPosition, i))
		{
			this->aim(userPosition.X, userPosition.Z, i);

			if (this->enemys[i]->getShootingDelay() >= 4.0f)
			{
				this->enemys[i]->hasNotShoot = true;
				this->enemys[i]->setShootingDelay(0.0f);
			}

			if (this->enemys[i]->hasNotShoot && this->shootRange(userPosition, i))
			{
				this->enemys[i]->cannonMgr.fireCannon(this->enemys[i]->cononMatrix);
				this->enemys[i]->hasNotShoot = false;
			}
		}	
	}
}

bool Tankmanager::load(const char * ChassisModel, const char * CononModel, const Vector & StartPos, int count)
{
	this->setEnemyCount(count);
	this->setStartPointTank(StartPos.Z);
	this->randomSpeedInitialized();
	

	srand(GetTickCount());

	for (unsigned int i = 0; i < this->enemyCount; i++)
	{
		Tank* enemy = new Tank();

		this->enemys.push_back(enemy);

		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		if ((rand() % 40 + 1) <= 5)
		{
			this->enemys[i]->isFaster = true;
		}
		this->enemys[i]->setIsActive(false);
		this->enemys[i]->load(ChassisModel, CononModel, Vector(float(this->giveRandomXPosition()), StartPos.Y, StartPos.Z * (((rand() % 3) + 1)) * (1 + r)), 5);
	}

	return true;
}

void Tankmanager::aim(int X, int Z, int i)
{
	this->enemys[i]->aimEnemy(X, Z);
	
}

void Tankmanager::ausrichten()
{
	Matrix rotation;
	rotation.rotationY(float(M_PI / 2));

	for (unsigned int i = 0; i < this->getEnemyCount(); i++)
	{
		this->enemys[i]->cassisMatrix *= rotation;
	}
}

void Tankmanager::randomSpeedInitialized()
{
	int c, n;

	srand(GetTickCount());

	for (c = 1; c <= this->getEnemyCount(); c++)
	{
		n = (rand() % 100) + 1;


		this->randomSpeed.push_back(n);
	}
}

void Tankmanager::update(float deltatime)
{
	for (unsigned int i = 0; i < this->getEnemyCount(); i++)
	{
		if (this->enemys[i]->getIsActive()) {
			this->enemys[i]->update(deltatime);
		}		
	}
}

void Tankmanager::startReset()
{
	srand(GetTickCount());

	for (unsigned int i = 0; i < this->getEnemyCount(); i++)
	{
		float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		this->enemys[i]->setZPosition((rand() % 18) -9, this->getStartPointTank() * (((rand() % 3) + 1)) * (1 + r));
		this->enemys[i]->cannonMgr.reset();
		this->enemys[i]->setIsActive(true);
		this->enemys[i]->isFaster = false;

		if ((rand() % 40 + 1) <= 5)
		{
			this->enemys[i]->isFaster = true;
		}
	}
}

bool Tankmanager::allEnemysHit()
{
	int sum = 0;

	for (unsigned int i = 0; i < this->getEnemyCount(); i++)
	{
		if (this->enemys[i]->getIsActive() == false)
		{
			sum += 1;
		}
	}

	if (sum == this->getEnemyCount())
	{
		return true;
	}

	return false;
}

int Tankmanager::giveRandomXPosition()
{
	int xPosition;

	if (this->xPositions.size() >= 1)
	{
		xPosition = this->xPositions[0];

		this->xPositions.erase(this->xPositions.begin());

		return xPosition;
	}
	else
	{
		xPosition = 0;

		if (this->getInvert())
		{
			for (unsigned int i = 1; i <= this->getMaxX(); i++)
			{
				this->xPositions.push_back(pow(-1, i) * i);
				this->xPositions.push_back(pow(-1, i + 1) * i);
			}

			this->setInvert(false);
		}
		else
		{
			for (unsigned int i = 1; i <= this->getMaxX(); i++)
			{
				this->xPositions.push_back(pow(-1, i + 1) * i);
				this->xPositions.push_back(pow(-1, i) * i);
			}
		}
	}
	
	return xPosition;
}

vector<Tank*> Tankmanager::getEnemys()
{
	return this->enemys;
}


