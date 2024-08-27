#define NOMINMAX
#include "Player.h"
#include "TextureManager.h"

void Player::Move() {

	ImGui::DragFloat3("worldTransform.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.rotate", &worldTransform_.rotation_.x, 0.01f);

	// ジョイスティック
	XINPUT_STATE joyState;

	// ジョイスティックが有効なら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// しきい値の設定
		const float threshold = 0.7f;

		// 移動フラグ
		bool isMoving = false;

		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {
			static_cast<float>(joyState.Gamepad.sThumbLX), 
			0.0f, 
			static_cast<float>(joyState.Gamepad.sThumbLY)
		};

		// 押し込みが遊び範囲を超えている場合
		if (Length(move) > threshold) {
			isMoving = true;
		}

		if (isMoving) {

			// 移動量に速さを反映
			move = Normalize(move) * speed;

			// 移動ベクトルをカメラの角度だけ回転する
			move = TransformNormal(
			    move, Multiply(Multiply(MakeRotateXMatrix(viewProjection_->rotation_.x), MakeRotateYMatrix(-viewProjection_->rotation_.y)), MakeRotateZMatrix(viewProjection_->rotation_.z)));

			// 移動
			worldTransform_.translation_ += move;

			// 移動方向に見た目を合わせる
			worldTransform_.rotation_.y = std::atan2(move.x, move.z);
			Vector3 moveZ = TransformNormal(move, MakeRotateYMatrix(worldTransform_.rotation_.y));

			worldTransform_.rotation_.x = std::atan2(-moveZ.y, moveZ.z);
		}
	}
}

void Player::Initialize(Model* model, ViewProjection* viewProjection) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
}

void Player::Update() {

	Move();

	// 行列の再計算と転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}
