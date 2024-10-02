#include "Enemy.h"
#include "Player.h"
#include "GameScene.h"

//#include "imgui.h"

void Enemy::PhaseInitialize() {

	// 初期フェーズの設定
	phase_ = Phase::APPROACH;
}

void Enemy::ApproachPhase() {

	// 移動(ベクトルの減算)
	worldTransform_.translation_.x -= 0.05f;
}

void Enemy::LeasePhase() {

	// 移動(ベクトルを加算)
	worldTransform_.translation_.x += 0.05f;
}

void Enemy::Fire() {

	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;

	// 自キャラのワールド座標を取得
	Vector3 worldPlayerPos = player_->GetWorldPosition();

	// 敵キャラのワールド座標を取得
	Vector3 worldEnemyPos = GetWorldPosition();

	// 敵キャラから自キャラへの差分ベクトルを求める
	Vector3 vecGep = worldPlayerPos - worldEnemyPos;

	// ベクトルの正規化
	vecGep = Normalize(vecGep);

	// ベクトルの長さを、速さに合わせる
	Vector3 velocity = vecGep * kBulletSpeed;

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
}

void Enemy::CountTimer() {

	// 発射タイマーカウントダウン
	fireTimer--;

	// 指定時間に達した
	if (fireTimer <= 0) {

		// 弾を発射
		Fire();

		// 発射タイマーを初期化
		fireTimer = kFireInterval;
	}
}

void Enemy::OnCollision() {}

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 position) {

	// NULLポインターチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	// シングルトンインスタンス
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	// フェーズの初期化
	PhaseInitialize();
}

void Enemy::Update() {

	#ifdef _DEBUG

	ImGui::Begin("EnemyInfo");
	ImGui::DragFloat3("translation", &worldTransform_.translation_.x, 0.01f);
	ImGui::End();

	#endif // DEBUG
	
	switch (phase_) {
	case Enemy::Phase::APPROACH:

		// 接近処理
		ApproachPhase();

		// 規定の位置に到達したら離脱に切り替え
		if (worldTransform_.translation_.x < -10.0f) {
			phase_ = Phase::LEACE;
		}
		break;
	case Enemy::Phase::LEACE:

		// 後退処理
		LeasePhase();

		// 規定の距離に到達したら接近に切り替え
		if (worldTransform_.translation_.x > 30.0f) {
			phase_ = Phase::APPROACH;
		}
		break;
	default:
		break;
	}

	CountTimer();

	// 行列を定数バッファに転送
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
