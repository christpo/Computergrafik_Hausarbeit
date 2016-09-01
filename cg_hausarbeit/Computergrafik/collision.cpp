#include "collision.h"
#include <float.h>


Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::satCollision(const OrientedBoundingBox & box1, const OrientedBoundingBox & box2)
{
	Vector normals[4];

	normals[0] = box1.downLeft - box1.downRight;
	normals[1] = box1.downRight - box1.upRight;
	normals[2] = box2.downLeft - box2.downRight;
	normals[3] = box2.downRight - box2.upRight;

	normals[0].X = -normals[0].X;
	normals[1].X = -normals[1].X;
	normals[2].X = -normals[2].X;
	normals[3].X = -normals[3].X;

	for (int i = 0; i < 2; i++) {
		
		float box1min, box1max, box2min, box2max;
		calcMinMaxVal(normals[i], box1, box1min, box1max);
		calcMinMaxVal(normals[i], box2, box2min, box2max);
		if (!overlaps(box1min, box1max, box2min, box2max)) {
			return false;
		}
	}
	for (int k = 2; k < 4; k++) {

		float box1min, box1max, box2min, box2max;
		calcMinMaxVal(normals[k], box1, box1min, box1max);
		calcMinMaxVal(normals[k], box2, box2min, box2max);
		if (!overlaps(box1min, box1max, box2min, box2max)) {
			return false;
		}
	}

	return true;
}

vector<Vector> Collision::calcNormales(const OrientedBoundingBox & box)
{
	vector<Vector>normals;

	return normals;
}

void Collision::calcMinMaxVal(const Vector & axis, const OrientedBoundingBox & box, float & min, float & max)
{
	min = FLT_MAX;
	max = FLT_MIN;
	
	vector<Vector> points;
	points.push_back(box.downLeft);
	points.push_back(box.downRight);
	points.push_back(box.upLeft);
	points.push_back(box.upRight);

	for (int i = 0; i < points.size(); i++) {
		
		float dot = points[i].dot(axis);
		if (dot < min) {
			min = dot;
		}
		if (dot > max) {
			max = dot;
		}
	}
}

bool Collision::overlaps(float min1, float max1, float min2, float max2)
{
	return isBetween(min1, min2, max2)|| isBetween(min2, min1, max1);
}

bool Collision::isBetween(float val, float min, float max)
{
	return min <= val && val <= max;
}
