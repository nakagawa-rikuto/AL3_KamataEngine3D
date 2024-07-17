#pragma once
#include "MyMath.h"
#include "imgui.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public:
	/// <summary>
	/// 半径の取得
	/// </summary>
	/// <returns></returns>
	float GetRadius() const;

	/// <summary>
	/// 半径を設定
	/// </summary>
	/// <param name="radius"></param>
	void SetRadius(float& radius);

public: /* 仮想関数 */
	virtual Vector3 GetCenterPosition() const = 0;

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision() {}

private: /* メンバ変数 */
	// 衝突半径
	float radius_ = 1.5f;

private: /* ポリモーフィズム */
	//virtual ~Collider() = default;
};
