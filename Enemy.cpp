#define NOMINMAX
#include "Enemy.h"
#include <DebugText.h>
#include <algorithm>
#include <numbers>

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
	param_ = std::sin(2 * pi() * walkTimer_ / kWalkMotionTime_);
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

void Enemy::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}
