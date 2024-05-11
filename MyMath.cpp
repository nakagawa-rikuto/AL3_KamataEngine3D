#include "MyMath.h"

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 translateMatrix = {
	    {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {translate.x, translate.y, translate.z, 1}}
    };

	return translateMatrix;
}

Matrix4x4 MakeScalseMatrix(const Vector3& scale) {

	Matrix4x4 scaleMatrix = {
	    {{scale.x, 0, 0, 0}, {0, scale.y, 0, 0}, {0, 0, scale.z, 0}, {0, 0, 0, 1}}
    };

	return scaleMatrix;
}

Matrix4x4 MakeRotateXMatrix(float radian) {

	/*外側の中かっこは、Matrix4x4構造体の初期化を表しており、
	  内側の中かっこは配列の初期化を表しています。*/
	Matrix4x4 result = {
	    {{1, 0, 0, 0}, {0, cos(radian), sin(radian), 0}, {0, -sin(radian), cos(radian), 0}, {0, 0, 0, 1}}
    };

	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian) {

	Matrix4x4 result = {
	    {{cos(radian), 0, -sin(radian), 0}, {0, 1, 0, 0}, {sin(radian), 0, cos(radian), 0}, {0, 0, 0, 1}}
    };

	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {

	Matrix4x4 result = {
	    {{cos(radian), sin(radian), 0, 0}, {-sin(radian), cos(radian), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}
    };

	return result;
}

Matrix4x4 Mutiply(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 answer = {};
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {

			answer.m[x][y] = 0;
			for (int z = 0; z < 4; ++z) {

				answer.m[x][y] += m1.m[x][z] * m2.m[z][y];
			}
		}
	}

	return answer;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	// 平行移動(T)
	Matrix4x4 translateMatrix_ = MakeTranslateMatrix(translate);

	// 拡縮(S)
	Matrix4x4 scaleMatrix_ = MakeScalseMatrix(scale);

	// 回転(R)
	Matrix4x4 roteMatrixX_ = MakeRotateXMatrix(rotate.x);
	Matrix4x4 roteMatrixY_ = MakeRotateYMatrix(rotate.y);
	Matrix4x4 roteMatrixZ_ = MakeRotateZMatrix(rotate.z);

	Matrix4x4 roteMatrixXYZ_ = Mutiply(roteMatrixX_, Mutiply(roteMatrixY_, roteMatrixZ_));

	// 行列の積の結合法則(W = SRT)
	Matrix4x4 affineMatrix_ = Mutiply(scaleMatrix_, roteMatrixXYZ_);
	affineMatrix_ = Mutiply(affineMatrix_, translateMatrix_);

	return affineMatrix_;
}

// EaseOut関数
float EaseOut(float t) { return 1.0f - (1.0f - t) * (1.0f - t); }

// EaseIn関数
float EaseIn(float t) { return t * t; }

// 線形補間関数
Vector3 Leap(const Vector3& start, const Vector3& end, float t) {

	// 補間パラメータが0ならばStartを返す
	if (t <= 0) {
		return start;
	}

	// 補間パラメータが1ならばEndを返す
	else if (t >= 1) {
		return end;
	}

	// startからEndまでの線形補間を計算して返す
	else {
		return Vector3(start.x + (end.x - start.x) * t, start.y + (end.y - start.y) * t, start.z + (end.z - start.z) * t);
	}
}
