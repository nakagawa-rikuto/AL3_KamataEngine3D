#include "Vector3.h"

Vector3 Vector3::operator+=(Vector3& obj) { 
	
	this->x += obj.x;
	this->y += obj.y;
	this->z += obj.z;

	return Vector3{this->x, this->y, this->z};
}

Vector3 const Vector3::operator+(const Vector3& obj) const{ 
	
	Vector3 sc = {};
	sc.x = this->x + obj.x;
	sc.y = this->y + obj.y;
	sc.z = this->z + obj.z;

	return sc;
}

Vector3 Vector3::operator-=(Vector3& obj) { 

	Vector3 sc = {};
	sc.x = this->x - obj.x;
	sc.y = this->y - obj.y;
	sc.z = this->z - obj.z;

	return sc;
}

Vector3 const Vector3::operator-(const Vector3& obj) const{ 

	Vector3 sc = {};
	sc.x = this->x - obj.x;
	sc.y = this->y - obj.y;
	sc.z = this->z - obj.z;

	return sc;
}

Vector3 Vector3::operator*=(float& obj) { 
	Vector3 sc = {};
	sc.x = this->x * obj;
	sc.y = this->y * obj;
	sc.z = this->z * obj;

	return sc;
}

Vector3 const Vector3::operator*(const float& obj) const{ 
	Vector3 sc = {};
	sc.x = this->x * obj;
	sc.y = this->y * obj;
	sc.z = this->z * obj;

	return sc;
}

Vector3 Vector3::operator*=(Vector3& obj) { 

	Vector3 sc = {};
	sc.x = this->x * obj.x;
	sc.y = this->y * obj.y;
	sc.z = this->z * obj.z;

	return sc;
}
