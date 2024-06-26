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

		// 速さ
		const float speed = 0.3f;

		// 移動量
		Vector3 move = {
			static_cast<float>(joyState.Gamepad.sThumbLX), 
			0.0f, 
			static_cast<float>(joyState.Gamepad.sThumbLY)
		};

		// 移動量に速さを反映
		move = Normalize(move) * speed;

		// 移動ベクトルをカメラの角度だけ回転する
		move = TransformNormal(move, 
			Multiply(Multiply(
				MakeRotateXMatrix(viewProjection_->rotation_.x), 
				MakeRotateYMatrix(-viewProjection_->rotation_.y)), 
				MakeRotateZMatrix(viewProjection_->rotation_.z)
			));

		// 移動
		worldTransform_.translation_ += move;

		// 移動方向に見た目を合わせる
		worldTransform_.rotation_.y = std::atan2(move.x, move.z);
		Vector3 moveZ = TransformNormal(move, MakeRotateYMatrix(worldTransform_.rotation_.y));

		worldTransform_.rotation_.x = std::atan2(-moveZ.y, moveZ.z);
	}
}

void Player::InitializeFloatingGimmick() {

	floatingParameter_ = 0.0f; 
}

void Player::UpdateFloatingGimmick() {

	// 浮遊移動のサイクル<frame>
	const uint16_t period = 300;

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * std::numbers<>

}

void Player::Initialize(Model* modelBody, Model* modelFace, Model* modelCore, Model* modelLeftArm, Model* modelRightArm, ViewProjection* viewProjection) {

	// Modelの指定
	faceModel_ = modelFace;
	bodyModel_ = modelBody;
	leftArmModel_ = modelLeftArm;
	rightArmModel_ = modelRightArm;
	coreModel_ = modelCore;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransformBody_.Initialize();
	worldTransformFace_.Initialize();
	worldTransformCore_.Initialize();
	worldTransformLeftArm_.Initialize();
	worldTransformRightArm_.Initialize();

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
	faceModel_->Draw(worldTransform_, *viewProjection_);
	bodyModel_->Draw(worldTransform_, *viewProjection_);
	leftArmModel_->Draw(worldTransform_, *viewProjection_);
	rightArmModel_->Draw(worldTransform_, *viewProjection_);
	coreModel_->Draw(worldTransform_, *viewProjection_);
}
