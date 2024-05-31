#include "Enemy.h"
#include "Player.h"

Enemy::~Enemy() {

	for (EnemyBullet* bullet : bullets_) {

		delete bullet;
	}
}

void Enemy::Fire() {

	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 1.0f;

	// 自キャラのワールド座標を取得
	Vector3 worldPlayerPos = player_->GetWorldPosition();

	// 敵キャラのワールド座標を取得
	Vector3 worldEnemyPos = Enemy::GetWorldPosition();

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
	bullets_.push_back(newBullet);
}

void Enemy::CountTimer() {

	// 発射タイマーカウントダウン
	fireTimer--;
}

void Enemy::PhaseInitialize() {

	// 初期フェーズの設定
	phase_ = Phase::APPROACH;
}

void Enemy::ApproachPhase() {

	// 移動(ベクトルの減算)
	worldTransform_.translation_.z -= 0.05f;
}

void Enemy::LeasePhase() {

	// 移動(ベクトルを加算)
	worldTransform_.translation_.z += 0.05f;
}

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインターチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	// シングルトンインスタンス
	worldTransform_.Initialize();
	worldTransform_.translation_ = {4.0f, 1.0f, 5.0f};

	// フェーズの初期化
	PhaseInitialize();
}

void Enemy::Update() {

	// ですフラグの立った弾を削除
	// remove_if()はあくまでリストから要素を消すだけ
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	CountTimer();

	switch (phase_) {
	case Enemy::Phase::APPROACH:

		ApproachPhase();

		// 規定の位置に到達したら離脱に切り替え
		if (worldTransform_.translation_.z < -10.0f) {
			phase_ = Phase::LEACE;
		}
		break;
	case Enemy::Phase::LEACE:

		LeasePhase();

		// 規定の距離に到達したら接近に切り替え
		if (worldTransform_.translation_.z > 30.0f) {
			phase_ = Phase::APPROACH;
		}
		break;
	default:
		break;
	}

	// 指定時間に達した
	if (fireTimer <= 0) {

		// 弾を発射
		Fire();

		// 発射タイマーを初期化
		fireTimer = kFireInterval;
	}

	// 弾の更新　
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// 行列計算
	worldTransform_.UpdateMatrix();

	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	// 弾の描画
	for (EnemyBullet* bullet : bullets_) {

		bullet->Draw(viewProjection);
	}
}
