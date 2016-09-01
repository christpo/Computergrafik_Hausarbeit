#include "Tankwave.h"



Tankwave::Tankwave()
{
}


Tankwave::~Tankwave()
{
}


void Tankwave::setWaveCount(int count)
{
	this->numberOfWafes = count;
}

int Tankwave::getWaveCount()
{
	return this->numberOfWafes;
}

void Tankwave::setMaxWaveCount(int count)
{
	this->maxWafeCount = count;
}

int Tankwave::getMaxWaveCount()
{
	return this->maxWafeCount;;
}

void Tankwave::setCount(float count)
{
	this->count = count;
}

float Tankwave::getCount()
{
	return this->count;
}

void Tankwave::setPause(float pause)
{
	this->pause = pause;
}

float Tankwave::getPause()
{
	return this->pause;
}

void Tankwave::startWave(float deltatime, float waveLength, Tank& tank, int maxNumberOfWafes)
{
	this->setCount(this->getCount() + deltatime);
	this->setMaxWaveCount(maxNumberOfWafes);

	if (this->getCount() >= this->getPause() && this->getWaveCount() <= this->getMaxWaveCount())
	{
		if (this->getCount() <= waveLength)
		{
			this->manager.driving(tank.getPosition(), deltatime, this->getWaveCount());	
			static float temp = 0.0f;

			temp += deltatime;

			if (temp > 1.0f)
			{
				//cout << "Welle endet in " << waveLength - this->getCount()  << " Sekunden " << endl;
				temp = 0.0f;
			}
		}
		if (this->getCount() > waveLength || this->manager.allEnemysHit())
		{
			tank.cannonMgr.reset();
			this->manager.startReset();
			this->setCount(0.0f);
			this->setWaveCount(this->getWaveCount() + 1);
		}
	}
	else if(this->getWaveCount() <= this->getMaxWaveCount())
	{
		cout << fixed;
		cout << setprecision(0);

		static float temp = 0.0f;

		temp += deltatime;

		if (temp > 1.0f)
		{
			cout << "Welle " << this->getWaveCount() << " / " << this->getMaxWaveCount() << " startet in " << this->getPause() - this->count << " Sekunden" << endl;
			temp = 0.0f;
		}	
	}
	else
	{
		//cout << "Das Spiel ist vorbei!" << endl;
		getchar();
	}
	this->manager.update(deltatime);

}

void Tankwave::load()
{
	this->manager.load("prakt4modelle//tank_bottom2.obj", "prakt4modelle//tank_top2.obj", Vector(2, 0, 40), 30);
	this->manager.ausrichten();
}

Tankmanager Tankwave::getTankManager()
{
	return this->manager;
}
	
