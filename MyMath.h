#pragma once
#define _USE_MATH_DEFINES
#include "math.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

// AABB
struct AABB {
	Vector3 min; //!< 最小点
	Vector3 max; //!< 最大点
};

// π
float pi();

// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 拡縮行列
Matrix4x4 MakeScalseMatrix(const Vector3& scale);

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 行列同士の掛け算
Matrix4x4 Mutiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 座標変換
Vector3 Transform(Vector3 vector, Matrix4x4 matrix);

// EaseOut関数
float EaseOut(float t);

// EaseIn関数
float EaseIn(float t);

// 線形補間関数 Leap
Vector3 Leap(const Vector3& start, const Vector3& end, float t);

// 内積の計算
float Dot(const Vector3& v1, const Vector3& v2);

#pragma region 衝突判定

// AABBとAABBの当たり判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

#pragma endregion 