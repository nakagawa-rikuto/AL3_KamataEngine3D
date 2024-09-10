#include "Vector3.h"

// ベクトル同士の加法
Vector3 Vector3::operator+(const Vector3& v) { return Vector3(x + v.x, y + v.y, z + v.z); }

// ベクトル同士の加法（+=）
Vector3& Vector3::operator+=(const Vector3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

// ベクトルと float の加法
Vector3 Vector3::operator+(const float& v) { return Vector3(x + v, y + v, z + v); }

// ベクトルと float の加法（+=）
Vector3& Vector3::operator+=(const float& v) {
	x += v;
	y += v;
	z += v;
	return *this;
}

// ベクトル同士の減法
Vector3 Vector3::operator-(const Vector3& v) { return Vector3(x - v.x, y - v.y, z - v.z); }

// ベクトル同士の減法（-=）
Vector3& Vector3::operator-=(const Vector3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

// ベクトルと float の減法
Vector3 Vector3::operator-(const float& v) { return Vector3(x - v, y - v, z - v); }

// ベクトルと float の減法（-=）
Vector3& Vector3::operator-=(const float& v) {
	x -= v;
	y -= v;
	z -= v;
	return *this;
}

// ベクトル同士の積（要素ごとの積）
Vector3 Vector3::operator*(const Vector3& v) { return Vector3(x * v.x, y * v.y, z * v.z); }

// ベクトル同士の積（*=）
Vector3& Vector3::operator*=(const Vector3& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

// ベクトルと float の積
Vector3 Vector3::operator*(const float& v) { return Vector3(x * v, y * v, z * v); }

// ベクトルと float の積（*=）
Vector3& Vector3::operator*=(const float& v) {
	x *= v;
	y *= v;
	z *= v;
	return *this;
}

// ベクトル同士の除法（要素ごとの除法）
Vector3 Vector3::operator/(const Vector3& v) { return Vector3(x / v.x, y / v.y, z / v.z); }

// ベクトル同士の除法（/=）
Vector3& Vector3::operator/=(const Vector3& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

// ベクトルと float の除法
Vector3 Vector3::operator/(const float& v) { return Vector3(x / v, y / v, z / v); }

// ベクトルと float の除法（/=）
Vector3& Vector3::operator/=(const float& v) {
	x /= v;
	y /= v;
	z /= v;
	return *this;
}