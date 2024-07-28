#define NOMINMAX
#include "Player.h"
#include "GlobalVariables.h"
#include "Hammer.h"
#include "LockOn.h"
#include "TextureManager.h"

// 衝突判定
void Player::OnCollision() {

	// ジャンプリクエスト
	behaviorRequest_ = Behavior::kJump;
}

/* ///////////////////////////////////////////
                    初期化
*/ ///////////////////////////////////////////
//  浮遊魏キックの初期化
void Player::InitializeFloatingGimmick() { floatingParameter_ = 0.0f; }

// 腕のギミックの初期化
void Player::InitializeArmGimmick() { armParameter_ = 0.0f; }

// 通常行動の初期化
void Player::BehaviorRootInitialize() { velocity_ = {0.0f, 0.0f, 0.0f}; }

// 攻撃行動の初期化
void Player::BehaviorAttackInitialize() {

	// 状態
	hammer_->WorldTransformInitialize();
	velocity_ = {};
	weaponAngle_ = startAngleWeapon_; // 初期角度
}

// ジャンプ行動の初期化
void Player::BehaviorJumpInitialize() {
	worldTransform_.translation_.y = 0.0f;

	// ジャンプの初速
	const float kJumpFirstSpeed = 3.0f;

	// ジャンプの初速を与える
	velocity_.y = kJumpFirstSpeed;
}

/* ///////////////////////////////////////////
                    更新
*/ ///////////////////////////////////////////
// 浮遊ギミックの更新
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

// 腕ギミックの更新
void Player::UpdateArmGimmick() {

	// 振り上げのサイクル<frame>
	const uint16_t period = 150;

	// 1フレームでのパラメータ加算値
	const float step = 2.0f / period;

	// パラメータを1ステップ分加算
	armParameter_ += step;

	// パラメータを0から2の範囲に制限
	if (armParameter_ > 2.0f) {
		armParameter_ -= 2.0f;
	}

	// 振り上げの振幅<度>
	const float amplitude = 50.0f; // 0度からn度まで振り上げ

	// 三角波の生成
	float triangleWave;
	if (armParameter_ < 1.0f) {
		triangleWave = 2.0f * armParameter_ - 1.0f;
	} else {
		triangleWave = 1.0f - 2.0f * (armParameter_ - 1.0f);
	}

	// 左腕のオフセットと回転
	worldTransformLeftArm_.rotation_.x = triangleWave * amplitude * (pi() / 180.0f); // 度からラジアンに変換

	// 右腕のオフセットと回転
	worldTransformRightArm_.rotation_.x = triangleWave * amplitude * (pi() / 180.0f); // 度からラジアンに変換
}

// 通常行動の更新
void Player::BehaviorRootUpdate() {

	// ジョイスティック
	XINPUT_STATE joyState;

	// 移動処理
	// ジョイスティックが有効なら
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		// Attackボタンを押したら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {

			// 行動リクエストにアタックをセット
			behaviorRequest_ = Behavior::kAttack;
		}

		// ジャンプボタンを押したら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {

			// 行動リクエストにジャンプをセット
			behaviorRequest_ = Behavior::kJump;
		}

		// 速さ
		const float speed = 0.5f;

		// 移動量
		velocity_ = {
		    (float) joyState.Gamepad.sThumbLX / SHRT_MAX, 
			0.0f, 
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX,
		};

		// 移動量に速さを反映
		velocity_ = Normalize(velocity_) * speed;

		// 移動ベクトルをカメラの角度だけ回転する
		velocity_ = TransformNormal(
		    velocity_, Multiply(Multiply(
				MakeRotateXMatrix(viewProjection_->rotation_.x), 
				MakeRotateYMatrix(-viewProjection_->rotation_.y)), 
				MakeRotateZMatrix(viewProjection_->rotation_.z)));

		// 移動
		worldTransform_.translation_ += velocity_;

		// ロックオン状態なら
		if (lockOn_->ExistTarget()) {

			// ロックオン座標
			Vector3 lockOnPosition = lockOn_->GetTargetPosition();

			// 追従対象からロックオン対象へのベクトル
			Vector3 sub = lockOnPosition - GetWorldTransform().translation_;

			// y軸周りの角度
			worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);

		} else {

			// 移動方向に見た目を合わせる
			worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);

			Vector3 moveZ = TransformNormal(velocity_, MakeRotateYMatrix(worldTransform_.rotation_.y));

			worldTransform_.rotation_.x = std::atan2(-moveZ.y, moveZ.z);
		}
	}

	// 浮遊処理
	UpdateFloatingGimmick();

	// 腕の処理
	UpdateArmGimmick();
}

// 攻撃行動の更新
void Player::BehaviorAttackUpdate() {

	Vector3 rotation = hammer_->GetRotate();

	if (lockOn_->ExistTarget()) {

		// ロックオン座標
		Vector3 lockOnPosition = lockOn_->GetTargetPosition();

		// 追従対象からロックオン対象へのベクトル
		Vector3 sub = lockOnPosition - GetWorldTransform().translation_;

		// y軸周りの角度
		worldTransform_.rotation_.y = std::atan2(sub.x, sub.z);

		//// 距離
		// float distance = Length(sub);

		//// 距離しきい値
		// const float threshold = 0.2f;

		//// しきい値より離れている時のみ
		// if (distance > threshold) {

		//	// しきい値を超える速さなら補正する
		//	if (speed > distance - threshold) {
		//
		//		// ロックオン対象へのめりこみ防止
		//		speed = distance - threshold;
		//	}
		//}
	}

	// 加速フェーズ
	if (weaponAngle_ < (startAngleWeapon_ + endAngleWeapon_) / 2.0f && velocity_.y < maxVelocity_) {
		velocity_.y += acceleration_;
	}
	// 減速フェーズ
	else if (weaponAngle_ >= (startAngleWeapon_ + endAngleWeapon_) / 2.0f && velocity_.y > 0.0f) {
		velocity_.y -= deceleration_;
	}

	// 速度を使って角度を更新
	weaponAngle_ += velocity_.y;

	// 武器の角度をアームの角度にマッピン
	const float startAngleArm = -210.0f;
	const float endAngleArm = -80.0f;
	float mappedAngleArm = startAngleArm + (weaponAngle_ - startAngleWeapon_) / (endAngleWeapon_ - startAngleWeapon_) * (endAngleArm - startAngleArm);

	// 度からラジアンに変換して武器の回転を設定
	rotation.x = weaponAngle_ * (pi() / 180.0f);
	worldTransformLeftArm_.rotation_.x = mappedAngleArm * (pi() / 180.0f);
	worldTransformRightArm_.rotation_.x = mappedAngleArm * (pi() / 180.0f);

	hammer_->SetRotate(rotation);

	// 角度を範囲に制限
	if (weaponAngle_ >= 80.0f) {
		weaponAngle_ = endAngleWeapon_;
		velocity_ = {0.0f, 0.0f, 0.0f};     // 振り下げ完了時に速度をゼロにする
		behaviorRequest_ = Behavior::kRoot; // 動作リクエストを変更
	}
}

// ジャンプ行動の更新
void Player::BehaviorJumpUpdate() {

	// 移動
	worldTransform_.translation_ += velocity_;

	// 重力加速度
	const float kGravityAcceleration = 0.5f;

	// 加速度ベクトル
	Vector3 accelerationVector = {0, -kGravityAcceleration, 0};

	// 加速する
	velocity_ += accelerationVector;

	// 着地
	if (worldTransform_.translation_.y <= 0.0f) {

		worldTransform_.translation_.y = 0.0f;

		// ジャンプの終了
		behaviorRequest_ = Behavior::kRoot;
	}
}

// 調整項目の適用
void Player::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformBody_.translation_ = globalVariables->GetVector3Value(groupName, "Body Translation");
	worldTransformLeftArm_.translation_ = globalVariables->GetVector3Value(groupName, "LeftArm Translation");
	worldTransformRightArm_.translation_ = globalVariables->GetVector3Value(groupName, "RightArm Translation");
}

// 中心座標を取得
Vector3 Player::GetCenterPosition() const {

	// ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 1.5f, 0.0f};

	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

// 初期化
void Player::Initialize(const std::vector<Model*>& models) {

	input_ = Input::GetInstance();

	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// ワールド変換の初期化
	worldTransform_.Initialize();

	// 親子関係を構築
	worldTransformBody_.SetParent(&worldTransform_);
	worldTransformFace_.SetParent(&worldTransformBody_);
	worldTransformCore_.SetParent(&worldTransformBody_);
	worldTransformLeftArm_.SetParent(&worldTransformBody_);
	worldTransformRightArm_.SetParent(&worldTransformBody_);

	// 位置の調整
	worldTransform_.translation_ = {0.0f, .0f, -5.0f};
	worldTransformLeftArm_.translation_ = {-1.8f, 3.0f, 0.0f};
	worldTransformRightArm_.translation_ = {1.8f, 3.0f, 0.0f};

	hammer_ = std::make_unique<Hammer>();
	hammer_->Initialize(models_[kModelIndexWeapon], &worldTransformBody_);
	
	// 浮遊の初期化
	InitializeFloatingGimmick();

	// 腕のギミックの初期化
	InitializeArmGimmick();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	// グループの追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Body Translation", worldTransformBody_.translation_);
	globalVariables->AddItem(groupName, "LeftArm Translation", worldTransformLeftArm_.translation_);
	globalVariables->AddItem(groupName, "RightArm Translation", worldTransformRightArm_.translation_);
}

// 更新
void Player::Update() {

	ImGui::DragFloat3("worldTransform.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat("weaponAngle", &weaponAngle_, 0.1f);

	if (input_->TriggerKey(DIK_SPACE)) {

		GlobalVariables::GetInstance()->SaveFile("Player");
	}

	// ハンマーの更新
	hammer_->Update();

	/* /////////////////
	  Behavior遷移の実装
	*/ /////////////////
	if (behaviorRequest_) {

		// 振る舞いを変更
		behavior_ = behaviorRequest_.value();

		// 各振る舞い毎の初期化
		switch (behavior_) {
		case Player::Behavior::kRoot:

			BehaviorRootInitialize();
			break;
		case Player::Behavior::kAttack:

			BehaviorAttackInitialize();
			break;
		case Player::Behavior::kJump:

			BehaviorJumpInitialize();
			break;
		}

		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 各振る舞い毎の更新
	switch (behavior_) {
	case Player::Behavior::kRoot:

		BehaviorRootUpdate();
		break;
	case Player::Behavior::kAttack:

		BehaviorAttackUpdate();
		break;
	case Player::Behavior::kJump:

		BehaviorJumpUpdate();
		break;
	}

	ApplyGlobalVariables();

	// 行列の再計算と転送
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformFace_.UpdateMatrix();
	worldTransformCore_.UpdateMatrix();
	worldTransformLeftArm_.UpdateMatrix();
	worldTransformRightArm_.UpdateMatrix();
}

// 描画
void Player::Draw(const ViewProjection& viewProjection) {

	// 3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection);
	models_[kModelIndexFace]->Draw(worldTransformFace_, viewProjection);
	models_[kModelIndexCore]->Draw(worldTransformCore_, viewProjection);
	models_[kModelIndexL_Arm]->Draw(worldTransformLeftArm_, viewProjection);
	models_[kModelIndexR_Arm]->Draw(worldTransformRightArm_, viewProjection);

	if (behavior_ == Behavior::kAttack) {
		hammer_->Draw(viewProjection);
	}
}
