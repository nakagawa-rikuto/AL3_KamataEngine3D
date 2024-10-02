#define NOMINMAX
#include "Player.h"
#include "GameScene.h"
#include "TextureManager.h"

#include "ImGui.h"

Player::~Player() { delete sprite2DReticle_; }

void Player::Move() {

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	// 押した方向で移動ベクトルを変更(左右)
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// おしたほうこうで移動ベクトルを変更(上下)
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// ジョイスティックの処理
	XINPUT_STATE joyState;
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX * 0.3f;
		move.y += static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX * 0.3f;
	} else if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// 移動限界
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 12.0f;

	// 範囲を超えない処理
	// Clampは－(マイナス)の値を取れない
	worldTransform_.translation_.x = std::max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = std::min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = std::max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = std::min(worldTransform_.translation_.y, kMoveLimitY);

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ += move;
}

void Player::Rotate() {

	// 回転速さ[ラジアン/ frame]
	const float kRotSpeed = 0.02f;

	// 押した方向で移動ベクトルを変更
	if (Input::GetInstance()->PushKey(DIK_A)) {

		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_D)) {

		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Reticle(ViewProjection& viewProjection) {

	/* /////////////////////////////////////
	       ゲームパッド
    */ /////////////////////////////////////
	// スプライトの現在座標を取得
	Vector2 spritePosition = sprite2DReticle_->GetPosition();

	XINPUT_STATE joyState;

	// ゲームパッド状態を維持取得
	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		spritePosition.x += static_cast<float>(joyState.Gamepad.sThumbRX) / SHRT_MAX * 5.0f;
		spritePosition.y -= static_cast<float>(joyState.Gamepad.sThumbRY) / SHRT_MAX * 5.0f;

		// スプライトの座標変更を反映
		sprite2DReticle_->SetPosition(spritePosition);

		// ゲームパッド未接続なら何もせずに抜ける
	} else if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
		return;
	}

	// Rトリガーを押していたら
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		/* //////////////////////
		    攻撃処理
		*/ //////////////////////
		Attack();
	}

	/* /////////////////////////////////////
	             行列の計算
	*/ /////////////////////////////////////
	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matWVP = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);
	// 合成行列を逆行列にする
	Matrix4x4 matInverseVPV = Inverse4x4(matWVP);

	/* /////////////////////////////////////
	            座標の変換
	*/ /////////////////////////////////////
	// スクリーン座標
	Vector3 posNear = {spritePosition.x, spritePosition.y, 0.0f};
	Vector3 posFar = {spritePosition.x, spritePosition.y, 1.0f};

	// スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverseVPV);
	posFar = Transform(posFar, matInverseVPV);

	/* /////////////////////////////////////
	               方向の設定
	*/ /////////////////////////////////////
	// マウスレイの方向
	Vector3 mouseDirection = Normalize(posFar - posNear);

	/* /////////////////////////////////////
	                座標の設定
	*/ /////////////////////////////////////
	// カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 50.0f;

	// ニアクリップ面上のワールド座標から一定距離前進したところに3Dレティクルを配置
	worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;
	worldTransform3DReticle_.translation_.z = worldTransform_.translation_.z + 30.0f;
	worldTransform3DReticle_.UpdateMatrix();

	#ifdef _DEBUG
	ImGui::Begin("Reticle");
	ImGui::Text("2DReticle : (%f, %f)", spritePosition.x, spritePosition.y);
	ImGui::Text("Near(%+.2f, %+.2f, %+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far(%+.2f, %+.2f, %+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("MouseDirection(%+.2f, %+.2f, %+.2f)", mouseDirection.x, mouseDirection.y, mouseDirection.z);
	ImGui::Text("3DReticle : (%+.2f, %+.2f, %+.2f)", worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();
#endif // DEBUG
}

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

void Player::Attack() {

	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	// 速度ベクトルを自機の向きに合わせて回転する
	// velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 自機から照準オブジェクトのベクトル
	velocity = worldTransform3DReticle_.translation_ - worldTransform_.translation_;
	velocity = Normalize(velocity) * kBulletSpeed;

	// 弾を生成し、初期化
	PlayerBullet* newBullet = new PlayerBullet();
	newBullet->Initialize(model_, GetWorldPosition(), velocity);

	// 弾を登録する
	gameScene_->AddPlayerBullet(newBullet);
}

void Player::OnCollision() {}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {

	// NULLポインターチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	// シングルトンインスタンス
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();
	worldTransform3DReticle_.translation_ = position;

	// レティクル用テクスチャ取得
	textureReticle_ = TextureManager::Load("./Resources/Reticle.png");

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle_, 
		Vector2(worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y), 
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), Vector2(0.5f, 0.5f));
}

void Player::Update(ViewProjection& viewProjection) {

#ifdef _DEBUG
	ImGui::Begin("Info");
	ImGui::DragFloat3("worldTransform.rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.scale", &worldTransform_.scale_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();
#endif // DEBUG

	/* //////////////////////
	        移動入力
	*/ //////////////////////
	Move();
	worldTransform_.translation_.z += 0.05f;

	/* //////////////////////
	        旋回処理
	*/ //////////////////////
	//Rotate();

	/* //////////////////////
	        レティクル
	*/ //////////////////////
	// Update3DReticle();
	Reticle(viewProjection);

	/* //////////////////////
	        行列計算
	*/ //////////////////////
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Player::DrawUI() {

	// 2Dレティクルを描画
	sprite2DReticle_->Draw();
}
