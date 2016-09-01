#include "vector.h"
#include <math.h>
#include <stdexcept>

#define EPSILON 1e-5

Vector::Vector()
{
}

Vector::Vector(float x, float y, float z) {
	
	this->X = x;
	this->Y = y;
	this->Z = z;
}

float Vector::dot(const Vector& v) const {
	
	return this->X * v.X + this->Y * v.Y + this->Z * v.Z;
}

Vector Vector::cross(const Vector& v) const {
	return Vector(	this->Y * v.Z - this->Z * v.Y,
					this->Z * v.X - this->X * v.Z,
					this->X * v.Y - this->Y * v.X);
}

Vector Vector::operator+(const Vector& v) const {

	return Vector(	this->X + v.X,
					this->Y + v.Y,
					this->Z + v.Z);
}

Vector Vector::operator-(const Vector& v) const {

	return Vector(	this->X - v.X,
					this->Y - v.Y,
					this->Z - v.Z);
}

Vector& Vector::operator+=(const Vector& v) {

	this->X += v.X;
	this->Y += v.Y;
	this->Z += v.Z;

	return *this;
}

Vector Vector::operator*(float c) const {

	return Vector(	this->X * c,
					this->Y * c,
					this->Z * c);
}

Vector Vector::operator-() const {

	return Vector(-this->X, -this->Y, -this->Z);
}

Vector& Vector::normalize() {

	float len = this->length();
	if (len == 0) {
		throw std::overflow_error("Divide by zero exception");
	}
	else {
		this->X /= len;
		this->Y /= len;
		this->Z /= len;

		return *this;
	}
}

float Vector::length() const {

	return sqrt(this->lengthSquared());
}

float Vector::lengthSquared() const {

	return this->dot(*this);
}

Vector Vector::reflection(const Vector& normal) const {
	
	return *this - (normal * 2 * this->dot(normal));
}

bool Vector::triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const {
	
	Vector n = (b - a).cross(c - a).normalize();

	float ndotd = n.dot(d);

	if (ndotd < EPSILON && ndotd > -EPSILON) {

		return false;
	}
	
	float d1 = n.dot(a);

	s = (d1 - n.dot(*this)) / ndotd;

	if (s < 0) {

		return false;
	}

	Vector p = *this + ((d) * s);

	float abc = (b - a).cross(c - a).length();
	float abp = (a - p).cross(b - p).length();
	float acp = (a - p).cross(c - p).length();
	float bcp = (b - p).cross(c - p).length();

	if (abc+EPSILON >= (abp + acp + bcp)) {

		return true;
	}

	return false;
}

Vector Vector::operator/(float c) const{
	if (c == 0) {
		throw std::overflow_error("Divide by zero exception");
	}
	return Vector(	this->X / c,
					this->Y / c,
					this->Z / c);
}
