#include "Enemy.h"

// 移動処理
void Enemy::Move() {

	const float kEnemySpeed = 0.5f;     // 円軌道上の速度
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
	worldTransform_.translation_ = Vector3(5.0f, 2.0f, 10.0f);
	worldTransformBody_.Initialize();
	worldTransformWeapon_.Initialize();

	// 親子関係の構築
	worldTransformBody_.SetParent(&worldTransform_);
	worldTransformWeapon_.SetParent(&worldTransformBody_);
}

// 更新
void Enemy::Update() {

	Move();

	Animation();

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

// アニメーション
void Enemy::Animation() {

	// 浮遊移動のサイクル<frame>
	const uint16_t period = 180;

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * pi() / period;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;

	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * pi());

	// 浮遊の振幅<m>
	float amplitude = 2.0f;

	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;

	worldTransformWeapon_.rotation_.x += 0.5f;
}
