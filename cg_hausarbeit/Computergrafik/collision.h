#pragma once
#include "Model.h"
#include "vector.h"
#include <vector>

class Collision
{
public:
	Collision();
	~Collision();

	bool satCollision(const OrientedBoundingBox& box1, const OrientedBoundingBox& box2);
	vector<Vector> calcNormales(const OrientedBoundingBox& box);
	void calcMinMaxVal(const Vector& axis, const OrientedBoundingBox& box1, float& min, float& max);
	bool overlaps(float min1, float max1, float min2, float max2);
	bool isBetween(float val, float min, float max);
};

