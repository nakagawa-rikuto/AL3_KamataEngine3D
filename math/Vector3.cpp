#include "Vector3.h"

Vector3 Vector3::operator+=(Vector3& obj) { 
	
	Vector3 sc = {};
	sc.x += obj.x;
	sc.y += obj.y;
	sc.z += obj.z;

	return sc;
}

Vector3 Vector3::operator-=(Vector3& obj) { 

	Vector3 sc = {};
	sc.x -= obj.x;
	sc.y -= obj.y;
	sc.z -= obj.z;

	return sc;
}
