#pragma once

#include <list>

#include "Collider.h"

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {
public:

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	/// <summary>
	/// 登録
	/// </summary>
	/// <param name="colliders"></param>
	void Entry(Collider* colliders);

	/// <summary>
	/// コライダー２つの衝突判定と応答
	/// </summary>
	/// <param name="colliderA"></param>
	/// <param name="colliderB"></param>
	void CheckCollisionPair(Collider* colliderA, Collider* colliderB);

	/// <summary>
	/// 全ての当たり判定チェック
	/// </summary>
	void CheckAllCollisions();

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// コライダー
	std::list<Collider*> colliders_;
};
