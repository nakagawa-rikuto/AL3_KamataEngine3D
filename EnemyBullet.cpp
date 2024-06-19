#define NOMINMAX
#include "EnemyBullet.h"
#include "TextureManager.h"

EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);

	velocity_ = velocity;

	model_ = model;

	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("./Resources/uvChecker.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 初期座標をセット
	worldTransform_.translation_ = position;

	// Z軸方向に伸びた形状
	worldTransform_.scale_.x = 0.5f;
	worldTransform_.scale_.y = 0.5f;
	worldTransform_.scale_.z = 3.0f;

	// Y軸周りの角度(θ)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	Vector3 velocityZ_ = TransformNormal(velocity_, MakeRotateYMatrix(-worldTransform_.rotation_.y));

	// X軸周りの角度(θ)
	worldTransform_.rotation_.x = std::atan2(velocityZ_.y, velocityZ_.z);
}

void EnemyBullet::Update() {

	worldTransform_.UpdateMatrix();

	// 座標を移動させる
	worldTransform_.translation_ += velocity_;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }
