#include "Collider.h"
#include "CollisionTypeIdDef.h"

/* ////////////////////////////////////////////////////
                        Getter
*/ ////////////////////////////////////////////////////
// 半径の取得
float Collider::GetRadius() const { return radius_; }

// 種別IDを取得
uint32_t Collider::GetTypeID() const { return typeID_; }

/* ////////////////////////////////////////////////////
                        Setter
*/ ////////////////////////////////////////////////////
// 半径を設定
void Collider::SetRadius(float& radius) { radius_ = radius; }

// 種別IDを取得
void Collider::SetTypeID(uint32_t typeID) { typeID_ = typeID; }

/* ////////////////////////////////////////////////////
                      基本的な関数
*/ ////////////////////////////////////////////////////
// 初期化
void Collider::Initialize() {
	worldTransform_.Initialize();
	worldTransform_.scale_ = {1.5f, 1.5f, 1.5f};
}

// 描画
void Collider::UpdateWorldTransform() {

	// ワールド座標をワールドトランスフォームに適用
	worldTransform_.translation_ = GetCenterPosition();
	worldTransform_.UpdateMatrix();
}

void Collider::Draw(Model* model, const ViewProjection& viewProjection) {

	// モデルの描画
	model->Draw(worldTransform_, viewProjection);
}

/* ////////////////////////////////////////////////////
                       仮想関数
*/ ////////////////////////////////////////////////////
void Collider::OnCollision(Collider* other) {

	// 衝突相手の種別IDを取得
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kDefault)) {
	}
}
