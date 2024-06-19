#include "Vector4.h"
Vector4 Vector4::operator+=(const Vector4& obj) {

	this->x += obj.x;
	this->y += obj.y;
	this->z += obj.z;
	this->w += obj.w;

	return Vector4{this->x, this->y, this->z, this->w};
}

Vector4 const Vector4::operator+(const Vector4& obj) const {

	Vector4 sc = {};
	sc.x = this->x + obj.x;
	sc.y = this->y + obj.y;
	sc.z = this->z + obj.z;
	sc.w = this->w + obj.w;

	return sc;
}

Vector4 Vector4::operator-=(const Vector4& obj) {

	this->x -= obj.x;
	this->y -= obj.y;
	this->z -= obj.z;
	this->w -= obj.w;

	return Vector4{this->x, this->y, this->z, this->w};
}

Vector4 const Vector4::operator-(const Vector4& obj) const {

	Vector4 sc = {};
	sc.x = this->x - obj.x;
	sc.y = this->y - obj.y;
	sc.z = this->z - obj.z;
	sc.w = this->w - obj.w;

	return sc;
}

Vector4 Vector4::operator*=(const float& obj) {

	this->x *= obj;
	this->y *= obj;
	this->z *= obj;
	this->w *= obj;

	return Vector4{this->x, this->y, this->z, this->w};
}

Vector4 const Vector4::operator*(const float& obj) const {

	Vector4 sc = {};
	sc.x = this->x * obj;
	sc.y = this->y * obj;
	sc.z = this->z * obj;
	sc.w = this->w * obj;

	return sc;
}