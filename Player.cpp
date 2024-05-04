#define NOMINMAX
#include "Player.h"
#include <numbers>
#include <algorithm>


void Player::Initialeze(Model* model, ViewProjection* viewProjection, const Vector3& pos) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;

	/// ***********************
	/// 初期回転
	/// ***********************
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {

	/// *********************
	/// 速度
	/// *********************
	// 移動入力
	// 設置状態
	if (onGround_) {
		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力時は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation_);
				}

				// 左右状態の切り替え
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
				}

				// 旋回開始時の角度を記録
				turnFirstRotationY_ = worldTransform_.rotation_.y;

				// 旋回タイマーに時間を設定する
				turnTimer_ = kTimeTurn;

				if (turnTimer_) {

					// 旋回制御
					// 右の自キャラの角度テーブル
					float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

					// 状況に応じた角度を取得する
					float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

					// 自キャラの角度を設定する
					worldTransform_.rotation_.y = destinationRotationY;
				}

				acceleration.x += KAcceleration_;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x > 0.0f) {

					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - KAcceleration_);
				}

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
				}

				// 旋回開始時の角度を記録
				turnFirstRotationY_ = worldTransform_.rotation_.y;

				// 旋回タイマーに時間を設定する
				turnTimer_ = kTimeTurn;

				if (turnTimer_ > 0.0f) {

					// 旋回制御
					// 右の自キャラの角度テーブル
					float destinationRotationYTable[] = {
					    std::numbers::pi_v<float> / 2.0f,
					    std::numbers::pi_v<float> * 3.0f / 2.0f // 個々の数値を変更
					};

					// 状況に応じた角度を取得する
					float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

					// 自キャラの角度を設定する
					worldTransform_.rotation_.y = destinationRotationY;
				}
				acceleration.x -= KAcceleration_;
			}

			// 加速/減速
			velocity_.x += acceleration.x;
			velocity_.y += acceleration.y;
			velocity_.z += acceleration.z;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed_, kLimitRunSpeed_);

		} else {

			// 　非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAttenuation_);
		}

		// ジャンプ入力
		if (Input::GetInstance()->PushKey(DIK_UP)) {

			//ジャンプ初速
			velocity_.x += (0, kGravityAcceleration_, 0);
			velocity_.y += (0, kGravityAcceleration_, 0);
			velocity_.z += (0, kGravityAcceleration_, 0);
		}

		// ジャンプ開始
		if (velocity_.y > 0.0f) {

			// 空中状態に移行
			onGround_ = false;
		}

	// 空中
	} else {

		// 着地フラグ
		bool landing = false;

		// 地面との当たり判定
		// 下降中？
		if (velocity_.y < 0) {

			// Y座標が地面以下になったら着地
			if (worldTransform_.translation_.y <= 1.0f) {
				landing = true;
			}
		}

		// 着地
		if (landing) {

			// めり込み排
			worldTransform_.translation_.y = 1.0f;

			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation_);

			// 下方向速度をリセット
			velocity_.y = 0.0f;

			// 設置状態に移行
			onGround_ = true;
		}

		// 落下速度
		velocity_.x += (0, -kGravityAcceleration_, 0);
		velocity_.y += (0, -kGravityAcceleration_, 0);
		velocity_.z += (0, -kGravityAcceleration_, 0);

		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed_);
	}
	// 移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 行列計算
	worldTransform_.UpdateMatrix();

	// 行列を更新して定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}
