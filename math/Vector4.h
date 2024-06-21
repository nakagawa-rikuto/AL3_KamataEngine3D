#pragma once

/// <summary>
/// 4次元ベクトル
/// </summary>
struct Vector4 final {
	float x;
	float y;
	float z;
	float w;

	Vector4 operator+=(Vector4& obj);
	Vector4 const operator+(const Vector4& obj) const;
	Vector4 operator-=(Vector4& obj);
	Vector4 const operator-(const Vector4& obj) const;
	Vector4 operator*=(float& obj);
	Vector4 const operator*(const float& obj) const;
};