#include "Enemy.h"

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインターチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	// シングルトンインスタンス
	worldTransform_.Initialize();
	worldTransform_.translation_.z = 5.0f;

	// 初期フェーズの設定
	phase_ = Phase::APPROACH;

	pFunc = &Enemy::Approach;
}

void Enemy::Update() {

	/*switch (phase_) {
	case Enemy::Phase::APPROACH:

		Approach();
		break;
	case Enemy::Phase::LEACE:

		Leace();
		break;
	default:
		break;
	}*/

	// メンバ関数ポインタに入っている関数を呼び出す
	(this->*Enemy::Phase[static_cast<size_t>(phase_)])();

	// 行列計算
	worldTransform_.UpdateMatrix();

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach() {

	// 移動(ベクトルの減算)
	worldTransform_.translation_.x += 0.1f;
	worldTransform_.translation_.z -= 0.3f;

	// 規定の位置に到達したら離脱に切り替え
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::LEACE;
	}
}

void Enemy::Leace() {

	// 移動(ベクトルを加算)
	worldTransform_.translation_.x -= 0.1f;
	worldTransform_.translation_.z += 0.3f;

	// 規定の距離に到達したら接近に切り替え
	if (worldTransform_.translation_.z > 10.0f) {
		phase_ = Phase::APPROACH;
	}
}

void (Enemy::*Enemy::Phase[])(){
    &Enemy::Approach, // 接近
    &Enemy::Leace     // 離脱
};
