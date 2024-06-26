#include "Enemy.h"

// 初期化
void Enemy::Initialize(const std::vector<Model*>& models) {

	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(5.0f, 0.0f, 10.0f);
	worldTransformBody_.Initialize();
	worldTransformWeapon_.Initialize();

	// 親子関係の構築
	worldTransformBody_.SetParent(&worldTransform_);
	worldTransformWeapon_.SetParent(&worldTransform_);
}

// 更新
void Enemy::Update() {

	// 行列の再計算と転送
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}

// 描画
void Enemy::Draw(const ViewProjection& viewProjection) { 
	// 3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection); 
	models_[kModelIndexWeapon]->Draw(worldTransformWeapon_, viewProjection);
}
