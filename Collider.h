#pragma once
#include "MyMath.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public: /* Getter・Setter */

	/// <summary>
	/// Getter
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// Setter
	/// </summary>
	/// <param name="radius"></param>
	void SetRadius(float& radius) { radius_ = radius; }

public: /* 仮想関数 */

	/// <summary>
	/// ワールド座標を取得
	/// </summary>
	/// <returns></returns>
	virtual Vector3 GetWorldPosition() const = 0;

	/// <summary>
	/// 衝突時に呼ばれる関数
	/// </summary>
	virtual void OnCollision() = 0;

private:
	// 衝突半径
	float radius_ = 1.0f;
};
