#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include <DebugText.h>
#include <algorithm>
#include <numbers>

// AABB取得関数
AABB Player::GetAABB() { 
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth_ / 2.0f, worldPos.y - kHeight_ / 2.0f, worldPos.z - kWidth_ / 2.0f};
	aabb.max = {worldPos.x + kWidth_ / 2.0f, worldPos.y + kHeight_ / 2.0f, worldPos.z + kWidth_ / 2.0f};

	return aabb;
}

void Player::ImGuiDebug(CollisionMapInfo& info, CollisionMapInfo& preInfo) {

#ifdef _DEBUG

	// 見たい情報を書き込む
	ImGui::Begin("Player");

	ImGui::Checkbox("onGround", &onGround_);
	ImGui::Checkbox("collisionMapInfo.kCeiling", &info.kCeiling_);
	ImGui::Checkbox("collisionMapInfo.kLanding", &info.kLanding_);
	ImGui::Checkbox("collisionMapInfo.kWalling", &info.kWall_);
	ImGui::Checkbox("collisionMapInfoPre.kWalling", &preInfo.kWall_);
	ImGui::DragFloat3("Veloctiy", &velocity_.x, 0.01f);

	ImGui::End();

#endif
}

void Player::InputMove() {

	if (onGround_) {

		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {

			// 左右加速
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_D)) {

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
			} else if (Input::GetInstance()->PushKey(DIK_LEFT) || Input::GetInstance()->PushKey(DIK_A)) {

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
			if (Input::GetInstance()->PushKey(DIK_UP) || Input::GetInstance()->PushKey(DIK_W)) {

				// ジャンプ初速
				velocity_.y += kJumpAcceleration_;
			}
		}

		// 空中
	} else {

		// 落下
		velocity_.y += -kGravityAcceleration_;

		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed_);
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

// 当たり判定
void Player::CollisionMap(CollisionMapInfo& info, CollisionMapInfo& preInfo) {
	CollisionDistanceTop(info, preInfo);
	CollisionDistanceBottom(info, preInfo);
	CollisionDistanceRight(info, preInfo);
	CollisionDistanceLeft(info, preInfo);
}

// 当たり判定(上)
void Player::CollisionDistanceTop(CollisionMapInfo& info, CollisionMapInfo& preInfo) {

	// 上昇
	if (info.moveVolume_.y <= 0) {
		// 早期リターン・ガード節
		return;
	}

	// 移動前の座標
	Vector3 positionsPre;
	positionsPre = worldTransform_.translation_ - info.moveVolume_;

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

	IndexSet indexSet;

	// 左上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hitLeft = true;
	} else {
		hitLeft = false;
	}

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hitRight = true;
	} else {
		hitRight = false;
	}

	// ブロックにヒット
	if (hitLeft || hitRight) {
		// めり込みを排除する方向に移動量を調整する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop || kLeftTop]);
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveVolume_.y = std::max(0.0f, rect.bottom - positionsPre.y - 1.0f + margin_);

		// 天井に当たったことを記録する
		info.kCeiling_ = true;
	} else {

		info.kCeiling_ = false;
		preInfo.kCeiling_ = info.kCeiling_;
	}
}

// 当たり判定(下)
void Player::CollisionDistanceBottom(CollisionMapInfo& info, CollisionMapInfo& preInfo) {

	// 上昇
	if (info.moveVolume_.y >= 0) {
		// 早期リターン・ガード節
		return;
	}

	// 移動前の座標
	Vector3 positionsPre;
	positionsPre = worldTransform_.translation_ - info.moveVolume_;

	// 移動後の４つの角の座標
	std::array<Vector3, 4> positionsNew;

	// 移動後の４つの角の計算
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveVolume_, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	// 真下の当たり判定を行う
	bool hitLeft = false;
	bool hitRight = false;

	IndexSet indexSet;

	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hitLeft = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hitRight = true;
	}

	// ブロックヒット
	if (hitLeft || hitRight) {

		// めり込みを排除する方向に移動量を接地する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom || kRightBottom]);

		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveVolume_.y = std::min(0.0f, (rect.top - positionsPre.y + 1.0f + margin_));

		// 地面に当たったことを記録する
		info.kLanding_ = true;

	} else {

		info.kLanding_ = false;
		preInfo.kLanding_ = info.kLanding_;
	}
}

// 当たり判定(右)
void Player::CollisionDistanceRight(CollisionMapInfo& info, CollisionMapInfo& preInfo) {

	// 右移動中
	if (velocity_.x <= 0) {
		return;
	}

	// 移動前の座標
	Vector3 positionsPre;
	positionsPre = worldTransform_.translation_ - info.moveVolume_;

	// 移動後の４つの角の座標
	std::array<Vector3, 4> positionsNew;

	// 移動後の４つの角の計算
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveVolume_, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hitTop = false;
	bool hitBottom = false;

	IndexSet indexSet;

	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hitTop = true;
	}

	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hitBottom = true;
	}

	if (info.kCeiling_ || info.kLanding_) {

		// ブロックにヒット
		if (hitTop && hitBottom) {

			// めり込みを排除する方向に移動量を接地する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop || kRightBottom]);

			// めり込み先ブロックの範囲矩形
			Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.moveVolume_.x = std::max(0.0f, (rect.left - positionsPre.x - 1.0f + margin_));

			// 壁に当たっていることを記録
			info.kWall_ = true;
		} else {

			info.kWall_ = false;
			preInfo.kWall_ = info.kWall_;
		}
	} else {

		// ブロックにヒット
		if (hitTop || hitBottom) {

			// めり込みを排除する方向に移動量を接地する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop || kRightBottom]);

			// めり込み先ブロックの範囲矩形
			Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.moveVolume_.x = std::max(0.0f, (rect.left - positionsPre.x - 1.0f + margin_));

			// 壁に当たっていることを記録
			info.kWall_ = true;
		} else {

			info.kWall_ = false;
			preInfo.kWall_ = info.kWall_;
		}
	}
}

// 当たり判定(左)
void Player::CollisionDistanceLeft(CollisionMapInfo& info, CollisionMapInfo& preInfo) {

	// 右移動中
	if (velocity_.x >= 0) {
		return;
	}

	// 移動前の座標
	Vector3 positionsPre;
	positionsPre = worldTransform_.translation_ - info.moveVolume_;

	// 移動後の４つの角の座標
	std::array<Vector3, 4> positionsNew;

	// 移動後の４つの角の計算
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveVolume_, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hitTop = false;
	bool hitBottom = false;

	IndexSet indexSet;

	// 左上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hitTop = true;
	}

	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {

		hitBottom = true;
	}

	if (info.kCeiling_ || info.kLanding_) {

		// ブロックにヒット
		if (hitTop && hitBottom) {

			// めり込みを排除する方向に移動量を接地する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop || kLeftBottom]);

			// めり込み先ブロックの範囲矩形
			Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.moveVolume_.x = std::min(0.0f, (rect.right - positionsPre.x + 1.0f + margin_));
			// info.moveVolume_.x = std::max(0.0f, (rect.left - positionsPre.x - 1.0f + margin_));

			// 壁に当たっていることを記録
			info.kWall_ = true;
		} else {

			info.kWall_ = false;
			preInfo.kWall_ = info.kWall_;
		}
	} else {

		// ブロックにヒット
		if (hitTop || hitBottom) {

			// めり込みを排除する方向に移動量を接地する
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop || kLeftBottom]);

			// めり込み先ブロックの範囲矩形
			Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			info.moveVolume_.x = std::min(0.0f, (rect.right - positionsPre.x + 1.0f + margin_));
			// info.moveVolume_.x = std::max(0.0f, (rect.left - positionsPre.x - 1.0f + margin_));

			// 壁に当たっていることを記録
			info.kWall_ = true;
		} else {

			info.kWall_ = false;
			preInfo.kWall_ = info.kWall_;
		}
	}
}

// 指定した角の座標計算
Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {

	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth_ / 2.0f, -kHeight_ / 2.0f, 0}, // kRightBottom
	    {-kWidth_ / 2.0f, -kHeight_ / 2.0f, 0}, // kLeftBottom
	    {+kWidth_ / 2.0f, +kHeight_ / 2.0f, 0}, // kRightTop
	    {-kWidth_ / 2.0f, +kHeight_ / 2.0f, 0}  // kLeftTop
	};

	return center + offsetTable[static_cast<uint32_t>(corner)];
}

// 判定結果を反映して移動させる
void Player::CollisionResultMove(CollisionMapInfo& info) {

	// 移動
	worldTransform_.translation_ += info.moveVolume_;
}

// 天井に接触している場合の処理
void Player::CollisionCeiling(const CollisionMapInfo& info, CollisionMapInfo& preInfo) {

	// 天井に当たった
	if (info.kCeiling_ && !preInfo.kCeiling_) {
		// DebugText::GetInstance()->ConsolePrintf("hit ceiling\n");
		preInfo.kCeiling_ = info.kCeiling_;
		velocity_.y = 0.0f;
	}
}

// 壁に触れている場合の処理
void Player::CollisionWalling(const CollisionMapInfo& info, CollisionMapInfo& preInfo) {

	// 壁接触による減衰
	if (info.kWall_) {

		preInfo.kWall_ = info.kWall_;
		// DebugText::GetInstance()->ConsolePrintf("hit wall\n");
		worldTransform_.translation_.x -= velocity_.x;
		velocity_.x = 0.0f;
	}
}

// 接地状態の切り替え処理
void Player::CollisionLanding(const CollisionMapInfo& info, CollisionMapInfo& preInfo) {

	// 自キャラが設置状態かどうか
	if (onGround_) {

		/* /////////////////////////////
		           接地状態の処理
		*/ /////////////////////////////

		// ジャンプ開始
		if (velocity_.y > 0.0f) {

			onGround_ = false;
		} else {

			/* ///////////////
			     落下判定
			*/ ///////////////

			// 移動後の４つの角の座標
			std::array<Vector3, 4> positionsNew;

			// 移動後の４つの角の計算
			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveVolume_, static_cast<Corner>(i));
			}

			MapChipType mapChipType;

			// 真下の当たり判定を行う
			bool hitLeft = true;
			bool hitRight = true;

			IndexSet indexSet;

			// 左下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -margin_, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hitLeft = true;
			} else {
				hitLeft = false;
			}

			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -margin_, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				hitRight = true;
			} else {
				hitRight = false;
			}

			/* ///////////////
			落下なら空中状態に切り替え
			*/ ///////////////

			// 落下開始
			if (!hitLeft && !hitRight) {

				// 空中状態に切り替える
				onGround_ = false;
			}
		}

	} else {

		/* /////////////////////////////
		           空中状態の処理
		*/ /////////////////////////////
		// 着地フラグ
		if (info.kLanding_ && !preInfo.kLanding_) {

			preInfo.kLanding_ = info.kLanding_;

			// 着地状態に切り替える(落下を止める)
			onGround_ = true;
			// DebugText::GetInstance()->ConsolePrintf("hit Landing\n");

			// 着地時にx速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding_);

			// worldTransform_.translation_.y -= velocity_.y;

			// y速度をゼロにする
			velocity_.y = 0.0f;
		}
	}
}

// 衝突判定
void Player::OnCollision(const Enemy* enemy) { 
	(void)enemy;

	// ジャンプ開始(仮処理)
	velocity_.y += 0.5f;
}

// 初期化
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

// 更新
void Player::Update() {

	/*///////////////////////////////////////////
	                  ImGui
	*////////////////////////////////////////////
	ImGuiDebug(collisionMapInfo_, preCollisionMapInfo_);

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
	             設置状態の切り替え
	*////////////////////////////////////////////
	CollisionLanding(collisionMapInfo_, preCollisionMapInfo_);

	/*///////////////////////////////////////////
	             壁に接触している場合の処理
	*////////////////////////////////////////////
	CollisionWalling(collisionMapInfo_, preCollisionMapInfo_);

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

// 描画
void Player::Draw() {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}

// ワールド座標を取得
Vector3 Player::GetWorldPosition() { 

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
