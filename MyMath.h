#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

/// <summary>
/// 2次元ベクトル
/// </summary>
struct Vector2 final {
	float x;
	float y;

	Vector2 operator+=(Vector2& obj);
	Vector2 const operator+(const Vector2& obj) const;
	Vector2 operator-=(Vector2& obj);
	Vector2 const operator-(const Vector2& obj) const;
	Vector2 operator*=(float& obj);
	Vector2 const operator*(const float& obj) const;
};

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
	Vector3 const operator*(const float& obj) const;
};

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

/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

// π
float pi();

#pragma region /// 座標変換 ///
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
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 単位行列の作成
Matrix4x4 MakeIdenitiy4x4();

// 転置行列
Matrix4x4 TransposeMatrix(const Matrix4x4& m);

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

// 透視影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// 正射影行列
Matrix4x4 MakeOrethographicMatrx(float left, float top, float right, float bottom, float nearClip, float farClip);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// 座標変換
Vector3 Transform(Vector3 vector, Matrix4x4 matrix);
#pragma endregion

#pragma region /// 計算 ///
// 内積の計算
float Dot(const Vector3& v1, const Vector3& v2);

// 長さの計算
float Length(const Vector3& v);
float Length(const Vector3& v1, const Vector3& v2);

// 正規化
Vector3 Normalize(const Vector3& v);
#pragma endregion
