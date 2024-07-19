#include "CollisionManager.h"

/// <summary>
/// リセット
/// </summary>
void CollisionManager::Reset() {

	// リストを空っぽにする
	colliders_.clear();
}

/// <summary>
/// 登録
/// </summary>
void CollisionManager::AddCollider(Collider* colliders) { 

	colliders_.push_back(colliders); 
}

/// <summary>
/// 2つのコライダーの衝突判定
/// </summary>
/// <param name="colliderA"></param>
/// <param name="colliderB"></param>
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) { 

	// コライダーAの座標を取得
	Vector3 positionA = colliderA->GetCenterPosition();

	// コライダーBの座標を取得
	Vector3 positionB = colliderB->GetCenterPosition();

	// 座標の差分ベクトル
	Vector3 subtract = positionB - positionA;

	// 座標Aと座標Bの距離を求める
	float distance = Length(subtract);

	// 球と球の交差判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) { // 04_13

		// コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();

		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}

/// <summary>
/// 全ての当たり判定チェック
/// </summary>
void CollisionManager::CheckAllCollisiions() {

	// リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {

		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {

			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}
