#define NOMINMAX
#include "EnemyBullet.h"
#include "TextureManager.h"
#include "Player.h"

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

	ImGui::DragFloat3("WorldTransform", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat("Speed", &speed, 0.1f);

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//
	worldTransform_.translation_ += velocity_;

	// 敵弾から自キャラのベクトルを計算
	Vector3 toPlayer = player_->GetWorldPosition() - worldTransform_.translation_;

	// ベクトルを正規化する
	toPlayer = Normalize(toPlayer);
	velocity_ = Normalize(velocity_);

	// 球面線形補間により、今の速度と自キャラのベクトルを新たな速度とする。
	velocity_ = Slerp(velocity_, toPlayer, t);

	velocity_ *= speed;

	// 進行方向に見た目の回転を合わせる
	// Y軸周りの角度(θ)
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

	// 横軸方向の長さを求める
	Vector3 velocityZ_ = TransformNormal(velocity_, MakeRotateYMatrix(-worldTransform_.rotation_.y));

	// X軸周りの角度(θ)
	worldTransform_.rotation_.x = std::atan2(velocityZ_.y, velocityZ_.z);

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }

