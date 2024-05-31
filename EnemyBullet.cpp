#define NOMINMAX
#include "EnemyBullet.h"
#include "TextureManager.h"

EnemyBullet::~EnemyBullet() {}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

	// NULLポインタチェック
	assert(model);

	velocity_ = velocity;

	model_ = model;

	// テクスチャの読み込み
	textureHandle_ = TextureManager::Load("./Resources/uvChecker.png");

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	// 初期座標をセット
	worldTransform_.translation_ = position;
}

void EnemyBullet::Update() {

	worldTransform_.UpdateMatrix();

	// 座標を移動させる
	worldTransform_.translation_ += velocity_;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) { model_->Draw(worldTransform_, viewProjection, textureHandle_); }
