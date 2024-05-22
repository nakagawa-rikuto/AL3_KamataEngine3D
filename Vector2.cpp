#include "Vector2.h"

Vector2 Vector2::operator+=(Vector2& obj) {

	this->x += obj.x;
	this->y += obj.y;

	return Vector2{this->x, this->y};
}

Vector2 const Vector2::operator+(const Vector2& obj) const {

	Vector2 sc = {};
	sc.x = this->x + obj.x;
	sc.y = this->y + obj.y;

	return sc;
}

Vector2 Vector2::operator-=(Vector2& obj) {

	this->x -= obj.x;
	this->y -= obj.y;

	return Vector2{this->x, this->y};
}

Vector2 const Vector2::operator-(const Vector2& obj) const {

	Vector2 sc = {};
	sc.x = this->x - obj.x;
	sc.y = this->y - obj.y;

	return sc;
}

Vector2 Vector2::operator*=(float& obj) {

	this->x *= obj;
	this->y *= obj;

	return Vector2{this->x, this->y};
}

Vector2 const Vector2::operator*(const float& obj) const {

	Vector2 sc = {};
	sc.x = this->x * obj;
	sc.y = this->y * obj;

	return sc;
}