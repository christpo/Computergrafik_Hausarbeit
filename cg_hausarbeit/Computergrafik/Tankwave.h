#pragma once
#include "Tankmanager.h"
#include <iomanip>

class Tankwave
{
public:
	Tankwave();
	~Tankwave();
	void setWaveCount(int count);
	int getWaveCount();
	void setMaxWaveCount(int count);
	int getMaxWaveCount();
	void setCount(float count);
	float getCount();
	void setPause(float pause);
	float getPause();

	void startWave(float deltatime, float waveLength, Tank& tank, int numberOfWafes);
	void load();

	Tankmanager getTankManager();

private:
	float pause = 5.0f;
	float count = 0.0f;
	int numberOfWafes = 1;
	int maxWafeCount;
	Tankmanager manager;
};

