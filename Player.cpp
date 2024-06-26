#define NOMINMAX
#include "Player.h"
#include "TextureManager.h"

// 
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

// 
void Player::InitializeFloatingGimmick() {floatingParameter_ = 0.0f; }

// 
void Player::InitializeArmGimmick() { armParameter_ = 0.0f; }

// 
void Player::UpdateFloatingGimmick() {

	// 浮遊移動のサイクル<frame>
	const uint16_t period = 180;

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * pi() / period;

	// パラメータを1ステップ分加算
	floatingParameter_ += step;

	// 2πを超えたら0に戻す
	floatingParameter_ = std::fmod(floatingParameter_, 2.0f * pi());

	// 浮遊の振幅<m>
	float amplitude = 1.0f;

	// 浮遊を座標に反映
	worldTransformBody_.translation_.y = std::sin(floatingParameter_) * amplitude;
}

void Player::UpdateArmGimmick() {

	// 浮遊移動のサイクル<frame>
	const uint16_t period = 180;

	// 1フレームでのパラメータ加算値
	const float step = 2.0f / period;

	// パラメータを1ステップ分加算
	armParameter_ += step;

	// パラメータを0から2の範囲に制限
	if (armParameter_ > 2.0f) {
		armParameter_ -= 2.0f;
	}

	// 浮遊の振幅<m>
	const float amplitude = 0.5f;

	  // 回転のオフセット（中心値）
	const float rotationOffset = 0.0f; // 回転の中心値を適切に設定

	// 三角波の生成
	float triangleWave;
	if (armParameter_ < 1.0f) {
		triangleWave = 2.0f * armParameter_ - 1.0f;
	} else {
		triangleWave = 1.0f - 2.0f * (armParameter_ - 1.0f);
	}

	// 浮遊を座標に反映
	worldTransformLeftArm_.rotation_.x = rotationOffset + triangleWave * amplitude;
	worldTransformRightArm_.rotation_.x = rotationOffset + triangleWave * amplitude;
}

// 
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

	// 親子関係を構築
	worldTransformBody_.SetParent(&worldTransform_);
	worldTransformFace_.SetParent(&worldTransformBody_);
	worldTransformCore_.SetParent(&worldTransformBody_);
	worldTransformLeftArm_.SetParent(&worldTransformBody_);
	worldTransformRightArm_.SetParent(&worldTransformBody_);

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;

	// 浮遊の初期化
	InitializeFloatingGimmick();

	// 腕のギミックの初期化
	InitializeArmGimmick();
}

// 
void Player::Update() {

	Move();

	UpdateFloatingGimmick();

	UpdateArmGimmick();

	// 行列の再計算と転送
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformFace_.UpdateMatrix();
	worldTransformCore_.UpdateMatrix();
	worldTransformLeftArm_.UpdateMatrix();
	worldTransformRightArm_.UpdateMatrix();
}

// 
void Player::Draw() {

	// 3Dモデルを描画
	bodyModel_->Draw(worldTransformBody_, *viewProjection_);
	faceModel_->Draw(worldTransformFace_, *viewProjection_);
	coreModel_->Draw(worldTransformCore_, *viewProjection_);
	leftArmModel_->Draw(worldTransformLeftArm_, *viewProjection_);
	rightArmModel_->Draw(worldTransformRightArm_, *viewProjection_);
	
}
