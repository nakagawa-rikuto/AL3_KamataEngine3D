#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+=(Vector3& obj);
	Vector3 const operator+(const Vector3& obj) const;
	Vector3 operator-=(Vector3& obj);
	Vector3 const operator-(const Vector3& obj) const;
	Vector3 operator*=(float& obj);
	Vector3 operator*=(Vector3& obj);
	Vector3 const operator*(const float& obj) const;
	Vector3 const operator*(const Vector3& obj) const;
	Vector3 operator/=(float& obj);
	Vector3 operator/=(Vector3& obj);
	Vector3 const operator/(const float& obj) const;
	Vector3 const operator/(const Vector3& obj) const;
};