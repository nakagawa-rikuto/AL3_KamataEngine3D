#include "Enemy.h"
#include "GameScene.h"
#include "Player.h"

void Enemy::PhaseInitialize() {

	// 初期フェーズの設定
	phase_ = Phase::APPROACH;
}

void Enemy::ApproachPhase() { Move(); }

void Enemy::CollisionPhase() {

	isTimer_--;

	if (isTimer_ <= 0.0f) {

		isDead_ = true;
	}
}

void Enemy::OnCollision() { isDead_ = true; }

void Enemy::OnCollision2() { 

	phase_ = Phase::Collsiion;
	//ImGui::DragFloat("Collision", &isTimer_, 1.0f);
}

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Move() {

	assert(player_);

	// 自キャラのワールド座標を取得
	Vector3 worldPlayerPos = player_->GetWorldPosition();

	// 敵キャラのワールド座標を取得
	Vector3 worldEnemyPos = GetWorldPosition();

	// 敵キャラから自キャラへの差分ベクトルを求める
	Vector3 vecGep = worldPlayerPos - worldEnemyPos;

	// ベクトルの正規化
	vecGep = Normalize(vecGep);

	// ベクトルの長さを、速さに合わせる
	Vector3 velocity = vecGep * speed_;

	worldTransform_.translation_ += velocity;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {

	// NULLポインターチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	// シングルトンインスタンス
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();
	

	isTimer_ = 30.0f;

	isDead_ = false;

	PhaseInitialize();
}

void Enemy::Update() {

#ifdef _DEBUG
	ImGui::Begin("EnemyInfo");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	if (phase_ == Phase::Collsiion) {
		ImGui::DragFloat("Collision", &isTimer_, 1.0f);
	}
	ImGui::End();
#endif // _DEBUG

	switch (phase_) {
	case Enemy::Phase::APPROACH:

		// 接近処理
		ApproachPhase();
		break;
	case Enemy::Phase::Collsiion:

		// 後退処理
		CollisionPhase();
		break;
	}

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
