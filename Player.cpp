#define NOMINMAX
#include <DebugText.h>
#include <algorithm>
#include <numbers>
#include "Player.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインターチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
}

void Player::Update() {

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}