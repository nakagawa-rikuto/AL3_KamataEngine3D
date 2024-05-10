#pragma once
#include <stdio.h>

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3 operator+=(Vector3& obj);
	Vector3 operator-=(Vector3& obj);
};