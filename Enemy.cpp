#include "Enemy.h"

Enemy::~Enemy() {

	for (EnemyBullet* bullet : bullets_) {

		delete bullet;
	}

	for (TimedCall* timedCall : timedCalls_) {
		delete timedCall;
	}
}

void Enemy::Fire() {

	// 弾の速度
	const float kBulletSpeed = 1.0f;
	Vector3 velocity(kBulletSpeed, 0, 0);

	// 速度ベクトルを自機の向きに合わせて回転する
	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::FireReset() {

	// 弾を発射する
	Fire();

	// 発射タイマーをセットする
	timedCalls_.push_back(new TimedCall(std::bind(&Enemy::FireReset, this), kFireInterval));
}

void Enemy::PhaseInitialize() {

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

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインターチェック
	assert(model);

	model_ = model;

	textureHandle_ = textureHandle;

	// シングルトンインスタンス
	worldTransform_.Initialize();
	worldTransform_.translation_.z = 5.0f;

	// 初期フェーズの設定
	phase_ = Phase::APPROACH;
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

	switch (phase_) {
	case Enemy::Phase::APPROACH:

		// 移動(ベクトルの減算)
		worldTransform_.translation_.x += 0.1f;
		worldTransform_.translation_.z -= 0.3f;

		// 規定の位置に到達したら離脱に切り替え
		if (worldTransform_.translation_.z < 0.0f) {
			phase_ = Phase::LEACE;
			timedCalls_.clear();
		}
		break;
	case Enemy::Phase::LEACE:

		// 移動(ベクトルを加算)
		worldTransform_.translation_.x -= 0.1f;
		worldTransform_.translation_.z += 0.3f;

		// 規定の距離に到達したら接近に切り替え
		if (worldTransform_.translation_.z > 10.0f) {
			phase_ = Phase::APPROACH;
		}
		break;
	default:
		break;
	}

	// 接近フェーズの初期化
	PhaseInitialize();

	// 弾の更新　
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
	 
	// 
	for (TimedCall* timedCall : timedCalls_) {
		timedCall->Update();
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
