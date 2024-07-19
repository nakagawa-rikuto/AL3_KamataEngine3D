#pragma once
#include <list>

#include "Collider.h"
#include "Model.h"

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager {

private:
	// コライダー
	std::list<Collider*> colliders_;

	// デバッグ用表示モデル
	std::unique_ptr<Model> debugModel_;

	// コライダーの表示フラグ
	bool isDisPlaye_;

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

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void UpdateWorldTransform();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

private:

	// 調整項目の適用
	void ApplyGlobaVariables();
};
