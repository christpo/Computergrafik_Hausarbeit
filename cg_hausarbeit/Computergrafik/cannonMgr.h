#pragma once

#include <vector>
#include "cannon.h"

using namespace std;

class CannonMgr
{
public:

	vector<Cannon*> cannons;

	int idx = 0;
	int bulletCount;

	void addCannon(int bulletCount);
	void fireCannon(Matrix& mat);
	void update(float deltaTime);
	void reset();
	void setIdX(int idx);

	CannonMgr();
	~CannonMgr();
};

