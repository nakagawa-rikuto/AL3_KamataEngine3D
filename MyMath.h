#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

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