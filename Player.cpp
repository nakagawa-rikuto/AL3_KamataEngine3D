#define NOMINMAX
#include "TextureManager.h"
#include "Player.h"
#include "GameScene.h"

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

	// 押した方向で移動ベクトルを変更(前後)
	if (Input::GetInstance()->PushKey(DIK_W)) {
		move.z += kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_S)) {
		move.z -= kCharacterSpeed;
	}

	// 移動限界
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

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

void Player::Update3DReticle(ViewProjection& viewProjection) {

	// 自機のワールド座標から3Dレティクルのワールド座標を計算
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;

	// 自機から3Dレティクルへのオフセット(z+向き)
	Vector3 offset = {0.0f, 0.0f, 1.0f};

	// ベクトルの長さを整える
	offset = Multiply(offset, worldTransform_.matWorld_);

	// ベクトルの正規化
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;

	// 3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = {GetWorldPosition().x * offset.x, GetWorldPosition().y * offset.y, GetWorldPosition().z * offset.z};

	// WorldTransform3DReticle_のワールド行列更新と転送
	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = GetWorld3DReticlePosition();

	// ビューポート行列
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matWVP = Multiply(Multiply(viewProjection.matView, viewProjection.matProjection), matViewport);

	// ワールド
	positionReticle = Transform(positionReticle, matWVP);

	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
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

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転する
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 自機から照準オブジェクトのベクトル
		velocity = worldTransform3DReticle_.translation_ - GetWorldPosition();
		velocity = Normalize(velocity) * kBulletSpeed;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, Player::GetWorldPosition(), velocity);

		// 弾を登録する
		gameScene_->AddPlayerBullet(newBullet);
	}
}

void Player::OnCollision() {}

void Player::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {

	// NULLポインターチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	// レティクル用テクスチャ取得
	textureReticle_ = TextureManager::Load("./Resources/Reticle.png");

	// シングルトンインスタンス
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	// スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle_, 
		Vector2(worldTransform3DReticle_.translation_.x, worldTransform3DReticle_.translation_.y), 
		Vector4(1.0f, 1.0f, 1.0f, 1.0f), 
		Vector2(0.5f, 0.5f));
}

void Player::Update(ViewProjection& viewProjection) {

#ifdef _DEBUG

	ImGui::Begin("Info");
	ImGui::DragFloat3("worldTransform.rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.scale", &worldTransform_.scale_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::DragFloat3("worldTransform3DReticle.translation", &worldTransform3DReticle_.translation_.x, 0.01f);
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
	        攻撃処理
	*/ //////////////////////
	Attack();

	/* //////////////////////
	        3Dレティクル
	*/ //////////////////////
	Update3DReticle(viewProjection);

	/* //////////////////////
	        行列計算
	*/ //////////////////////
	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 3Dレティクルを描画
	model_->Draw(worldTransform3DReticle_, viewProjection);
}

void Player::DrawUI() {

	// 2Dレティクルを描画
	sprite2DReticle_->Draw();
}

