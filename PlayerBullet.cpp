#define NOMINMAX
#include "PlayerBullet.h"
#include "TextureManager.h"

PlayerBullet::~PlayerBullet() {}

void PlayerBullet::OnCollision() {
	isDead_ = true; 
}

Vector3 PlayerBullet::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {

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

void PlayerBullet::Update() { 

	worldTransform_.UpdateMatrix(); 

	// 座標を移動させる(1フレーム分の移動量を足しこむ)
	worldTransform_.translation_ += velocity_;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) { 

	model_->Draw(worldTransform_, viewProjection, textureHandle_); 
}
