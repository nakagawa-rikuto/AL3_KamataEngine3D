#pragma once
#include <list>

#include "Collider.h"

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {

private:
	// コライダー
	std::list<Collider*> colliders_;

public:

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 登録
	/// </summary>
	void AddCollider(Collider* colliders);

	/// <summary>
	/// 2つのコライダーの衝突判定と応答
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 全ての当たり判定チェック
	/// </summary>
	void CheckAllCollisiions();
};
