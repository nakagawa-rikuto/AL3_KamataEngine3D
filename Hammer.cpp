#include "Hammer.h"

// 　回転の取得
const Vector3& Hammer::GetRotate() const { return worldTransform_.rotation_; }

// 回転のセット
void Hammer::SetRotate(const Vector3& rotation) { worldTransform_.rotation_ = rotation; }

// 中心座標の取得
Vector3 Hammer::GetCenterPosition() const { return Vector3(); }

// 衝突判定
void Hammer::OnCollision() {}

// WorldTransformの初期化
void Hammer::WorldTransformInitialize() { 
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f}; 

	Collider::Initialize();
}

// 初期化
void Hammer::Initialize(Model* model, const WorldTransform* worldTransform) {

	model_ = model;

	worldTransform_.SetParent(worldTransform);
}

// 更新
void Hammer::Update() { 

	worldTransform_.UpdateMatrix(); 
}

// 描画
void Hammer::Draw(const ViewProjection& viewProjection) { 

	model_->Draw(worldTransform_, viewProjection); 
}
