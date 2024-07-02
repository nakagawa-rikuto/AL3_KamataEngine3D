#define NOMINMAX
#include "Player.h"
#include "TextureManager.h"

// 
void Player::Move() {

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
void Player::BehaviorRootInitialize() {}

// 
void Player::BehaviorAttackInitialize() {}

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

// 
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

//
void Player::BehaviorRootUpdate() {

	// ビヘイビアの遷移
	if (changeTimer_ <= 0) {
		behaviorRequest_ = Behavior::kAttack;
		changeTimer_ = 150.0f;
	}

	Move();

	UpdateFloatingGimmick();

	UpdateArmGimmick();

	// 行列の再計算と転送
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformFace_.UpdateMatrix();
	worldTransformCore_.UpdateMatrix();
	worldTransformLeftArm_.UpdateMatrix();
	worldTransformRightArm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}

//
void Player::BehaviorAttackUpdate() {

	if (changeTimer_ <= 0) {
	
		behaviorRequest_ = Behavior::kRoot;
		changeTimer_ = 300.0f;
	}

	// 振り下げのサイクル<frame>
	const uint16_t downPeriod = 150;

	// 1フレームでのパラメータ加算値
	const float downStep = 2.0f / downPeriod;

	// 振り下げパラメータを1ステップ分加算
	armParameter_ += downStep;

	// パラメータを0から2の範囲に制限
	if (armParameter_ > 2.0f) {
		armParameter_ -= 2.0f;
	}

	// 三角波の生成（振り下げ用）
	float downTriangleWave;
	if (armParameter_ < 1.0f) {
		downTriangleWave = 2.0f * armParameter_ - 1.0f;
	} else {
		downTriangleWave = 1.0f - 2.0f * (armParameter_ - 1.0f);
	}

	// 角度の範囲を設定
	const float startAngleArm = -210.0f; // 開始角度(Arm)
	const float endAngleArm = -80.0f;    // 終了角度(Arm)
	const float startAngleWeapon = -30.0f;
	const float endAngleWeapon = 100.0f;

	// 三角波を角度範囲にマッピング
	float mappedAngleArm = startAngleArm + (downTriangleWave + 1.0f) / 2.0f * (endAngleArm - startAngleArm);
	float mappedAngleWeapon = startAngleWeapon + (downTriangleWave + 1.0f) / 2.0f * (endAngleWeapon - startAngleWeapon);

	// 左腕の回転（振り下げ）
	worldTransformLeftArm_.rotation_.x = mappedAngleArm * (pi() / 180.0f); // 度からラジアンに変換

	// 右腕の回転（振り下げ）
	worldTransformRightArm_.rotation_.x = mappedAngleArm * (pi() / 180.0f); // 度からラジアンに変換

	// アイテムの回転
	worldTransformWeapon_.rotation_.x = mappedAngleWeapon * (pi() / 180.0f);  // 度からラジアンに変換

	// 行列の再計算と転送
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformFace_.UpdateMatrix();
	worldTransformCore_.UpdateMatrix();
	worldTransformLeftArm_.UpdateMatrix();
	worldTransformRightArm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}

// 初期化
void Player::Initialize(const std::vector<Model*>& models) {

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
	worldTransformWeapon_.SetParent(&worldTransformBody_);

	worldTransformLeftArm_.translation_ = {-1.8f, 3.0f, 0.0f};
	worldTransformRightArm_.translation_ = {1.8f, 3.0f, 0.0f};

	// 浮遊の初期化
	InitializeFloatingGimmick();

	// 腕のギミックの初期化
	InitializeArmGimmick();
}

// 更新
void Player::Update() { 


	ImGui::DragFloat3("worldTransform.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.rotate", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("Weapon.translation", &worldTransformLeftArm_.translation_.x, 0.01f);
	ImGui::DragFloat3("Weapon.rotate", &worldTransformLeftArm_.rotation_.x, 0.01f);
	ImGui::DragFloat("Timer", &changeTimer_);

	// カウントの減少
	changeTimer_--;

	

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
	}
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
	
		models_[kModelIndexWeapon]->Draw(worldTransformWeapon_, viewProjection);
	}
}
