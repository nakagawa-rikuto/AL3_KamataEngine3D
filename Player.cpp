#define NOMINMAX
#include "Player.h"
#include "GameScene.h"
#include "TextureManager.h"

Player::~Player() { delete sprite2DReticle_; }

/* //////////////////////////////////////////////////
                    Getter
*/ //////////////////////////////////////////////////
Vector3 Player::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}
Vector3 Player::GetWorld3DReticlePosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

/* //////////////////////////////////////////////////
                    衝突判定
*/ //////////////////////////////////////////////////
void Player::OnCollision() { 
	worldTransform_.scale_.x += 0.05f;
	worldTransform_.scale_.y += 0.05f;
	worldTransform_.scale_.z += 0.05f;
}

/* //////////////////////////////////////////////////
                    初期化
*/ //////////////////////////////////////////////////
void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {

	// NULLポインターチェック
	assert(model);

	model_ = model;
	bulletModel_ = Model::CreateFromOBJ("Bullet", true);

	textureHandle_ = textureHandle;

	// シングルトンインスタンス
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	worldTransform3DReticle_.parent_ = &worldTransform_;
	worldTransform3DReticle_.translation_ = {640.0f, 360.0f, 0.0f};

	// レティクル用テクスチャ取得
	textureReticle_ = TextureManager::Load("./Resources/Reticle.png");

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle_, 
		Vector2(worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y), 
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f));
}

/* //////////////////////////////////////////////////
                    更新
*/ //////////////////////////////////////////////////
void Player::Update() {

#ifdef _DEBUG
	ImGui::Begin("Info");
	ImGui::DragFloat3("worldTransform.rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.scale", &worldTransform_.scale_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.translation", &worldTransform_.translation_.x, 0.01f);

	ImGui::DragFloat3("worldTransform3D.rotation", &worldTransform3DReticle_.rotation_.x, 0.01f);
	ImGui::DragFloat3("worldTransform3D.scale", &worldTransform3DReticle_.scale_.x, 0.01f);
	ImGui::DragFloat3("worldTransform3D.translation", &worldTransform3DReticle_.translation_.x, 0.01f);
	ImGui::End();
#endif // DEBUG

	/* //////////////////////
	        移動入力
	*/ //////////////////////
	Move();

	/* //////////////////////
	        旋回処理
	*/ //////////////////////
	Rotate();

	/* //////////////////////
	        レティクル
	*/ //////////////////////
	// Update3DReticle();
	Reticle();

	/* //////////////////////
	        行列計算
	*/ //////////////////////
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
	worldTransform3DReticle_.UpdateMatrix();
}

/* //////////////////////////////////////////////////
                    描画
*/ //////////////////////////////////////////////////
void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection);
}

/* //////////////////////////////////////////////////
					移動処理
*/ //////////////////////////////////////////////////
void Player::Move() {

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	XINPUT_STATE joyState;

	// おしたほうこうで移動ベクトルを変更(上下)
	if (Input::GetInstance()->PushKey(DIK_W)) {
		move.z += kCharacterSpeed;
		worldTransform_.scale_ -= 0.0001f;
	} else if (Input::GetInstance()->PushKey(DIK_S)) {
		move.z -= kCharacterSpeed;
		worldTransform_.scale_ -= 0.0001f;
	}

	// ゲームパッド状態を維持取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.z += static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * 0.8f;
		//move.y -= static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * 0.8f;
		//worldTransform_.scale_ += static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * 0.001f;
		//worldTransform_.scale_ -= static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * 0.001f;
	}

	// 前進・後退の移動を方向に合わせる
	Vector3 forward = GetForwardVector(worldTransform_.rotation_);
	move = forward * move.z;

	// 移動限界
	const float kMoveLimitX = 120.0f;
	const float kMoveLimitY = 90.0f;

	// 範囲を超えない処理
	// Clampは－(マイナス)の値を取れない
	worldTransform_.translation_.x = std::max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = std::min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = std::max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = std::min(worldTransform_.translation_.y, kMoveLimitY);

	// 座標移動(ベクトルの加算)

	worldTransform_.translation_ += move;
}

/* //////////////////////////////////////////////////
                    旋回処理
*/ //////////////////////////////////////////////////
void Player::Rotate() {

	// カメラの移動ベクトル
	Vector3 move = {0, 0, 0};

	// 回転速さ[ラジアン/ frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {

		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_RIGHT)) {

		worldTransform_.rotation_.y += kRotSpeed;
	}

	if (Input::GetInstance()->PushKey(DIK_UP)) {

		worldTransform_.rotation_.x -= kRotSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_DOWN)) {

		worldTransform_.rotation_.x += kRotSpeed;
	}

	// ジョイスティックの処理
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x -= static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX * 0.01f;
		move.y += static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX * 0.01f;
	}

	worldTransform_.rotation_.x += move.x;
	worldTransform_.rotation_.y += move.y;
}

/* //////////////////////////////////////////////////
                    レティクルの処理
*/ //////////////////////////////////////////////////
void Player::Reticle() {

	/* /////////////////////////////////////
	       ゲームパッド
	*/ /////////////////////////////////////

	// レティクル座標の移動ベクトル
	Vector2 move = {0, 0};

	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		// Rトリガーを押していたら
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			/* //////////////////////
			    攻撃処理
			*/ //////////////////////
			Attack();
			worldTransform_.scale_ -= 0.0005f;
		}
	} 

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {

		/* //////////////////////
		    攻撃処理
		*/ //////////////////////
		Attack();
		worldTransform_.scale_ -= 0.0001f;
	}

#ifdef _DEBUG
	/*ImGui::Begin("Reticle");
	ImGui::Text("2DReticle : (%f, %f)", spritePosition.x, spritePosition.y);
	ImGui::Text("Near(%+.2f, %+.2f, %+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far(%+.2f, %+.2f, %+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("MouseDirection(%+.2f, %+.2f, %+.2f)", mouseDirection.x, mouseDirection.y, mouseDirection.z);
	ImGui::Text("3DReticle : (%+.2f, %+.2f, %+.2f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();*/
#endif // DEBUG
}

/* //////////////////////////////////////////////////
                    攻撃処理
*/ //////////////////////////////////////////////////
void Player::Attack() {

	// 弾の速度
	const float kBulletSpeed = 1.5f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 自機の向いている方向に飛ばす
	velocity = GetForwardVector(worldTransform_.rotation_);

	velocity = Normalize(velocity) * kBulletSpeed;

	// 弾を生成し、初期化
	PlayerBullet* newBullet = new PlayerBullet();
	newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity);

	// 弾を登録する
	gameScene_->AddPlayerBullet(newBullet);
}

Vector3 Player::GetForwardVector(const Vector3& rotation) {
	float yaw = rotation.y; // Y軸回転 (ヨー)
	float pitch = rotation.x; // X軸回転 (ピッチ)

	return Vector3(
	    sinf(yaw), // X成分
	    -sinf(pitch),    // Y成分（無視）
	    cosf(yaw)  // Z成分
	);
}
