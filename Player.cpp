#include "Player.h"
#include <numbers>

void Player::Initialeze(Model* model, uint32_t texturehandle, ViewProjection* viewProjection, const Vector3& pos) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = texturehandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;

	/// ***********************
	/// 初期回転
	/// ***********************
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	/// *********************
	/// 速度
	/// *********************
	// 移動入力

	// 移動
	worldTransform_.translation_ += velocity_;

	// 行列計算
	worldTransform_.UpdateMatrix();

	// 行列を更新して定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}
