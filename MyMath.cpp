#include "MyMath.h"

Matrix4x4 MyMath::MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 translateMatrix = {
	    {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {translate.x, translate.y, translate.z, 1}}
    };

	return translateMatrix;
}

Matrix4x4 MyMath::MakeScalseMatrix(const Vector3& scale) {

	Matrix4x4 scaleMatrix = {
	    {{scale.x, 0, 0, 0}, {0, scale.y, 0, 0}, {0, 0, scale.z, 0}, {0, 0, 0, 1}}
    };

	return scaleMatrix;
}

Matrix4x4 MyMath::MakeRotateXMatrix(float radian) {

	/*外側の中かっこは、Matrix4x4構造体の初期化を表しており、
	  内側の中かっこは配列の初期化を表しています。*/
	Matrix4x4 result = {
	    {{1, 0, 0, 0}, {0, cos(radian), sin(radian), 0}, {0, -sin(radian), cos(radian), 0}, {0, 0, 0, 1}}
    };

	return result;
}

Matrix4x4 MyMath::MakeRotateYMatrix(float radian) {

	Matrix4x4 result = {
	    {{cos(radian), 0, -sin(radian), 0}, {0, 1, 0, 0}, {sin(radian), 0, cos(radian), 0}, {0, 0, 0, 1}}
    };

	return result;
}

Matrix4x4 MyMath::MakeRotateZMatrix(float radian) {

	Matrix4x4 result = {
	    {{cos(radian), sin(radian), 0, 0}, {-sin(radian), cos(radian), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}
    };

	return result;
}

Matrix4x4 MyMath::Mutiply(const Matrix4x4& m1, const Matrix4x4& m2) {

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

Matrix4x4 MyMath::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	// 平行移動(T)
	translateMatrix_ = MakeTranslateMatrix(translate);

	// 拡縮(S)
	scaleMatrix_ = MakeScalseMatrix(scale);

	// 回転(R)
	roteMatrixX_ = MakeRotateXMatrix(rotate.x);
	roteMatrixY_ = MakeRotateYMatrix(rotate.y);
	roteMatrixZ_ = MakeRotateZMatrix(rotate.z);

	roteMatrixXYZ_ = Mutiply(roteMatrixX_, Mutiply(roteMatrixY_, roteMatrixZ_));

	// 行列の積の結合法則(W = SRT)
	affineMatrix_ = Mutiply(scaleMatrix_, roteMatrixXYZ_);
	affineMatrix_ = Mutiply(affineMatrix_, translateMatrix_);

	return affineMatrix_;
}
