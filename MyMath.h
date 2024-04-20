#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <assert.h>
#include <cmath>

class MyMath {

public:
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

private:
	/// *************************************
	/// メンバ変数
	/// *************************************
	// アフィン(W)
	Matrix4x4 affineMatrix_;

	// 平行移動(S)
	Matrix4x4 translateMatrix_;

	// 拡縮(S)
	Matrix4x4 scaleMatrix_;

	// 回転(R)
	Matrix4x4 roteMatrixX_;
	Matrix4x4 roteMatrixY_;
	Matrix4x4 roteMatrixZ_;
	Matrix4x4 roteMatrixXYZ_;
};
