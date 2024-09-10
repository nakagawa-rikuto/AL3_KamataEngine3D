#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	/// ===加法=== ///
	Vector3 operator+(const Vector3& v);
	Vector3& operator+=(const Vector3& v);
	Vector3 operator+(const float& v);
	Vector3& operator+=(const float& v);

	/// ===減法=== ///
	Vector3 operator-(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3 operator-(const float& v);
	Vector3& operator-=(const float& v);

	/// ===積=== ///
	Vector3 operator*(const Vector3& v);
	Vector3& operator*=(const Vector3& v);
	Vector3 operator*(const float& v);
	Vector3& operator*=(const float& v);

	/// ===除法=== ///
	Vector3 operator/(const Vector3& v);
	Vector3& operator/=(const Vector3& v);
	Vector3 operator/(const float& v);
	Vector3& operator/=(const float& v);
};