#define NOMINMAX
#include "Player.h"
#include "GameScene.h"

//#include "imgui.h"

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

Vector3 Player::GetWorldPosition() { 

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos; 
}

void Player::Attack() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転する
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

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

	// シングルトンインスタンス
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Player::Update() {

#ifdef _DEBUG
	ImGui::Begin("Info");
	ImGui::DragFloat3("worldTransform.rotation", &worldTransform_.rotation_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.scale", &worldTransform_.scale_.x, 0.01f);
	ImGui::DragFloat3("worldTransform.translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();
#endif // DEBUG

	/* //////////////////////
	        旋回処理
	*/ //////////////////////
	Rotate();

	/* //////////////////////
	        移動入力
	*/ //////////////////////
	Move();

	/* //////////////////////
	        攻撃処理
	*/ //////////////////////
	Attack();

	/* //////////////////////
	        行列計算
	*/ //////////////////////
	// 行列の更新
	worldTransform_.UpdateMatrix();

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

