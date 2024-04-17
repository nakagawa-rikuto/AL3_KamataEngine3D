#include "Player.h"

void Player::Initialeze(Model* model, uint32_t texturehandle, ViewProjection* viewProjection) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = texturehandle;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
}

void Player::Update() {

	// 行列を定数バッファに転送
	// TransferMatrixはワールド行列を転送する関数
	worldTransform_.TransferMatrix();
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}
