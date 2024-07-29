#define NOMINMAX
#include "Enemy.h"
#include <DebugText.h>
#include <algorithm>
#include <numbers>

AABB Enemy::GetAABB() { 

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth_ / 2.0f, worldPos.y - kHeight_ / 2.0f, worldPos.z - kWidth_ / 2.0f};
	aabb.max = {worldPos.x + kWidth_ / 2.0f, worldPos.y + kHeight_ / 2.0f, worldPos.z + kWidth_ / 2.0f};

	return aabb;
}

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;

	worldTransform_.rotation_.y =  std::numbers::pi_v<float> / 2.0f;

	// 速度の設定
	velocity_ = {-kWalkSpeed_, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() {

	/*///////////////////////////////////////////
	        　　     簡単な移動s
	*////////////////////////////////////////////
	// 移動
	worldTransform_.translation_ += velocity_;

	/*///////////////////////////////////////////
	        　　     アニメーション
	*////////////////////////////////////////////
	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	// 回転アニメーション
	param_ = std::sin((2 * pi()) * (walkTimer_ / kWalkMotionTime_));
	radian_ = (kWalkMotionAngleStart_ + kWalkMotionAngleEnd_) * (param_ + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = radian_ * pi() / 180.0f;

	/*///////////////////////////////////////////
	        　　     行列の計算
	*////////////////////////////////////////////
	// 行列計算
	worldTransform_.UpdateMatrix();

	// 行列を更新して定数バッファに転送
	worldTransform_.UpdateMatrix();
}

// 描画
void Enemy::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}

// 衝突判定
void Enemy::OnCollision(const Player* player) {

	(void)player; 
}

// ワールド座標を取得
Vector3 Enemy::GetWorldPosition() { 

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
