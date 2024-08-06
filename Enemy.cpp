#include "Enemy.h"
#include "CollisionTypeIdDef.h"

// 移動処理
void Enemy::Move() {

	const float kEnemySpeed = 0.1f;     // 円軌道上の速度
	//const float kRotationSpeed = 0.05f; // 回転速度（ラジアン毎フレーム）
	Vector3 velocity(0, 0, kEnemySpeed);

	worldTransform_.rotation_.y += 0.01f;

	velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	worldTransform_.translation_ += velocity;
}

// 中心座標の取得
Vector3 Enemy::GetCenterPosition() const {

	// 見た目上の中心点オフセット(モデル座標)
	const Vector3 offset = {0.0f, 1.0f, 0.0f};

	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);
	return worldPos;
}

// 初期化
void Enemy::Initialize(const std::vector<Model*>& models) {

	// 基底クラスの初期化
	BaseCharacter::Initialize(models);

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = Vector3(5.0f, 0.0f, 10.0f);
	worldTransformBody_.Initialize();
	worldTransformWeapon_.Initialize();

	// 親子関係の構築
	worldTransformBody_.SetParent(&worldTransform_);
	worldTransformWeapon_.SetParent(&worldTransform_);

	// 種別IDの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));
}

// 更新
void Enemy::Update() {

	ImGui::DragFloat3("EnemyWorldTransform", &worldTransformBody_.translation_.x, 0.1f);

	Move();

	// 行列の再計算と転送
	BaseCharacter::Update();
	worldTransformBody_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}

// 描画
void Enemy::Draw(const ViewProjection& viewProjection) { 
	// 3Dモデルを描画
	models_[kModelIndexBody]->Draw(worldTransformBody_, viewProjection); 
	models_[kModelIndexWeapon]->Draw(worldTransformWeapon_, viewProjection);
}
