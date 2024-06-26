#include "Enemy.h"

// 移動処理
void Enemy::Move() {

	const float kEnemySpeed = 1.0f;     // 円軌道上の速度
	//const float kRotationSpeed = 0.05f; // 回転速度（ラジアン毎フレーム）
	Vector3 velocity(0, 0, kEnemySpeed);

	worldTransform_.rotation_.y += 0.1f;

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	worldTransform_.translation_ += velocity;
}

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

	Move();

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
