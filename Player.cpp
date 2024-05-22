#define NOMINMAX
#include "Player.h"

void Player::Rotate() {

	// 回転速さ[ラジアン/ frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {

		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {

		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインターチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	// シングルトンインスタンス
	input_ = Input::GetInstance();
	worldTransform_.Initialize();
}

void Player::Update() {

#ifdef _DEBUG

	ImGui::Begin("Info");
	ImGui::DragFloat3("worldTransform.rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.scale", &worldTransform_.scale_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();
#endif // DEBUG

	/* //////////////////////
	        旋回処理
	*/ //////////////////////
	Rotate();

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	/* //////////////////////
	        移動入力
	*/ //////////////////////
	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// おしたほうこうで移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 移動限界
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	// 範囲を超えない処理
	// Clampは－(マイナス)の値を取れない
	//worldTransform_.translation_.x = std::clamp(worldTransform_.translation_.x, kMoveLimitX, -kMoveLimitX);
	//worldTransform_.translation_.y = std::clamp(worldTransform_.translation_.y, kMoveLimitY, -kMoveLimitY);
	worldTransform_.translation_.x = std::max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = std::min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = std::max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = std::min(worldTransform_.translation_.y, kMoveLimitY);

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;

	/* //////////////////////
	        攻撃処理
	*/ //////////////////////
	Attack();

	// 弾の更新　
	if (bullet_) {

		bullet_->Update();
	}

	// 行列計算
	worldTransform_.UpdateMatrix();

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	if (bullet_) {

		bullet_->Draw(viewProjection);
	}
}

void Player::Attack() {

	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, worldTransform_.translation_);

		// 弾を登録すr
		bullet_ = newBullet;
	}
}
