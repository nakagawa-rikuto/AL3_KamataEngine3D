#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインターチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	// シングルトンインスタンス
	worldTransform_.Initialize();
	worldTransform_.translation_.z = 5.0f;

	//初期フェーズの設定
	phase_ = Phase::APPROACH;
}

void Enemy::Update() {

	switch (phase_) {
	case Enemy::Phase::APPROACH:

		// 移動(ベクトルの減算)
		worldTransform_.translation_.x += 0.1f;
		worldTransform_.translation_.z -= 0.3f;

		// 規定の位置に到達したら離脱に切り替え
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::LEACE;
		}
		break;
	case Enemy::Phase::LEACE:

		// 移動(ベクトルを加算)
		worldTransform_.translation_.x -= 0.1f;
		worldTransform_.translation_.z += 0.3f;

		// 規定の距離に到達したら接近に切り替え
		if (worldTransform_.translation_.z > 10.0f) {
			phase_ = Phase::APPROACH;
		}
		break;
	default:
		break;
	}

	// 行列計算
	worldTransform_.UpdateMatrix();

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
