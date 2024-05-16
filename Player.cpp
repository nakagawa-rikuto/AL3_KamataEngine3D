#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include <DebugText.h>
#include <algorithm>
#include <numbers>

void Player::InputMove() {

	if (onGround_) {

		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力時は急ブレーキ
					velocity_.x *= (1.0f - kAttenuation_);
				}

				// 左右状態の切り替え
				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;

					// 最初の角度を記録
					turnFirstRotationY_ = worldTransform_.rotation_.y;

					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn_;
				}
				acceleration.x += KAcceleration_;
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

				if (velocity_.x > 0.0f) {

					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - KAcceleration_);
				}

				// 左右状態の切り替え
				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;

					// 最初の角度を記録
					turnFirstRotationY_ = worldTransform_.rotation_.y;

					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn_;
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

		/*///////////////////////////////////////////
		                 ジャンプ
		*////////////////////////////////////////////

		if (velocity_.y <= 0) {
			if (Input::GetInstance()->PushKey(DIK_UP)) {

				// ジャンプ初速
				velocity_.y += kJumpAcceleration_;
			}
		}

		// ジャンプ開始
		if (velocity_.y > 0.0f) {

			// 空中状態に移行
			onGround_ = false;
		}

		// 空中
	} else {

		// 着地フラグ
		bool landing_ = false;

		// 落下
		velocity_.y += -kGravityAcceleration_;

		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed_);

		// 地面との当たり判定
		// 下降中？
		if (velocity_.y < 0) {

			// Y座標が地面以下になったら着地
			if (worldTransform_.translation_.y <= 2.0f) {
				landing_ = true;
			}
		}

		// 着地
		if (landing_) {

			// めり込み排除
			worldTransform_.translation_.y = 2.0f;

			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuationLanding_);

			// 下方向速度をリセット
			velocity_.y = 0.0f;

			// 接地状態に移行
			onGround_ = true;
		}
	}
	// 移動
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
}

void Player::Turning() {

	
	if (turnTimer_ > 0.0f) {

		// 旋回タイマーを1/60秒分カウントダウンする
		turnTimer_ -= 1.0f / 60.0f;

		// 右の自キャラの角度テーブル
		float destinationRotationYTable[] = {
		    std::numbers::pi_v<float> / 2.0f,       // Right
		    std::numbers::pi_v<float> * 3.0f / 2.0f // Left
		};

		// 最終的な目標角度を取得する
		float turnLastRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

		// 時間によっての目標角度
		// 経過時間が増加するにつれてtの値を0に近づかせる
		float t = 1.0f - turnTimer_ / kTimeTurn_;
		float easedT = EaseIn(t);

		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = (1.0f - t) * turnFirstRotationY_ + easedT * turnLastRotationY;
	}
}

void Player::CollisionMap(CollisionMapInfo& info, CollisionMapInfo& preInfo) {
	CollisionDistanceTop(info, preInfo);
	// CollisionDistanceBottom(info);
	// CollisionDistanceRight(info);
	// CollisionDistanceLeft(info);
}

void Player::CollisionDistanceTop(CollisionMapInfo& info, CollisionMapInfo& preInfo) {

	// 上昇
	if (info.moveVolume_.y <= 0) {
		// 早期リターン・ガード節
		return;
	}

	// 移動後の４つの角の座標
	std::array<Vector3, 4> positionsNew;

	// 移動後の４つの角の計算
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveVolume_, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真上の当たり判定を行う
	bool hitLeft = false;
	bool hitRight = false;

	// 左上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hitLeft = true;
	} else if (mapChipType == MapChipType::kBlank) {
		hitLeft = false;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hitRight = true;
	} else if (mapChipType == MapChipType::kBlank) {
		hitRight = false;
	}

	// ブロックにヒット
	if (hitLeft || hitRight) {
		// めり込みを排除する方向に移動量を調整する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop || kLeftTop]);
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveVolume_.y = std::max(0.0f, info.moveVolume_.y);

		// 天井に当たったことを記録する
		info.kCeiling_ = true;
	} else {

		info.kCeiling_ = false;
		preInfo.kCeiling_ = info.kCeiling_;
	}
}

// void Player::CollisionDistanceBottom(CollisionMapInfo& info) {}

// void Player::CollisionDistanceRight(CollisionMapInfo& info) {}

// void Player::CollisionDistanceLeft(CollisionMapInfo& info) {}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth_ / 2.0f, -kHeight_ / 2.0f, 0}, // kRightBottom
	    {-kWidth_ / 2.0f, -kHeight_ / 2.0f, 0}, // kLeftBottom
	    {+kWidth_ / 2.0f, +kHeight_ / 2.0f, 0}, // kRightTop
	    {-kWidth_ / 2.0f, +kHeight_ / 2.0f, 0}  // kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::CollisionResultMove(CollisionMapInfo& info) {

	// 移動
	worldTransform_.translation_ += info.moveVolume_;
}

void Player::CollisionCeiling(const CollisionMapInfo& info, CollisionMapInfo& preInfo) {

	// 天井に当たった
	if (info.kCeiling_ && !preInfo.kCeiling_) {
		DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		preInfo.kCeiling_ = info.kCeiling_;
		velocity_.y = 0.0f;
	}
}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& pos) {

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

	/*///////////////////////////////////////////
	                   移動入力
	*////////////////////////////////////////////
	InputMove();

	/*///////////////////////////////////////////
	             移動量を加味して衝突判定する
	*////////////////////////////////////////////

	// 移動量に速度の値をコピー
	collisionMapInfo_.moveVolume_ = velocity_;

	// マップ衝突チェック
	CollisionMap(collisionMapInfo_, preCollisionMapInfo_);

	/*///////////////////////////////////////////
	            判定結果を反映して移動させる
	*////////////////////////////////////////////
	CollisionResultMove(collisionMapInfo_);

	/*///////////////////////////////////////////
	             天井に接触している場合の処理
	*////////////////////////////////////////////
	CollisionCeiling(collisionMapInfo_, preCollisionMapInfo_);

	/*///////////////////////////////////////////
	             壁に接触している場合の処理
	*////////////////////////////////////////////

	/*///////////////////////////////////////////
	             設置状態の切り替え
	*////////////////////////////////////////////

	/*///////////////////////////////////////////
	                 旋回制御
	*////////////////////////////////////////////
	Turning();

	/*///////////////////////////////////////////
	        　　     行列の計算
	*////////////////////////////////////////////
	// 行列計算
	worldTransform_.UpdateMatrix();

	// 行列を更新して定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}
