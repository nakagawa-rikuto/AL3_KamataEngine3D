#include "CollisionManager.h"

/* //////////////////////////////////////////////////
                       リセット
*/ //////////////////////////////////////////////////
void CollisionManager::Reset() {
	// リストを空っぽにする
	colliders_.clear();
}

/* //////////////////////////////////////////////////
                        登録
*/ //////////////////////////////////////////////////
void CollisionManager::Entry(Collider* colliders) {
	// リストに登録
	colliders_.push_back(colliders);
}

/* //////////////////////////////////////////////////
              コライダー２つの衝突判定と応答
*/ //////////////////////////////////////////////////
void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	// コライダーAの座標を取得
	Vector3 positionA = colliderA->GetWorldPosition();

	// コライダーBの座標を取得
	Vector3 positionB = colliderB->GetWorldPosition();

	// 衝突フィルタリング
	if ((colliderA->GetAttribute() != colliderB->GetMask()) || (colliderB->GetAttribute() != colliderA->GetMask())) {
		return;
	}

	// 座標の差分ベクトル
	Vector3 subtract = positionB - positionA;

	// 座標Aと座標Bの距離を求める
	float distance = Length(subtract);

	// 球と球の交差判定
	if (distance < colliderA->GetRadius() + colliderB->GetRadius()) {

		// コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();

		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}

/* //////////////////////////////////////////////////
                 全ての当たり判定チェック
*/ //////////////////////////////////////////////////
void CollisionManager::CheckAllCollisions() {
	// リスト内部のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得
		Collider* colliderA = *itrA;

		// イテレータBはイテレータAの次の要素から回す（重複判定を回避）
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得
			Collider* colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}
}

	/* //////////////////////////////////
	            基本的な関数
	*/ //////////////////////////////////
	void CollisionManager::Initialize() {}

	void CollisionManager::Update() {}

	void CollisionManager::Draw() {}
