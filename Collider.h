#pragma once
#include "MyMath.h"
#include "CollisionConfig.h"

/// <summary>
/// 衝突判定オブジェクト
/// </summary>
class Collider {
public: /* Getter */
	/// <summary>
	/// Getter
	/// </summary>
	/// <returns></returns>
	float GetRadius() { return radius_; }

	/// <summary>
	/// Getter
	/// </summary>
	/// <returns></returns>
	uint32_t GetAttribute() { return collisionAttribute_; }

	/// <summary>
	/// Getter
	/// </summary>
	/// <returns></returns>
	uint32_t GetMask() { return CollisionMask_; }

public: /* Setter */

	/// <summary>
	/// Setter
	/// </summary>
	/// <param name="radius"></param>
	void SetRadius(float& radius) { radius_ = radius; }

	/// <summary>
	/// Setter
	/// </summary>
	/// <param name="collisionAttribute"></param>
	void SetAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	/// <summary>
	/// Setter
	/// </summary>
	/// <param name="collisionMask"></param>
	void SetMask(uint32_t collisionMask) { CollisionMask_ = collisionMask; }

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

	// 衝突属性（自分）
	uint32_t collisionAttribute_ = 0xffffffff;

	// 衝突マスク（相手）
	uint32_t CollisionMask_ = 0xffffffff;
};
