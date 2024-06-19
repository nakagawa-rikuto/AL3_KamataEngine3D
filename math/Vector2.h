#pragma once

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

	Vector2 operator+=(const Vector2& obj);
	Vector2 const operator+(const Vector2& obj) const;
	Vector2 operator-=(const Vector2& obj);
	Vector2 const operator-(const Vector2& obj) const;
	Vector2 operator*=(const float& obj);
	Vector2 const operator*(const float& obj) const;
};
