#include "FollowCamera.h"
#include "LockOn.h"

void FollowCamera::Move() {

	if (target_) {

		// 追従対象からカメラまでのオフセット
		Vector3 offset = {0.0f, 5.0f, -40.0f};

		offset = TransformNormal(offset, 
			Multiply(Multiply(
				MakeRotateXMatrix(viewProjection_.rotation_.x),
				MakeRotateYMatrix(viewProjection_.rotation_.y)), 
				MakeRotateZMatrix(viewProjection_.rotation_.z)
			));

		// 座標をコピーしてオフセット分ずらす
		viewProjection_.translation_ = target_->translation_ + offset;
	}
}

void FollowCamera::Rotate() {

	// ジョイスティック
	XINPUT_STATE joyState;

	// ジョイスティックが有効なら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// 回転速度
		const float rotate = 0.05f;

		// 回転の動き
		viewProjection_.rotation_.y += 
			static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * rotate;
	}
}

void FollowCamera::LockOnMove() {

	// ロックオン座標
	Vector3 lockOnPosition = lockOn_->GetTargetPosition();

	// 追従対象からロックオン対象へのベクトル
	Vector3 sub = lockOnPosition - target_->translation_;

	// y軸周りの角度
	viewProjection_.rotation_.y = std::atan2(sub.x, sub.z);
}

void FollowCamera::Initialize() { 

	viewProjection_.Initialize(); 
}

void FollowCamera::Update() {

	// ロックオン中
	if (lockOn_->ExistTarget()) {

		Move();

		LockOnMove();
	} 
	// ロックオン中じゃない
	else {

		Move();

		Rotate();
	}

	// ビュー行列の更新
	viewProjection_.UpdateMatrix();
}