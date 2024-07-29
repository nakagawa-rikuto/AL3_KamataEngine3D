#include "CollisionManager.h"
#include "GlobalVariables.h"

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
	for (Collider* collider : colliders_) {

		collider->Initialize();
	}
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
	if (distance < colliderA->GetRadius() + colliderB->GetRadius()) {

		// コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision(colliderB);

		// コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision(colliderA);
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

// 初期化
void CollisionManager::Initialize() {
	// モデルの読み込み
	debugModel_.reset(Model::CreateFromOBJ("ColliderModel", true));

	// フラグの初期化
	isDisPlaye_ = true;

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Collider";

	// グループの追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "IsDisPlay", isDisPlaye_);
}

// 更新
void CollisionManager::UpdateWorldTransform() {

	// 非表示なら抜ける
	if (!isDisPlaye_) {

		return;
	}

	// 全てのコライダーについて
	for (Collider* collider : colliders_) {

		// 更新
		collider->UpdateWorldTransform();
	}

	ApplyGlobaVariables();
}

// 描画
void CollisionManager::Draw(const ViewProjection& viewProjection) {

	// 非表示なら抜ける
	if (!isDisPlaye_) {

		return;
	}

	// 全てのコライダーについて
	for (Collider* collider : colliders_) {

		// 更新
		collider->Draw(debugModel_.get(), viewProjection);
	}
}

void CollisionManager::ApplyGlobaVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Collider";
	isDisPlaye_ = globalVariables->GetBoolValue(groupName, "IsDisPlay");
}
