#include "Collider.h"

// 半径の取得
float Collider::GetRadius() const { return radius_; }

// 半径を設定
void Collider::SetRadius(float& radius) {

	radius_ = radius; 
}

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
